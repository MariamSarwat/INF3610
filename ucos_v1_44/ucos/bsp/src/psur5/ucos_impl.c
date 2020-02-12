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
*                               Cortex-R5 (psur5) Implementation Specific
*
* File : ucos_impl.c
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <cpu_core.h>
#include  <cpu.h>
#include  <ucos_bsp.h>

#include  <xparameters.h>

void UCOS_LowLevelInit (void)
{
#if 0
#if (UCOS_ZYNQ_CONFIG_CACHES == DEF_ENABLED)
    /* Init L1 Caches. */
    CPU_CortexA9_BranchPredictEn();
#if (UCOS_ZYNQ_ENABLE_OPTIMS == DEF_ENABLED)
    CPU_CortexA9_L1PrefetchEn();
    CPU_CortexA9_L2PrefetchEn();
    CPU_CortexA9_FLZEn();
#endif

#if (UCOS_ZYNQ_ENABLE_CACHES == DEF_ENABLED)
    CPU_CortexA9_L1DCacheEn();
    CPU_CortexA9_L1ICacheEn();
#endif

    /* Init L2 Caches. */
#if (UCOS_AMP_MASTER == DEF_ENABLED)
#if (UCOS_ZYNQ_CONFIG_CACHES == DEF_ENABLED)
#if (UCOS_ZYNQ_ENABLE_OPTIMS == DEF_ENABLED)
    L2CacheC_AuxCtrlSet(0x72060801);
    L2CacheC_PrefetchCtrlSet(0x71000001);
#else
    L2CacheC_AuxCtrlSet(0x02060000);
#endif
    L2CacheC_TagRamLatencySet(1u, 1u, 1u);
    L2CacheC_DataRamLatencySet(1u, 2u, 1u);
#endif

#if (UCOS_ZYNQ_ENABLE_CACHES == DEF_ENABLED)
    L2CacheC_En();
#endif
#endif /* #if (UCOS_ZYNQ_CONFIG_CACHES == DEF_ENABLED) */
#endif /* #if (UCOS_AMP_MASTER == DEF_ENABLED) */
#endif
}
