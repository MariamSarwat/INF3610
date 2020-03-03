/*
*********************************************************************************************************
*                                             uCOS XSDK BSP                               
*                                                                                         
*                      (c) Copyright 2014-2019; Silicon Laboratories Inc.,                
*                             400 W. Cesar Chavez, Austin, TX 78701                       
*                                                                                         
*                All rights reserved. Protected by international copyright laws.          
*                                                                                         
*               Your use of this software is subject to your acceptance of the terms      
*               of a Silicon Labs Micrium software license, which can be obtained by      
*               contacting info@micrium.com. If you do not agree to the terms of this     
*               license, you may not use this software.                                   
*                                                                                         
*               Please help us continue to provide the Embedded community with the finest 
*               software available. Your honesty is greatly appreciated.                  
*                                                                                         
*                 You can find our product's documentation at: doc.micrium.com            
*                                                                                         
*                       For more information visit us at: www.micrium.com                 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                                SYSTICK
*
* File : ucos_osiii_bsp.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <lib_def.h>
#include  <cpu.h>

#include  "ucos_osiii_bsp.h"
#include  <ucos_int.h>
#include  <os.h>
#include  <os_cfg_app.h>


typedef struct ttcps {
    CPU_REG32  ClkCtrl;
    CPU_REG32  RESERVED0[2];
    CPU_REG32  CtrCtrl;
    CPU_REG32  RESERVED1[2];
    CPU_REG32  CtrVal;
    CPU_REG32  RESERVED2[2];
    CPU_REG32  IntervalCtrVal;
    CPU_REG32  RESERVED3[2];
    CPU_REG32  Match1CtrVal;
    CPU_REG32  RESERVED4[2];
    CPU_REG32  Match2CtrVal;
    CPU_REG32  RESERVED5[2];
    CPU_REG32  Match3CtrVal;
    CPU_REG32  RESERVED6[2];
    CPU_REG32  IntStat;
    CPU_REG32  RESERVED7[2];
    CPU_REG32  IntEn;
    CPU_REG32  RESERVED8[2];
    CPU_REG32  EventCtrl;
    CPU_REG32  RESERVED9[2];
    CPU_REG32  EventReg;
    CPU_REG32  RESERVED10[2];
} TTCPS, *TTCPS_PTR;

#define  TTC_TIMER_TICK           ((TTCPS_PTR)(0xFF110000))


/*
*********************************************************************************************************
*                                       BSP_OS_TmrTickHandler()
*
* Description : Interrupt handler for the tick timer
*
* Argument(s) : cpu_id     Source core id
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  UCOS_TmrTickHandler(void *p_int_arg, CPU_INT32U cpu_id)
{
    (void)p_int_arg;
    (void)cpu_id;

    TTC_TIMER_TICK->IntStat;
    OSTimeTick();
}


/*
 *********************************************************************************************************
 *                                            BSP_OS_TmrTickInit()
 *
 * Description : Initialize uC/OS-III's tick source
 *
 * Argument(s) : ticks_per_sec              Number of ticks per second.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : Application.
 *
 * Note(s)     : none.
 *********************************************************************************************************
 */

void UCOS_TmrTickInit (CPU_INT32U tick_rate)
{

    UCOS_IntVectSet (68u,
                     0u,
                     0u,
                     UCOS_TmrTickHandler,
                     DEF_NULL);

    UCOS_IntSrcEn (68u);

    TTC_TIMER_TICK->IntervalCtrVal = 100000000 / tick_rate;
    TTC_TIMER_TICK->IntEn |= DEF_BIT_00;
    TTC_TIMER_TICK->CtrCtrl |= DEF_BIT_01;
    TTC_TIMER_TICK->CtrCtrl &= ~DEF_BIT_00;
}

