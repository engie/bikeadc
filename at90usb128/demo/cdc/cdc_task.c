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
#include <avr/interrupt.h>
#include <stdio.h>

//#include "lib_mcu/mcu.h"


//_____ M A C R O S ________________________________________________________





//_____ D E F I N I T I O N S ______________________________________________



//_____ D E C L A R A T I O N S ____________________________________________

void timer_init()
{
    //8MHz external clock
    //Set timer0 to 8MHz/64 = 125KHz
    //Counter to fire at  = 2KHz (ish)
    TCCR0A = (1u << WGM01); //CTC Mode
    TCCR0B = (1u << CS01)
           | (1u << CS00); //Divide by 64
    OCR0A = 63;
    OCR0B = 0; //Reset when = 63
    TIMSK0 = (1u << OCIE0A); //This triggers the ADC
}

volatile U8 cpt_sof;
extern U8    rx_counter;
extern U8    tx_counter;
S_line_coding line_coding;

typedef unsigned short uint16;
typedef struct { uint16 voltage; uint16 current; } READING;
typedef uint16 CB_t;
#define CB_SIZE 1024
static READING cb_data[CB_SIZE];
volatile static CB_t cb_head = 1;
volatile static CB_t cb_tail = 0;

CB_t next_index( CB_t i )
{
    i += 1;
    if( i == CB_SIZE )
    {
        i = 0;
    }

    return i;
}

void cb_push( READING* r )
{
    CB_t next_head = next_index(cb_head);

    if( next_head == cb_tail )
    {
        Led0_toggle();
        return;
    }

    cb_data[cb_head] = *r;
    cb_head = next_head;
}

int cb_pop( READING* r )
{
    CB_t next_tail = next_index( cb_tail );

    if( next_tail == cb_head )
    {
        return 0;
    }

    *r = cb_data[cb_tail];
    cb_tail = next_tail;
    return 1;
}

void selectADCChannel( int channel )
{
    ADMUX = (1u << REFS0 )
        //| (1u << REFS1 ) //2.56V reference
        | channel;
}

static int channel = 1;
void adc_init()
{
    //Need 25 cycles per 2kHZ tick
    selectADCChannel( channel );
    ADCSRB = ( 1u << ADTS0 )
           | ( 1u << ADTS1 ); //Trigger = Timer/counter0 Compare match
    DIDR0 = 0xFF; //Disable digital inputs on port F 
    ADCSRA = ( 1u << ADEN ) //ADC Enabled
           | ( 1u << ADATE ) //Start conversion on trigger
           | ( 1u << ADIE ) //Enable interrupt
           | ( 1u << ADPS2 )
           | ( 1u << ADPS1 ); //Divide by 64 to give 125KHz clock
}

ISR(TIMER0_COMPA_vect)
{
}

static READING cur_reading;
static uint16 count = 0;
ISR(ADC_vect)
{
    //Got a reading. HAVE TO READ LOWER FIRST ARGAGRG
    uint16 reading = ADCL;
    reading |= ((uint16) ADCH) << 8;

    if( channel == 1 )
    {
        cur_reading.voltage = reading;
        channel = 7;
    }
    else
    {
        cur_reading.current = reading;
        channel = 1;
        cb_push( &cur_reading );
        count++;
    }
    selectADCChannel( channel );

    //Clear the flag
    ADCSRA |= (1u << ADIF);
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

        if ( waiting_to_start && uart_usb_test_hit())   // Something received from the USB ?
        {
            waiting_to_start = 0;
            timer_init();
            adc_init();
        }

        if( !waiting_to_start )
        {
            READING r;
            if( cb_pop( &r ) )
            {
                uart_usb_putchar( (r.voltage >> 8) & 0xFF );
                uart_usb_putchar( r.voltage & 0xFF );
                uart_usb_putchar( (r.current >> 8) & 0xFF  );
                uart_usb_putchar( r.current & 0xFF );

                uint16 checksum = r.voltage ^ r.current;
                uart_usb_putchar( (checksum >> 8) & 0xFF  );
                uart_usb_putchar( checksum & 0xFF );
            }
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
}
