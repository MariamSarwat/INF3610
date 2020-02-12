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
*                              AXI INTERRUPT CONTROLLER FOR THE MICROBLAZE
*
* File : ucos_int_impl.h
*********************************************************************************************************
*/

#ifndef  UCOS_INT_PRESENT
#define  UCOS_INT_PRESENT

#include  <lib_def.h>
#include  <cpu.h>


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

typedef  void  (*UCOS_INT_FNCT_PTR)(void *, CPU_INT32U);


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN UCOS_IntInit    (void);

CPU_BOOLEAN UCOS_IntSrcEn   (CPU_INT32U         int_id);

CPU_BOOLEAN UCOS_IntSrcDis  (CPU_INT32U         int_id);

CPU_BOOLEAN UCOS_IntVectSet (CPU_INT32U         int_id,
                             UCOS_INT_FNCT_PTR  int_fnct,
                             void              *p_int_arg);

void        BSP_IntHandler  (void);


#endif /* UCOS_INT_PRESENT */
