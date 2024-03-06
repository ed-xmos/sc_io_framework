// Copyright 2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#include <print.h>
#include <stdio.h>

#include <xcore/hwtimer.h>
#include <xcore/select.h>
#include <xcore/port.h>
#include <xcore/triggerable.h>

#include "app_main.h"

#define ADC_READ_INTERVAL   (500 * XS1_TIMER_KHZ)   // Time in between conversions
#define ADC_CHARGE_PERIOD   (200 * XS1_TIMER_MHZ)   // Time to charge cap
#define ADC_NOISE_DEADBAND        2             // change of 1/256 will not trigger  

#define ABS(x) ((x)<0 ? (-x) : (x))

static enum adc_state{
        ADC_IDLE = 0,
        ADC_CHARGING,
        ADC_CONVERTING
}adc_state;

static int remove_offset(int discharge_time) {
  int offsetted = (discharge_time - 270);
  if (offsetted < 0) offsetted = 0;
  return offsetted;
}



void adc_task(void){
    printstrln("adc_task");

    port_t p_adc = XS1_PORT_1P; // X1D39
    port_enable(p_adc);
    set_pad_properties(p_adc, DRIVE_8MA, PULL_UP_WEAK, 0, 0);

    hwtimer_t t_adc = hwtimer_alloc();
    adc_state = ADC_IDLE;

    int processed_val = 0;
    int discharge_start_time = 0;

    int trigger_time = hwtimer_get_time(t_adc);
    trigger_time += ADC_READ_INTERVAL;

    hwtimer_set_trigger_time(t_adc, trigger_time);

    triggerable_enable_trigger(t_adc);

    printstrln("adc_task");

    while(1) {
        SELECT_RES(
            CASE_THEN(t_adc, adc_timer_event),
            CASE_GUARD_THEN(p_adc, adc_state == ADC_CONVERTING, adc_reading)
        )
        {
            adc_timer_event:
            {
                switch(adc_state){
                    case ADC_IDLE:               
                        printstr("adc_charging\n");
                        port_out(p_adc, 1); // Drive a 1 to charge the capacitor
                        trigger_time += ADC_CHARGE_PERIOD;
                        hwtimer_set_trigger_time(t_adc, trigger_time);
                        adc_state = ADC_CHARGING;
                    break;
                    case ADC_CHARGING:
                        printstr("adc_converting\n");
                        port_in(p_adc); // Make input
                        discharge_start_time = trigger_time;
                        port_set_trigger_in_equal(p_adc, 0);
                        triggerable_enable_trigger(p_adc);
                        adc_state = ADC_CONVERTING;
                    break;
                    default:
                        printstr("Error - timer event in ADC_CONVERTING\n");
                    break;
                }
            }
            continue;

            adc_reading:
            {
                printstr("adc_reading\n");
                int discharge_end_time = hwtimer_get_time(t_adc);
                int discharge_elapsed_time = discharge_end_time - discharge_start_time;

                triggerable_disable_trigger(p_adc);

                printintln(discharge_elapsed_time);

                trigger_time += ADC_READ_INTERVAL - ADC_CHARGE_PERIOD;
                hwtimer_set_trigger_time(t_adc, trigger_time);

                adc_state = ADC_IDLE;
            }
            continue;
        }
    }
}
/*

        case (adc_state == ADC_CONVERTING) => p_adc when pinseq(0) :> int _:
        t_periodic :> discharge_end_time;
            unsigned discharge_time = discharge_end_time - discharge_start_time;
            int upper = (last_discharge_time * (256 + ADC_NOISE_DEADBAND)) >> 8;
            int lower = (last_discharge_time * (256 - ADC_NOISE_DEADBAND)) >> 8;
            if ((discharge_time > upper) || (discharge_time < lower)) {
                processed_val = remove_offset(discharge_time);
                i_resistor.value_change_event();
                last_discharge_time = discharge_time;
            }

            adc_state = ADC_IDLE;
        break;
        }
    }
    }
*/

