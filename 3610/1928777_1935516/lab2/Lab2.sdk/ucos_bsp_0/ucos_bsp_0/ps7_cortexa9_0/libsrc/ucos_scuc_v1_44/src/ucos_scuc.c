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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                     CORTEX A9 SNOOP CONTROL UNIT
*
* File : ucos_scuc.c
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
#include  <ucos_scuc.h>


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             SCUC_En()
*
* Description : Enable the snoop control unit.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void SCUC_En (void)
{
    SCUCREG->CTRL |= SCUC_BIT_CTRL_SCUEN;
}


/*
*********************************************************************************************************
*                                           SCUC_InvalidateAll()
*
* Description : Invalidate all for the current core.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void SCUC_InvalidateAll (void)
{
    SCUCREG->INVALL = 0xF << (XPAR_CPU_ID * 4);
}


/*
*********************************************************************************************************
*                                           SCUC_SpeculativeLineFillsEn()
*
* Description : Enable speculative line fills to L2.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void SCUC_SpeculativeLineFillsEn (void)
{
    SCUCREG->CTRL |= SCUC_BIT_CTRL_SPECLFEN;
}


/*
*********************************************************************************************************
*                                           SCUC_StandbyEn()
*
* Description : Enable standby operation.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void SCUC_StandbyEn (void)
{
    SCUCREG->CTRL |= SCUC_BIT_CTRL_STANDBYEN;
}


/*
*********************************************************************************************************
*                                           SCUC_ICStandbyEn()
*
* Description : Enable standby operation.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void SCUC_ICStandbyEn (void)
{
    SCUCREG->CTRL |= SCUC_BIT_CTRL_ICSTANDBYEN;
}


/*
*********************************************************************************************************
*                                           SCUC_DevicePort0En()
*
* Description : For all non-cacheable access to master port 0.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void SCUC_DevicePort0En (void)
{
    SCUCREG->CTRL |= SCUC_BIT_CTRL_FORCEP0EN;
}
