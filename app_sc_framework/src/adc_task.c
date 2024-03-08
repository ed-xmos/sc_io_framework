// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include <xcore/hwtimer.h>
#include <xcore/select.h>
#include <xcore/port.h>
#include <xcore/triggerable.h>

#include "app_main.h"

#define ADC_READ_INTERVAL   (10 * XS1_TIMER_KHZ)    // Time in between individual conversions in milliseconds
#define ADC_CHARGE_PERIOD   (500 * XS1_TIMER_MHZ)   // Time to charge cap in microseconds
#define ADC_NOISE_DEADBAND   2                      // change of 1/256 will not trigger  

#define ABS(x) ((x)<0 ? (-x) : (x))

static enum adc_state{
        ADC_IDLE = 2,
        ADC_CHARGING = 1,
        ADC_CONVERTING = 0 // Optimisation as ISA can do != 0 on select guard
}adc_state;



void adc_task(void){
    printstrln("adc_task");
    triggerable_disable_all();

    port_t p_adc[] = {XS1_PORT_1A, XS1_PORT_1D}; // X0D00, 11
    unsigned p_adc_idx = 0;
    const unsigned num_ports = sizeof(p_adc) / sizeof(port_t);

    unsigned results[num_ports] = {0}; // The ADC read values

    for(unsigned i = 0; i < num_ports; i++){
        port_enable(p_adc[i]);
        set_pad_properties(p_adc[i], DRIVE_2MA, PULL_NONE, 0, 0);
        // set_pad_drive_mode(p_adc, DRIVE_BOTH);

        port_clear_trigger_in(p_adc[i]);
        triggerable_enable_trigger(p_adc[i]);
    }

    hwtimer_t t_adc = hwtimer_alloc();
    int discharge_start_time = 0;
    int trigger_time = hwtimer_get_time(t_adc);
    trigger_time += ADC_READ_INTERVAL;

    adc_state = ADC_IDLE;

    hwtimer_set_trigger_time(t_adc, trigger_time);
    triggerable_enable_trigger(t_adc);

    // We read through different line
    unsigned p_adc_idx_other = 0;


    printstrln("adc_task");

    while(0){
        port_out(p_adc[0], 1);
        port_out(p_adc[1], 1);
        delay_microseconds(500);
        port_in(p_adc[0]);
        port_out(p_adc[1], 0);
        delay_milliseconds(2);

        port_out(p_adc[0], 1);
        port_out(p_adc[1], 1);
        delay_microseconds(500);
        port_in(p_adc[1]);
        port_out(p_adc[0], 0);
        delay_milliseconds(100);
    }

    while(1){
        SELECT_RES(
            CASE_GUARD_THEN(t_adc, adc_state != ADC_CONVERTING, adc_timer_event),
            CASE_GUARD_THEN(p_adc[p_adc_idx_other], adc_state == ADC_CONVERTING, adc_read)
        )
        {
            adc_timer_event:
            {
                switch(adc_state){
                    case ADC_IDLE:
                        for(unsigned i = 0; i < num_ports; i++){
                            port_out(p_adc[i], 1); // Drive a 1 to charge the capacitor via all ines
                        } 
                        trigger_time += ADC_CHARGE_PERIOD;
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
                        trigger_time += ADC_READ_INTERVAL - ADC_CHARGE_PERIOD;
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

                results[p_adc_idx] = discharge_elapsed_time;

                hwtimer_set_trigger_time(t_adc, trigger_time);
                adc_state = ADC_IDLE;
                if(++p_adc_idx == num_ports){
                    p_adc_idx = 0;
                    printf("ADCs: %d %d\n", results[0], results[1]);
                }
            }
            break;
        }
    }
}