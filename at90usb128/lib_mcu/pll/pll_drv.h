//! @file pll_drv.h
//! pll_drv.h,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the low level macros and definition for the USB PLL
//!
//! @version 1.7 at90usb128-demo-cdc-1_0_3
//!
//! @todo


#ifndef PLL_DRV_H
#define PLL_DRV_H

//_____ I N C L U D E S ____________________________________________________

//! @defgroup PLL PLL driver Module
//! PLL Module
//! @{
//_____ M A C R O S ________________________________________________________

   //! @defgroup PLL_macros PLL Macros
   //! These functions allow to control the PLL
   //! @{
#define PLLx24 			( (0<<PLLP2) | (0<<PLLP1) | (0<<PLLP0) )
#define PLLx12 			( (0<<PLLP2) | (0<<PLLP1) | (1<<PLLP0) )
#define PLLx08 			( (0<<PLLP2) | (1<<PLLP1) | (0<<PLLP0) )
#define PLLx06 			( (0<<PLLP2) | (1<<PLLP1) | (1<<PLLP0) )
#define PLLx04 			( (1<<PLLP2) | (0<<PLLP1) | (0<<PLLP0) )
#define PLLx03 			( (1<<PLLP2) | (0<<PLLP1) | (1<<PLLP0) )
#define PLLx04_8        ( (1<<PLLP2) | (1<<PLLP1) | (0<<PLLP0) )
#define PLLx02          ( (1<<PLLP2) | (1<<PLLP1) | (1<<PLLP0) )


      //! @brief Start the PLL at only 48 MHz, regarding CPU frequency
      //! Start the USB PLL with clockfactor
      //! clockfactor can be PLLx24, PLLx12, PLLx08
      //! PLLx06, PLLx04, PLLx03
#define Start_pll(clockfactor)                                                  \
           (PLLCSR = ( clockfactor  | (1<<PLLE)  ))

      //! return 1 when PLL locked
#define Is_pll_ready()       (PLLCSR & (1<<PLOCK) )

      //! Test PLL lock bit and wait until lock is set
#define Wait_pll_ready()     while (!(PLLCSR & (1<<PLOCK)))

      //! Stop the PLL
#define Stop_pll()           (PLLCSR  &= (~(1<<PLLE)) ) 

      // Start the PLL in autofactor mode
      // regarding FOSC define
#if   (FOSC==2000)
      //! Start the PLL in autofactor mode
      //! regarding FOSC define
   #define Pll_start_auto()   Start_pll(PLLx24)
#elif (FOSC==4000)
   #define Pll_start_auto()   Start_pll(PLLx12)
#elif (FOSC==6000)
   #define Pll_start_auto()   Start_pll(PLLx08)
#elif (FOSC==8000)
      //! Start the PLL in autofactor mode
      //! regarding FOSC define
   #define Pll_start_auto()   Start_pll(PLLx06)
#elif (FOSC==12000)
   #define Pll_start_auto()   Start_pll(PLLx04)
#elif (FOSC==16000)
   #define Pll_start_auto()   Start_pll(PLLx03)
#elif (FOSC==20000)
   #define Pll_start_auto()   Start_pll(PLLx04_8)
#elif (FOSC==24000)
   #define Pll_start_auto()   Start_pll(PLLx02)
#else
   #error   "FOSC should be defined in config.h"
#endif

   //! @}

//! @}
#endif  // PLL_DRV_H


