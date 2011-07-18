/*H**************************************************************************
* NAME:         usb_specific_request.h
*----------------------------------------------------------------------------
* Copyright (c) 2006 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      at90usb128-demo-cdc-1_0_3
* REVISION:     1.1
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the user call-back functions corresponding to the
* application:
* CDC DEVICE
*****************************************************************************/

#ifndef _USB_USER_ENUM_H_
#define _USB_USER_ENUM_H_

/*_____ I N C L U D E S ____________________________________________________*/

#include "config.h"

/*_____ M A C R O S ________________________________________________________*/


#define GET_LINE_CODING           0x21
#define SET_LINE_CODING           0x20
#define SET_CONTROL_LINE_STATE    0x22
#define SEND_BREAK                0x23
#define SEND_ENCAPSULATED_COMMAND 0x00
#define GET_ENCAPSULATED_COMMAND  0x01

extern  code S_usb_device_descriptor usb_dev_desc;
extern  code S_usb_user_configuration_descriptor usb_conf_desc;
extern  code S_usb_user_configuration_descriptor usb_other_conf_desc;
extern  code S_usb_device_qualifier_descriptor usb_qual_desc;
extern  code S_usb_manufacturer_string_descriptor usb_user_manufacturer_string_descriptor;
extern  code S_usb_product_string_descriptor usb_user_product_string_descriptor;
extern  code S_usb_serial_number usb_user_serial_number;
extern  code S_usb_language_id usb_user_language_id;



/*_____ D E F I N I T I O N ________________________________________________*/
Bool  usb_user_read_request(U8, U8);
Bool  usb_user_get_descriptor(U8 , U8);
void  usb_user_endpoint_init(U8);
void  cdc_get_line_coding();
void  cdc_set_line_coding();
void 	cdc_set_control_line_state (void);

// ____ T Y P E  D E F I N I T I O N _______________________________________

typedef struct
{
	U32 dwDTERate;
	U8 bCharFormat;
	U8 bParityType;
	U8 bDataBits;
}S_line_coding;

#endif

