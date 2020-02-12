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
*                                      AXI INTERRUPT CONTROLLER
*
* File: ucos_axiintc.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <xparameters.h>
#include <ucos_axiintc.h>

/*
*********************************************************************************************************
*                                       EXTERN DECLARATIONS
*********************************************************************************************************
*/

extern  UCOS_AXIINTC_Config  UCOS_AXIINTC_ConfigTable[];


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  AXIINTC  UCOS_AXIINTC_Table[XPAR_UCOS_AXIINTC_NUM_INSTANCES];


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            AXIIntCInit()
*
* Description : Initialize an AXI Interrupt Controller.
*
* Argument(s) : device_id  ID of the controller to init.
*
* Return(s)   : Handle to the AXI Interrupt Controller instance or DEF_NULL if
*               the devide id is invalid.
*
* Note(s)     : Should be called before accessing any other functionalities of this peripheral.
*
*********************************************************************************************************
*/

AXIINTC_HANDLE  AXIIntCInit (CPU_INT32U device_id)
{
    AXIINTC_HANDLE Handle;
    AXIINTC_PTR    IntC;
    CPU_SR_ALLOC();


    if (device_id > XPAR_UCOS_AXIINTC_NUM_INSTANCES) {
        return DEF_NULL;
    }

    Handle = (AXIINTC_HANDLE)&UCOS_AXIINTC_Table[device_id];

    Handle->DeviceId = UCOS_AXIINTC_ConfigTable[device_id].DeviceId;
    IntC = (AXIINTC_PTR)UCOS_AXIINTC_ConfigTable[device_id].BaseAddress;
    Handle->AXIIntC = IntC;

    CPU_CRITICAL_ENTER();
    IntC->MER = AXIINTC_BIT_MER_ME | AXIINTC_BIT_MER_HIE;       /* Enable hardware interrupts.                          */
    CPU_CRITICAL_EXIT();

    return (Handle);
}


/*
*********************************************************************************************************
*                                           AXIIntCIntEnSet()
*
* Description : Set a bit in the interrupt enable register.
*
* Argument(s) : handle   Handle to the interrupt controller instance.
*               int_id   ID of the interrupt to set.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

/* TODO - Handle case were the SIE register is not available. */
CPU_BOOLEAN  AXIIntCIntEnSet (AXIINTC_HANDLE handle, CPU_INT32U int_id)
{
    AXIINTC_PTR IntC;


    IntC = handle->AXIIntC;


    IntC->SIE = 1 << int_id;

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           AXIIntCIntEnClr()
*
* Description : Clear a bit in the interrupt enable register.
*
* Argument(s) : handle   Handle to the interrupt controller instance.
*               int_id   ID of the interrupt to clear.
*
* Return(s)   : DEF_OK     Operation successful.
*               DEF_FAIL   Operation failed.
*
* Note(s)     : nonel.
*
*********************************************************************************************************
*/

/* TODO - Handle case where the CIE register is not available. */
CPU_BOOLEAN  AXIIntCIntEnClr (AXIINTC_HANDLE handle, CPU_INT32U int_id)
{
    AXIINTC_PTR IntC;


    IntC = handle->AXIIntC;


    IntC->CIE = 1 << int_id;

    return (DEF_OK);
}
