//! @file usb_key.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level macros and definition for USB KEY board
//!
//! @version 1.6 at90usb128-demo-cdc-1_0_3
//!
//! @todo
//! @bug

#ifndef USB_KEY_H
#define USB_KEY_H

//_____ I N C L U D E S ____________________________________________________


#include "config.h"


//_____ M A C R O S ________________________________________________________


//! @defgroup USB_KEY_module USBKEY Module
//! This module contains low level hardware abstraction layer for AT90USBkey board
//! @image html avrusbkey.gif
//! @{

      //! @defgroup USB_key_leds Leds Management
      //! Macros to manage Leds on USB KEY
      //! @{
#define Leds_init()     (DDRD |= 0xF0)
#define Leds_on()       (PORTD |= 0xF0)   // Orange
#define Leds_off()      (PORTD &= 0x0F)
#define Led0_on()       (PORTD |= 0x10)   // Red
#define Led1_on()       (PORTD |= 0x20)   // Green
#define Led2_on()       (PORTD |= 0x40)   // Green
#define Led3_on()       (PORTD |= 0x80)   // Red
#define Led0_off()      (PORTD &= 0xEF)
#define Led1_off()      (PORTD &= 0xDF)
#define Led2_off()      (PORTD &= 0xBF)
#define Led3_off()      (PORTD &= 0x7F)
#define Led0_toggle()   (PIND |= 0x10)
#define Led1_toggle()   (PIND |= 0x20)
#define Led2_toggle()   (PIND |= 0x40)
#define Led3_toggle()   (PIND |= 0x80)
#define Leds_set_val(c) (Leds_off(),PORTD |= (c<<4)&0xF0)
#define Leds_get_val()  (PORTD>>4)
#define Is_led0_on()    (PIND & 0x10 ? TRUE : FALSE)
      //! @}

      //! @defgroup USB_key_joy Joystick Management
      //! Macros to manage Joystick on USB KEY
      //! @{
#define Joy_init()            (DDRB &= 0x1F, PORTB |= 0xE0, DDRE &= 0xE7, PORTE |= 0x30)
#define Is_joy_up()           ((PINB & 0x80) ?  FALSE : TRUE)
#define Is_joy_not_up()       ((PINB & 0x80) ?  TRUE  : FALSE)
#define Is_joy_left()         ((PINB & 0x40) ?  FALSE : TRUE)
#define Is_joy_not_left()     ((PINB & 0x40) ?  TRUE : FALSE)
#define Is_joy_select()       ((PINB & 0x20) ?  FALSE : TRUE)
#define Is_joy_not_select()   ((PINB & 0x20) ?  TRUE : FALSE)
#define Is_joy_right()        ((PINE & 0x10) ?  FALSE : TRUE)
#define Is_joy_not_right()    ((PINE & 0x10) ?  TRUE : FALSE)
#define Is_joy_down()         ((PINE & 0x20) ?  FALSE : TRUE)
#define Is_joy_not_down()     ((PINE & 0x20) ?  TRUE : FALSE)
      //! @}

      //! @defgroup USB_key_HWB HWB button management
      //! HWB button is connected to PE2 and can also
      //! be used as generic push button
      //! @{
#define Hwb_button_init()      (DDRE &= 0xFB, PORTE |= 0x04)
#define Is_hwb()               ((PINE & 0x04) ?  FALSE : TRUE)
#define Is_not_hwb()           ((PINE & 0x04) ?  TRUE : FALSE)
      //! @}

// USB KEY ADC Channel Definition
#define ADC_TEMP_CH  0x00

#ifdef USE_ADC       //!< this define is set in config.h file



//! @brief This function performs an ADC conversion from the USB KEY TEMP channel
//! an returns the 10 bits result in an U16 value.
//!
//! @warning USE_ADC should be defined in config.h
//!
//! @param none
//!
//! @return U16 analog sensor temperature value.
//!
   U16 Get_adc_temp_val(void);


//! @brief This function performs an ADC conversion from the USB KEY TEMP channel
//! an returns the 10 bits result of the temperature (in °C) in an S16 value.
//!
//! @warning USE_ADC should be defined in config.h
//!
//! @param none
//!
//! @return S16 temperature value in °C.
//!
   S16  Read_temperature(void);

#endif  //USE_ADC

      //! @defgroup USB_key_df USB KEY Hardware data flash configuration
      //! Macros to init the environnement for DF on USB KEY
      //! @{
//! Type of DF implemented on USB KEY.
#ifdef USBKEY_HAS_321_DF
   #define DF_4_MB   // First prototypes version have AT90DB321C memories
#else
   #define DF_8_MB   // AT45DB642D
#endif

#define DF_SELECT_MEM        0
#define DF_UNSELECT_MEM      1

#define DF_NB_MEM          2


#ifndef AVRGCC
   // PORTB_Bit0=1,DDRB_Bit0=0: SPI Slave Select Input. The data direction of the
   //    NOT(SS) is INPUT and the pull-up resistor is activated.
   // DDRB_Bit1=1: the SCK signal direction is OUTPUT.
   // DDRB_Bit2=1: the SI signal direction is OUTPUT.
   // DDRB_Bit3=0: the SO signal direction is INPUT.
   // PORTE_Bit0=1,DDRE_Bit0=1,PORTE_Bit1=1,DDRE_Bit1=1: both CS are driven high
   //    and both CS are OUTPUT.
   #define Init_df_usb_key()  (PORTB_Bit0=1,DDRB_Bit0=0,DDRB_Bit1=1,DDRB_Bit2=1,\
                              DDRB_Bit3=0,DDRE_Bit0=1,DDRE_Bit1=1,Df_desel_all())
   #define DF_CS0             PORTE_Bit0
   #define DF_CS1             PORTE_Bit1
   #define DF_CS2             DF_CS0
   #define DF_CS3             DF_CS0
   #define Df_select_df(i, val) (DF_CS##i = val)

   #define Df_desel_all()     (DF_CS0=DF_UNSELECT_MEM,DF_CS1=DF_UNSELECT_MEM) // set CS# dataflash memories (unselect).
#else //! for AVRGCC
   #define Init_df_usb_key()  (PORTB|=0x01,DDRB &= 0xFE,DDRB|=0x01,DDRB|=0x04,\
                              DDRB|=0x04,DDRE|=0x01,DDRE|=0x02,Df_desel_all())
   #define DF_CS_PORT         PORTE
   #define DF_CS0             0
   #define DF_CS1             1
   #define DF_CS2             DF_CS0
   #define DF_CS3             DF_CS0

   #define Df_desel_all()     (DF_CS_PORT|=0x03) // set CS# dataflash memories (unselect).   

#endif

#define df_init_spi()      Init_df_usb_key()
      //! @}

//! @}

#endif  // USB_KEY_H
