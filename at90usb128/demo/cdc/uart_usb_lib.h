//!
//! @file uart_usb_lib.h,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file controls the UART USB functions.
//!
//! @version 1.2 at90usb128-demo-cdc-1_0_3 $Id: uart_usb_lib.h,v 1.2 2006/03/17 13:06:22 rletendu Exp $
//!

#ifndef _UART_USB_LIB_H_
#define _UART_USB_LIB_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/
#ifdef UART_USB_DEFAULT_INPUT
#define uart_usb_getchar            _getkey
#define uart_usb_test_hit           test_hit
#endif

#ifdef UART_USB_DEFAULT_OUTPUT
#define uart_usb_putchar            putchar
#endif

/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ D E C L A R A T I O N ______________________________________________*/

void  uart_usb_init(void);
bit   uart_usb_test_hit(void);
char uart_usb_getchar(void);
bit   uart_usb_tx_ready(void);
int  uart_usb_putchar(int);
void  uart_usb_flush(void);

#endif /* _UART_USB_LIB_H_ */
