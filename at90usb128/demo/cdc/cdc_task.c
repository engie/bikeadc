//!
//! @file cdc_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file manages the CDC task.
//!
//!
//! @version 1.6 at90usb128-demo-cdc-1_0_3 $Id: cdc_task.c,v 1.6 2006/03/17 13:06:22 rletendu Exp $
//!
//! @todo
//! @bug
//!/

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf_usb.h"
#include "cdc_task.h"
#include "lib_mcu/usb/usb_drv.h"
#include "usb_descriptors.h"
#include "modules/usb/device_chap9/usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/uart/uart_lib.h"
#include "uart_usb_lib.h"
#include <stdio.h>

//#include "lib_mcu/mcu.h"


//_____ M A C R O S ________________________________________________________





//_____ D E F I N I T I O N S ______________________________________________



//_____ D E C L A R A T I O N S ____________________________________________


volatile U8 cpt_sof;
extern U8    rx_counter;
extern U8    tx_counter;
S_line_coding line_coding;

typedef unsigned short uint16;
static uint16 voltage;
static uint16 current;

void selectADCChannel( int channel )
{
    ADMUX = (1u << REFS0 )
        //| (1u << REFS1 ) //2.56V reference
        | channel;
}

static int channel = 0;
static int cpt_adc = 0;
void adc_init()
{
    selectADCChannel( channel );
    ADCSRB = 0; //Free running mode. Not used.
    DIDR0 = 0xFF; //Disable digital inputs on port F 
    ADCSRA = ( 1u << ADEN ) //ADC Enabled
        | ( 1u << ADSC ) //Start conversion
        | ( 1u << ADPS2 )
        | ( 1u << ADPS1 )
        | ( 1u << ADPS0 ); // divide clock by 128 
}

int adc_read()
{
    if( ADCSRA & (1u << ADIF) )
    {
        bool success = 0;
        //Got a reading. HAVE TO READ LOWER FIRST ARGAGRG
        uint16 reading = ADCL;
        reading |= ((uint16) ADCH) << 8;

        if( channel == 0 )
        {
            voltage = reading;
            channel = 1;
        }
        else
        {
            current = reading;
            channel = 0;
            success = 1;
        }
        selectADCChannel( channel );

        //Clear the flag
        ADCSRA |= (1u << ADIF);
        //Start the next one
        ADCSRA |= (1u << ADSC);
        return success;
    }

    return 0;
}

//! @brief This function initializes the hardware ressources required for CDC demo.
//!
//!
//! @param none
//!
//! @return none
//!
//!/
void cdc_task_init(void)
{
    DDRF = 0; //Port F is an input port
    PORTF = 0; //Disable pullups
    uart_init();
    Leds_init();
    Hwb_button_init();
    Usb_enable_sof_interrupt();
#ifdef AVRGCC
    fdevopen(uart_usb_putchar,uart_usb_getchar); //for printf redirection 
#endif
}


//! @brief Entry point of the uart cdc management
//!
//! This function links the uart and the USB bus.
//!
//! @param none
//!
//! @return none
static int waiting_to_start = 1;
void cdc_task(void)
{
    if(Is_device_enumerated()) //Enumeration processs OK ?
    {
        if(cpt_sof>=NB_MS_BEFORE_FLUSH && tx_counter!=0 )  //Flush buffer in Timeout
        {
            cpt_sof=0;
            uart_usb_flush();
        }

        if (uart_test_hit())    //Something on USART ?
        {
            uart_usb_putchar(uart_getchar());   // Loop back, USART to USB
        }

        if ( waiting_to_start && uart_usb_test_hit())   // Something received from the USB ?
        {
            waiting_to_start = 0;
            adc_init();
        }

        if( cpt_adc == 1 )
        {
            if( adc_read() )
            {
                uint16 checksum = voltage ^ current;
                printf("%03x,%03x,%03x\r\n", voltage, current, checksum);
                Led0_toggle();
            }
            cpt_adc = 0;
        }
    }
}



//! @brief sof_action
//!
//! This function increments the cpt_sof counter each times
//! the USB Start Of Frame interrupt subroutine is executed (1ms)
//! Usefull to manage time delays
//!
//! @warning Code:?? bytes (function code length)
//!
//! @param none
//!
//! @return none
void sof_action()
{
    cpt_sof++;
    cpt_adc = 1;
}
