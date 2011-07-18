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
#include "lib_mcu\usb\usb_drv.h"
#include "usb_descriptors.h"
#include "modules\usb\device_chap9\usb_standard_request.h"
#include "usb_specific_request.h"
#include "lib_mcu/uart/uart_lib.h"
#include "uart_usb_lib.h"
#include <stdio.h>



//_____ M A C R O S ________________________________________________________





//_____ D E F I N I T I O N S ______________________________________________



//_____ D E C L A R A T I O N S ____________________________________________


volatile U8 cpt_sof;
extern U8    rx_counter;
extern U8    tx_counter;
S_line_coding line_coding;


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
   uart_init();
   Leds_init();
   Joy_init();
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
         Led0_toggle();
  	   }
   
      if (Uart_tx_ready())    //USART free ?
      {
         if (uart_usb_test_hit())   // Something received from the USB ?
	      {
	 	      while (rx_counter)
		      {
			      uart_putchar(uart_usb_getchar());   // loop back USB to USART
               Led3_toggle();
		      }
	      }
      }

      if ( cpt_sof>=REPEAT_KEY_PRESSED)   //Debounce joystick events
      {
         if (Is_joy_select())
         printf ("Select Pressed !\r\n");

         if (Is_joy_right())
         printf ("Right Pressed !\r\n");
   
         if (Is_joy_left())
         printf ("Left Pressed !\r\n");

         if (Is_joy_down())
         printf ("Down Pressed !\r\n");

         if (Is_joy_up())
         printf ("Up Pressed !\r\n");

         if(Is_hwb())
         printf("Hello from AT90USBXXX !\r\n");
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
