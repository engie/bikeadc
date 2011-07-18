//! @file power_drv.h,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the Power Management low level driver definition
//! This module allows to configure the different power mode of the AVR core and
//! also to setup the the internal clock prescaler
//!
//! @version 1.5 at90usb128-demo-cdc-1_0_3 $Id: power_drv.h,v 1.5 2006/03/10 15:21:22 rletendu Exp $
//!
//! @todo
//! @bug


#ifndef _POWER_DRV_H_
#define _POWER_DRV_H_

//! @defgroup powermode Power management drivers module
//!
//! @{
   
//_____ M A C R O S ________________________________________________________

#define Setup_idle_mode()			               (SMCR=0,SMCR |= (1<<SE))
#define Setup_power_down_mode() 	               (SMCR=0,SMCR |= (1<<SE)+(1<<SM1))
#define Setup_adc_noise_reduction_mode()			(SMCR=0,SMCR |= (1<<SE)+(1<<SM0))
#define Setup_power_save_mode()	               (SMCR=0,SMCR |= (1<<SE)+(1<<SM1)+(1<<SM0))
#define Setup_standby_mode()		               (SMCR=0,SMCR |= (1<<SE)+(1<<SM2)+(1<<SM1))
#define Setup_ext_standby_mode()	               (SMCR=0,SMCR |= (1<<SE)+(1<<SM2)+(1<<SM1)+(1<<SM0))

//! Clear_prescaler.
//!
//! This function reset the internal CPU core clock prescaler
//!
//!
//! @param none
//!
//! @return none.
//!
#define Clear_prescaler()                       (CLKPR = (1<<CLKPCE),CLKPR = 0)

//! Set_prescaler.
//!
//! This function configure the internal CPU core clock prescaler value
//!
//!
//! @param x: prescaler new value
//!
//! @return none.
//!
#define Set_prescaler(x)                        (CLKPR = (1<<CKLPCE),CLKPR = x)



#define Sleep_instruction()		{asm("SLEEP");}

//Backward compatibility
#define Set_power_down_mode()		set_power_down_mode()
#define Set_idle_mode()				set_idle_mode()

//_____ D E C L A R A T I O N ______________________________________________

void set_idle_mode(void);
void set_power_down_mode(void);
void set_adc_noise_reduction_mode(void);
void set_power_save_mode(void);
void set_standby_mode(void);
void set_ext_standby_mode(void);

//! Enter_idle_mode.
//!
//! This function makes the AVR core enter idle mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_idle_mode()                 (set_idle_mode())

//! Enter_power_down_mode.
//!
//! This function makes the AVR core enter power down mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_power_down_mode()           (set_power_down_mode())

//! Enter_adc_noise_reduction_mode.
//!
//! This function makes the AVR core enter adc noise reduction mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_adc_noise_reduction_mode()  (set_adc_noise_reduction_mode())

//! Enter_power_save_mode.
//!
//! This function makes the AVR core enter power save mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_power_save_mode()           (set_power_save_mode())

//! Enter_standby_mode.
//!
//! This function makes the AVR core enter standby mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_standby_mode()              (set_standby_mode())

//! Enter_ext_standby_mode.
//!
//! This function makes the AVR core enter extended standby mode.
//!
//! @param none
//!
//! @return none.
//!
#define Enter_ext_standby_mode()          (set_ext_standby_mode())


//! @}

#endif  // _POWER_DRV_H_

