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
*                                    Cortex A9 Low Level Functions
*
* File : ucos_cpu_cortexa9.h
*********************************************************************************************************
*/

#ifndef  UCOS_CPU_CORTEXA9_PRESENT
#define  UCOS_CPU_CORTEXA9_PRESENT

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void CPU_CortexA9_L1DCacheEn (void);

void CPU_CortexA9_L1ICacheEn (void);

void CPU_CortexA9_BranchPredictEn (void);

void CPU_CortexA9_L1PrefetchEn (void);

void CPU_CortexA9_L2PrefetchEn (void);

void CPU_CortexA9_FLZEn (void);

#endif /* UCOS_CPU_CORTEXA9_PRESENT */
