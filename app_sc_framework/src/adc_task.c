// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include "adc_task.h"
#include <xcore/hwtimer.h>
#include <xcore/select.h>
#include <xcore/triggerable.h>
#include <xcore/assert.h>
#include <xcore/channel.h>


#include "app_main.h"


typedef enum adc_state_t{
        ADC_IDLE = 2,
        ADC_CHARGING = 1,
        ADC_CONVERTING = 0 // Optimisation as ISA can do != 0 on select guard
}adc_state_t;

typedef enum adc_mode_t{
        ADC_CONVERT = 0,
        ADC_CALIBRATION_MANUAL,
        ADC_CALIBRATION_AUTO        // WIP
}adc_mode_t;

static inline int post_process_result(  int discharge_elapsed_time,
                                        unsigned *zero_offset_ticks,
                                        unsigned max_offsetted_conversion_time[],
                                        int *conversion_history,
                                        int *hysteris_tracker,
                                        unsigned adc_idx,
                                        unsigned num_ports,
                                        adc_mode_t adc_mode){
    const int max_result_scale = (1 << ADC_BITS_TARGET) - 1;

    // Apply filter. First populate filter history.
    static unsigned filter_write_idx = 0;
    static unsigned filter_stable = 0;
    unsigned offset = adc_idx * RESULT_HISTORY_DEPTH + filter_write_idx;
    *(conversion_history + offset) = discharge_elapsed_time;
    if(adc_idx == num_ports - 1){
        if(++filter_write_idx == RESULT_HISTORY_DEPTH){
            filter_write_idx = 0;
            filter_stable = 1;
        }
    }
    // Moving average filter
    int accum = 0;
    int *hist_ptr = conversion_history + adc_idx * RESULT_HISTORY_DEPTH;
    for(int i = 0; i < RESULT_HISTORY_DEPTH; i++){
        accum += *hist_ptr;
        hist_ptr++;
    }
    int filtered_elapsed_time = accum / RESULT_HISTORY_DEPTH;

    // Remove zero offset and clip
    int zero_offsetted_ticks = filtered_elapsed_time - *zero_offset_ticks;
    if(zero_offsetted_ticks < 0){
        if(filter_stable){
            // *zero_offset_ticks += (zero_offsetted_ticks / 2); // Move zero offset halfway to compensate gradually
        }
        zero_offsetted_ticks = 0;
    }

    // Clip count positive
    if(zero_offsetted_ticks > max_offsetted_conversion_time[adc_idx]){
        if(adc_mode == ADC_CALIBRATION_MANUAL){
            max_offsetted_conversion_time[adc_idx] = zero_offsetted_ticks;
        } else {
            zero_offsetted_ticks = max_offsetted_conversion_time[adc_idx];  
        }
    }

    // Calculate scaled output
    int scaled_result = 0;
    if(max_offsetted_conversion_time[adc_idx]){ // Avoid / 0 during calibrate
        scaled_result = (max_result_scale * zero_offsetted_ticks) / max_offsetted_conversion_time[adc_idx];
    }

    // // Clip positive and move max if needed
    // if(scaled_result > max_result_scale){
    //     scaled_result = max_result_scale; // Clip
    //     // Handle moving up the maximum val
    //     if(adc_mode == ADC_CALIBRATION_MANUAL){
    //         int new_max_offsetted_conversion_time = (max_result_scale * zero_offsetted_ticks) / scaled_result;
    //         max_offsetted_conversion_time[adc_idx] += (new_max_offsetted_conversion_time - max_offsetted_conversion_time[adc_idx]);
    //     }
    // }

    // Apply hysteresis
    if(scaled_result > hysteris_tracker[adc_idx] + RESULT_HYSTERESIS || scaled_result == max_result_scale){
        hysteris_tracker[adc_idx] = scaled_result;
    }
    if(scaled_result < hysteris_tracker[adc_idx] - RESULT_HYSTERESIS || scaled_result == 0){
        hysteris_tracker[adc_idx] = scaled_result;
    }

    scaled_result = hysteris_tracker[adc_idx];

    return scaled_result;
}


static int stored_max[ADC_NUM_CHANNELS] = {0};
int adc_save_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports) __attribute__ ((weak));
int adc_save_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports)
{
    for(int i = 0; i < num_ports; i++){
        stored_max[i] = max_offsetted_conversion_time[i];
    }

    return 0; // Success
}

int adc_load_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports) __attribute__ ((weak));
int adc_load_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports)
{
    for(int i = 0; i < num_ports; i++){
        max_offsetted_conversion_time[i] = stored_max[i];
    }

    return 0; // Success
}

void adc_task(chanend_t c_adc){
    printstrln("adc_task");
    triggerable_disable_all();

    port_t p_adc[] = ADC_PINS;
    unsigned p_adc_idx = 0;
    const unsigned num_ports = ADC_NUM_CHANNELS;
    adc_mode_t adc_mode = ADC_CONVERT;


    unsigned results[num_ports] = {0}; // The ADC read values scaled to max_result_scale

    const int port_drive = DRIVE_8MA;
    const int capacitor_nf = 10;
    const int resistor_ohms_max = 10500; // nominal maximum value
    // const int resistor_ohms_min = 9500; // nominal minimum value
    const int resistor_ohms_min = 4500; // nominal minimum value

    const int charge_resistance_max = resistor_ohms_max / num_ports;
    const int rc_times_to_charge_fully = 10; // 5 RC times should be sufficient but slightly better crosstalk from 10
    const int max_charge_period_ticks = (rc_times_to_charge_fully * capacitor_nf * charge_resistance_max) / 10;

    const int max_discharge_period_ticks = (capacitor_nf * resistor_ohms_max) / 10;
    xassert(ADC_READ_INTERVAL > max_charge_period_ticks + max_discharge_period_ticks * 2); // Ensure conversion rate is low enough. *2 to allow post processing time
    printintln(ADC_READ_INTERVAL); printintln(max_charge_period_ticks +max_discharge_period_ticks);

    // Calaculate zero offset based on drive strength
    unsigned zero_offset_ticks = 0;
    switch(port_drive){
        case DRIVE_2MA:
            zero_offset_ticks = capacitor_nf * 24;
            printstrln("DRIVE_2MA\n");
        break;
        case DRIVE_4MA:
            zero_offset_ticks = capacitor_nf * 12;
            printstrln("DRIVE_4MA\n");
        break;
        case DRIVE_8MA:
            zero_offset_ticks = capacitor_nf * 8;
            printstrln("DRIVE_8MA\n");
        break;
        case DRIVE_12MA:
            zero_offset_ticks = capacitor_nf * 6;
            printstrln("DRIVE_12MA\n");
        break;
    }

    // For filter
    int conversion_history[num_ports][RESULT_HISTORY_DEPTH] = {{0}};
    int hysteris_tracker[num_ports] = {0};

    // Calibration for scaling to full scale
    unsigned max_offsetted_conversion_time[num_ports] = {0};

    // Initialise all ports and apply estimated max conversion
    for(unsigned i = 0; i < num_ports; i++){
        max_offsetted_conversion_time[i] = (resistor_ohms_min * capacitor_nf * 100) / 1035; // Calibration factor of / 10.35

        port_enable(p_adc[i]);
        set_pad_properties(p_adc[i], port_drive, PULL_NONE, 0, 0);

        port_clear_trigger_in(p_adc[i]);
        triggerable_enable_trigger(p_adc[i]);
    }

    hwtimer_t t_adc = hwtimer_alloc();
    int discharge_start_time = 0;
    int trigger_time = hwtimer_get_time(t_adc);
    trigger_time += ADC_READ_INTERVAL;

    adc_state_t adc_state = ADC_IDLE;

    hwtimer_set_trigger_time(t_adc, trigger_time);
    triggerable_enable_trigger(t_adc);

    // We read through different line than the one being discharged
    unsigned p_adc_idx_other = 0;

    printstrln("adc_task");

    while(1){
        SELECT_RES(
            CASE_GUARD_THEN(t_adc, adc_state != ADC_CONVERTING, adc_timer_event),
            CASE_GUARD_THEN(p_adc[p_adc_idx_other], adc_state == ADC_CONVERTING, adc_read),
            CASE_THEN(c_adc, adc_process_command)
        )
        {
            adc_timer_event:
            {
                switch(adc_state){
                    case ADC_IDLE:
                        for(unsigned i = 0; i < num_ports; i++){
                            port_out(p_adc[i], 1); // Drive a 1 to charge the capacitor via all ines
                        } 
                        trigger_time += max_charge_period_ticks;
                        hwtimer_change_trigger_time(t_adc, trigger_time);
                        adc_state = ADC_CHARGING;
                    break;

                    case ADC_CHARGING:
                        hwtimer_clear_trigger_time(t_adc);
                        p_adc_idx_other = (p_adc_idx + 1) % num_ports; // We will read from a different line. Any will do.
                        for(unsigned i = 0; i < num_ports; i++){
                            port_in(p_adc[i]); // Make all inputs - hold charge for now
                        } 
                        port_set_trigger_in_equal(p_adc[p_adc_idx_other], 0); // Trigger ADC read when cap discharges to a read 0
                        trigger_time += ADC_READ_INTERVAL - max_charge_period_ticks;
                        adc_state = ADC_CONVERTING;

                        port_out(p_adc[p_adc_idx], 0); // Drive low to start discharge
                        discharge_start_time = hwtimer_get_time(t_adc);
                    break;

                    default:
                        printstr("Unreachable\n");
                    break;
                }
            }
            break;

            adc_read:
            {
                int discharge_end_time = hwtimer_get_time(t_adc);
                int discharge_elapsed_time = discharge_end_time - discharge_start_time;
                port_clear_trigger_in(p_adc[p_adc_idx_other]);
                port_in(p_adc[p_adc_idx]); // Terminate discharge. No point in fully discharging cap.

                results[p_adc_idx] = post_process_result(discharge_elapsed_time,
                                                        &zero_offset_ticks,
                                                        max_offsetted_conversion_time,
                                                        (int*)conversion_history,
                                                        hysteris_tracker,
                                                        p_adc_idx,
                                                        num_ports,
                                                        adc_mode);

                hwtimer_set_trigger_time(t_adc, trigger_time);
                adc_state = ADC_IDLE;
                if(++p_adc_idx == num_ports){
                    p_adc_idx = 0;
                    // printf("ADCs: %d %d\n", results[0], results[1]);
                    // printintln(discharge_elapsed_time);
                }
            }
            break;

            adc_process_command:
            {
                uint32_t command = chan_in_word(c_adc);
                switch(command & ADC_CMD_MASK){
                    case ADC_CMD_READ:
                        (void)command; // Work around  error: expected expression
                        unsigned idx = command & ~ADC_CMD_MASK;
                        chan_out_word(c_adc, results[idx]);
                    break;

                    case ADC_CMD_CAL_MODE_START:
                        adc_mode = ADC_CALIBRATION_MANUAL;
                        for(unsigned i = 0; i < num_ports; i++){
                            max_offsetted_conversion_time[i] = 0;
                        }

                        printstrln("ADC_CALIBRATION_MANUAL");
                    break;

                    case ADC_CMD_CAL_MODE_FINISH:
                        adc_mode = ADC_CONVERT;
                        adc_save_calibration(max_offsetted_conversion_time, num_ports);
                        printstrln("ADC_CALIBRATION_SAVED");
                    break; 
                }
            }
            break;
        } // Select
    } // while 1
}