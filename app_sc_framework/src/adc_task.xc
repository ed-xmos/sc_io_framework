// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>
#include <xs1.h>
#include <platform.h>
#include <assert.h>

#include "adc_task.h"
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

    return filtered_elapsed_time;

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


// static int stored_max[ADC_NUM_CHANNELS] = {0};
// int adc_save_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports) __attribute__ ((weak));
// int adc_save_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports)
// {
//     for(int i = 0; i < num_ports; i++){
//         stored_max[i] = max_offsetted_conversion_time[i];
//     }

//     return 0; // Success
// }

// int adc_load_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports) __attribute__ ((weak));
// int adc_load_calibration(unsigned max_offsetted_conversion_time[], unsigned num_ports)
// {
//     for(int i = 0; i < num_ports; i++){
//         max_offsetted_conversion_time[i] = stored_max[i];
//     }

//     return 0; // Success
// }

on tile[0]: port p_adc[] = ADC_PINS;


void adc_task(chanend c_adc){
    printstrln("adc_task");
  

    // Current conversion index
    unsigned adc_idx = 0;

    adc_mode_t adc_mode = ADC_CONVERT;
    timer tmr_charge;
    timer tmr_discharge;
    timer tmr_overshoot;

    // Set all ports to input
    for(int i = 0; i < ADC_NUM_CHANNELS; i++){
        unsigned dummy;
        p_adc[i] :> dummy;
    }

    unsigned results[ADC_NUM_CHANNELS] = {0}; // The ADC read values scaled to max_result_scale

    const int port_drive = DRIVE_8MA;
    const int capacitor_nf = 10;
    const int resistor_ohms_max = 10000 / 2; // nominal maximum value

    const int rc_times_to_charge_fully = 10; // 5 RC times should be sufficient but slightly better crosstalk from 10
    const int max_charge_period_ticks = (rc_times_to_charge_fully * capacitor_nf * resistor_ohms_max) / 10;

    const int max_discharge_period_ticks = (capacitor_nf * resistor_ohms_max) / 10;
    assert(ADC_READ_INTERVAL > max_charge_period_ticks + max_discharge_period_ticks * 2); // Ensure conversion rate is low enough. *2 to allow post processing time
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
    int conversion_history[ADC_NUM_CHANNELS][RESULT_HISTORY_DEPTH] = {{0}};
    int hysteris_tracker[ADC_NUM_CHANNELS] = {0};

 
    adc_state_t adc_state = ADC_IDLE;

    // Set init time for charge
    int time_trigger_charge = 0;
    tmr_charge :> time_trigger_charge;
    time_trigger_charge += max_charge_period_ticks; // start in one conversion period's
    
    int time_trigger_discharge = 0;
    int time_trigger_overshoot = 0;

    int16_t start_time, end_time;
    unsigned init_port_val = 0;

    printstrln("adc_task");
    while(1){
        select{
            case adc_state == ADC_IDLE => tmr_charge when timerafter(time_trigger_charge) :> int _:
                p_adc[adc_idx] :> init_port_val;
                time_trigger_discharge = time_trigger_charge + max_charge_period_ticks;

                p_adc[adc_idx] <: init_port_val ^ 0x1; // Drive opposite to what we read to "charge"
                adc_state = ADC_CHARGING;
            break;

            case adc_state == ADC_CHARGING => tmr_discharge when timerafter(time_trigger_discharge) :> int _:
                p_adc[adc_idx] :> int _ @ start_time; // Make Hi Z and grab time
                time_trigger_overshoot = time_trigger_discharge + max_charge_period_ticks;

                adc_state = ADC_CONVERTING;
            break;

            case adc_state == ADC_CONVERTING => p_adc[adc_idx] when pinseq(init_port_val) :> int _ @ end_time:
                printintln(end_time - start_time);
                
                if(++adc_idx == ADC_NUM_CHANNELS){
                    adc_idx = 0;
                }
                tmr_charge :> time_trigger_charge; // Start next conversion straight away

                adc_state == ADC_IDLE;
            break;

            case adc_state == ADC_CONVERTING => tmr_overshoot when timerafter(time_trigger_overshoot) :> int _:
                printstrln("overshoot");
                p_adc[adc_idx] :> int _ @ end_time;
                printintln(end_time - start_time);

                if(++adc_idx == ADC_NUM_CHANNELS){
                    adc_idx = 0;
                }
                tmr_charge :> time_trigger_charge; // Start next conversion straight away

                adc_state == ADC_IDLE;
            break;
        }

    } // while 1
}