/*
 * Program to do Packet Delivery Ratio testing
 * To execute a test:
 * 	- program this firmware to 2 tags
 * 	- attach one tag to a serial port and run the pdr_logger.py script
 * 	- put this tag in rx mode by pressing button 1
 * 	- put the other tag in tx mode by pressing button 3
 *
 *  Created on: Feb 8, 2013
 *  Authors:
 *  	glenn.ergeerts@artesis.be
 */


#include <string.h>

#include <dll/dll.h>
#include <trans/trans.h>
#include <hal/system.h>
#include <hal/button.h>
#include <hal/leds.h>
#include <hal/rtc.h>
#include <msp430.h>
#include <framework/log.h>
#include <hal/uart.h>

#define SYNC_WORD 0xCE

#define CHANNEL_ID	0x18

#define INTERRUPT_BUTTON1 	(1)
#define INTERRUPT_BUTTON2 	(1 << 1)
#define INTERRUPT_BUTTON3 	(1 << 2)
#define INTERRUPT_RTC 		(1 << 3)

typedef enum {
	mode_idle,
	mode_tx,
	mode_rx
} app_mode_t;

static uint16_t counter = 0;
static uint8_t interrupt_flags = 0;
//static u8 tx_mode_enabled = 0;
static app_mode_t mode = mode_idle;
static bool start_channel_scan = true;

dll_channel_scan_t scan_cfg1 = {
		CHANNEL_ID,
		FrameTypeForegroundFrame,
		1000, // TODO increase this after stack supports receiving multiple packets during one scan
		0
};

dll_channel_scan_series_t scan_series_cfg;

void start_rx()
{
	start_channel_scan = false;
	dll_channel_scan_series(&scan_series_cfg);
	led_on(1);
}

void stop_rx()
{
	start_channel_scan = false;
	dll_stop_channel_scan();
	led_off(1);
}

void start_tx()
{
	led_on(3);
	trans_tx_foreground_frame((uint8_t*)&counter, sizeof(counter), 0xFF, CHANNEL_ID, 10);
}

void tx_callback(Trans_Tx_Result result)
{
	switch(result){
			case TransPacketSent:
				log_print_string("TX successful");
				counter++; // increment even if CCA failed
				led_off(3);
				break;
			case TransTCAFail:
				log_print_string("T_CA is less than the T_G");
				break;
			case TransPacketFail:
				log_print_string("Problem with the CCA");
				counter++; // increment even if CCA failed
				led_off(3);
				break;
		}

}

void rx_callback(dll_rx_res_t* rx_res)
{
	dll_foreground_frame_t* foreground_frame = (dll_foreground_frame_t*) rx_res->frame;
	uart_transmit_data(SYNC_WORD);
	uart_transmit_message(foreground_frame->address_ctl->source_id, 8);
	uart_transmit_message(foreground_frame->payload, 2);
	uart_transmit_message((uint8_t*) &rx_res->rssi, 1);
	led_toggle(3);

	start_channel_scan = true;
}

void main(void) {
	system_init();
	button_enable_interrupts();

	rtc_init_counter_mode();
	rtc_start();

	trans_init();
	trans_set_tx_callback(&tx_callback);
	dll_init();
	dll_set_rx_callback(&rx_callback);

	dll_channel_scan_t scan_confgs[1];
	scan_confgs[0] = scan_cfg1;

	scan_series_cfg.length = 1;
	scan_series_cfg.values = scan_confgs;

	while(1)
	{

        if(INTERRUPT_BUTTON1 & interrupt_flags)
        {
        	interrupt_flags &= ~INTERRUPT_BUTTON1;

        	if(mode != mode_rx)
        	{
        		mode = mode_rx;
        		start_channel_scan = true;
        	}
        	else
        	{
        		mode = mode_idle;
        		stop_rx();
        	}

        	button_clear_interrupt_flag();
        	button_enable_interrupts();
        }


        if (INTERRUPT_BUTTON3 & interrupt_flags)
        {
        	interrupt_flags &= ~INTERRUPT_BUTTON3;

        	if (mode == mode_tx)
			{
        		mode = mode_idle;
				rtc_disable_interrupt();
			} else {
				mode = mode_tx;
				stop_rx();
				rtc_enable_interrupt();
			}

        	button_clear_interrupt_flag();
        	button_enable_interrupts();
        }

        if (INTERRUPT_RTC & interrupt_flags)
		{
        	start_tx();

			interrupt_flags &= ~INTERRUPT_RTC;
		}

        if (start_channel_scan) start_rx();


		system_lowpower_mode(4,1);
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void)
{
	if(button_is_active(1))
		interrupt_flags |= INTERRUPT_BUTTON1;
	else
		interrupt_flags &= ~INTERRUPT_BUTTON1;

	if(button_is_active(3))
		interrupt_flags |= INTERRUPT_BUTTON3;
	else
		interrupt_flags &= ~INTERRUPT_BUTTON3;

	if(interrupt_flags != 0)
	{
		button_disable_interrupts();
		// TODO: debouncing
		LPM4_EXIT;
	}
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR (void)
{
	if(button_is_active(1))
		interrupt_flags |= INTERRUPT_BUTTON1;
	else
		interrupt_flags &= ~INTERRUPT_BUTTON1;

	if(button_is_active(3))
		interrupt_flags |= INTERRUPT_BUTTON3;
	else
		interrupt_flags &= ~INTERRUPT_BUTTON3;

	if(interrupt_flags != 0)
	{
		button_disable_interrupts();
		LPM4_EXIT;
	}
}

#pragma vector=RTC_VECTOR
__interrupt void RTC_ISR (void)
{
    switch (RTCIV){
        case 0: break;  //No interrupts
        case 2: break;  //RTCRDYIFG
        case 4:         //RTCEVIFG
        	interrupt_flags |= INTERRUPT_RTC;
        	LPM4_EXIT;
            break;
        case 6: break;  //RTCAIFG
        case 8: break;  //RT0PSIFG
        case 10: break; //RT1PSIFG
        default: break;
    }
}
