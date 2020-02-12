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
*                                        GIGABIT ETHERNET MAC
*
* File : ucos_emacps.h
*********************************************************************************************************
*/

#ifndef  UCOS_EMACPS_PRESENT
#define  UCOS_EMACPS_PRESENT


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <lib_def.h>
#include <cpu.h>


/*
*********************************************************************************************************
*                                        STRUCTURE DEFINITIONS
*********************************************************************************************************
*/


typedef struct {
    CPU_INT32U  DeviceId;
    CPU_INT32U  BaseAddress;
    CPU_INT32U  ClkFreq;
    CPU_INT32U  IntSource;
} UCOS_EMACPS_Config;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#endif /* UCOS_EMACPS_PRESENT */
