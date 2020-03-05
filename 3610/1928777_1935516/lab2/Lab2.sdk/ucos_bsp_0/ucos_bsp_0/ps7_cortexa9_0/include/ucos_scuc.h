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
* File : ucos_scuc.h
*********************************************************************************************************
*/

#ifndef  UCOS_SCUC_PRESENT
#define  UCOS_SCUC_PRESENT


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  SCUCREG            ((SCUC_PTR)(XPAR_PS7_SCUC_0_BASEADDR))

/*
*********************************************************************************************************
*                                        REGISTER DEFINITIONS
*********************************************************************************************************
*/


                                                                /* ------------ GIC DISTRIBUTOR INTERFACE ------------- */
typedef  struct  scuc {
    CPU_REG32  CTRL;                                            /* SCU Control Register.                                */
    CPU_REG32  CFG;                                             /* SCU Configuration Register.                          */
    CPU_REG32  PSR;                                             /* SCU Power Status Register.                           */
    CPU_REG32  INVALL;                                          /* SCU Invalidate All Register.                         */
    CPU_REG32  FILERSTART;                                      /* Filtering Start Address Register.                    */
    CPU_REG32  FILTEREND;                                       /* Filtering End Address Register.                      */
    CPU_REG32  SAC;                                             /* SCU Secure Access Control Register.                  */
    CPU_REG32  SNSAC;                                           /* SCU None-Secure Access Control Register.             */
} SCUC, *SCUC_PTR;


                                                                /* --------------- SCU CONTROL REGISTER --------------- */
#define  SCUC_BIT_CTRL_SCUEN            DEF_BIT_00              /* SCU Enable.                                          */
#define  SCUC_BIT_CTRL_FILTEREN         DEF_BIT_01              /* SCU Address Filtering Enable.                        */
#define  SCUC_BIT_CTRL_PARITYEN         DEF_BIT_02              /* SCU Tag RAM Parity Enable.                           */
#define  SCUC_BIT_CTRL_SPECLFEN         DEF_BIT_03              /* SCU Speculative Line Fills Enable.                   */
#define  SCUC_BIT_CTRL_FORCEP0EN        DEF_BIT_04              /* Force All Device to Port 0.                          */
#define  SCUC_BIT_CTRL_STANDBYEN        DEF_BIT_05              /* SCU Standby Enable.                                  */
#define  SCUC_BIT_CTRL_ICSTANDBYEN      DEF_BIT_06              /* IC Standby Enable.                                   */

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/


void SCUC_En (void);

void SCUC_InvalidateAll (void);

void SCUC_SpeculativeLineFillsEn (void);

void SCUC_StandbyEn (void);

void SCUC_ICStandbyEn (void);

void SCUC_DevicePort0En (void);


#endif /* UCOS_SCUC_PRESENT */
