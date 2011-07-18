//! @file conf_scheduler.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the scheduler configuration definition
//!
//! @version 1.1 at90usb128-demo-cdc-1_0_3 $Id: conf_scheduler.h,v 1.1 2005/10/28 14:37:52 rletendu Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_SCHEDULER_H_
#define _CONF_SCHEDULER_H_



/*--------------- SCHEDULER CONFIGURATION --------------*/
#define SCHEDULER_TYPE          SCHEDULER_FREE  // SCHEDULER_(TIMED|TASK|FREE|CUSTOM)
#define Scheduler_task_1_init   usb_task_init
#define Scheduler_task_1        usb_task
#define Scheduler_task_2_init   cdc_task_init
#define Scheduler_task_2        cdc_task




#endif  //! _CONF_SCHEDULER_H_

