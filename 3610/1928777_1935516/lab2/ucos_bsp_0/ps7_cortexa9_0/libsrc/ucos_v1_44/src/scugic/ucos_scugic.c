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
*                                CORTEX A GENERIC INTERRUPT CONTROLLER
*
* File : ucos_scugic.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>

#include  <xparameters.h>
#include  <ucos_scugic.h>


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         SCUGIC_PrioMaskSet()
*
* Description : Set CPU's interrupt priority mask.
*
* Argument(s) : prio    	Priority mask.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  SCUGIC_PrioMaskSet (CPU_INT32U prio)
{
    if(prio < 256) {
        CPU_MB();
        SCUGIC_IF_REG->ICCPMR = prio;
        CPU_MB();
    }
}


/*
*********************************************************************************************************
*                                           SCUGIC_PrioSet()
*
* Description : Set interrupt priority.
*
* Argument(s) : int_id  Interrupt id.
*
*               prio    Interrupt priority.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  SCUGIC_PrioSet (CPU_INT32U  int_id,
                      CPU_INT32U  prio)
{
    CPU_INT32U  reg_off;
    CPU_INT32U  reg_byte;
    CPU_INT32U  temp_reg;
    CPU_SR_ALLOC();


    if(prio >= 256) {
        return;
    }

    CPU_CRITICAL_ENTER();

    reg_off = int_id >> 2u;
    reg_byte = int_id & 0x03;

    temp_reg = SCUGIC_DIST_REG->ICDIPRn[reg_off];
    temp_reg = temp_reg & ~(0xFF << (reg_byte * 8u));
    temp_reg = temp_reg | ((prio & 0x1Fu) << (reg_byte * 8u));

    SCUGIC_DIST_REG->ICDIPRn[reg_off] = temp_reg;

    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                           SCUGIC_TargetSet()
*
* Description : Set interrupt target.
*
* Argument(s) : int_id              Interrupt id.
*
*               int_target_list     Interrupt CPU target list.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  SCUGIC_TargetSet (CPU_INT32U  int_id,
                        CPU_INT08U  int_target_list)
{
    CPU_INT32U  reg_off;
    CPU_INT32U  reg_byte;
    CPU_INT32U  temp_reg;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();

    reg_off = int_id >> 2u;
    reg_byte = int_id & 0x03;

    temp_reg = SCUGIC_DIST_REG->ICDIPTRn[reg_off];
    temp_reg = temp_reg & ~(0xFF << (reg_byte * 8u));
    temp_reg = temp_reg | ((int_target_list & 0x1Fu) << (reg_byte * 8u));

    SCUGIC_DIST_REG->ICDIPTRn[reg_off] = temp_reg;

    CPU_CRITICAL_EXIT();
}

/*
*********************************************************************************************************
*                                           SCUGIC_TypeSet()
*
* Description : Set interrupt type.
*
* Argument(s) : int_id              Interrupt id.
*
*               type                0 for level sensitive interrupt 1 for edge.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  SCUGIC_TypeSet (CPU_INT32U        int_id,
                      CPU_BOOLEAN       type)
{
    CPU_INT32U  reg_off;
    CPU_INT32U  reg_byte;
    CPU_INT32U  temp_reg;
    CPU_SR_ALLOC();


    if(type > 1) {
        return;
    }

    CPU_CRITICAL_ENTER();

    reg_off = int_id >> 4u;
    reg_byte = int_id & 0x0F;

    temp_reg = SCUGIC_DIST_REG->ICDICFRn[reg_off];
    temp_reg = temp_reg & ~(0x1 << ((reg_byte * 2u) + 1));
    temp_reg = temp_reg | ((type) << ((reg_byte * 2u) + 1));

    SCUGIC_DIST_REG->ICDICFRn[reg_off] = temp_reg;

    CPU_CRITICAL_EXIT();
}


/*
*********************************************************************************************************
*                                            SCUGIC_SGITrig()
*
* Description : Trigger software generated interrupt.
*
* Argument(s) : int_sgi             SGI ID.
*
*               int_target_list     Interrupt CPU target list
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void  SCUGIC_SGITrig  (CPU_INT32U  int_sgi,
                       CPU_INT32U  int_target_list)
{
    CPU_MB();
    SCUGIC_DIST_REG->ICDSGIR = ((int_target_list & 0xFF) << 16u) | int_sgi;
    CPU_MB();
}

