#include "bsp_init.h"

#include <stdio.h>
#include <xparameters.h>
#include <xil_printf.h>
#include <xil_exception.h>

#define TIMER_CLOCK_FREQUENCY 333000000
#define TIMER_TICK_FREQUENCY 100

/**
 * Assign the driver structures for the interrupt controller and
 * PL Core interrupt source and the axi timer
 */

XIntc axi_intc;
XGpioPs gpSwitch;

OS_TCB StartupTaskTCB;
CPU_STK StartupTaskStk[UCOS_START_TASK_STACK_SIZE];

int initialize_axi_intc() {
	int status = XIntc_Initialize(&axi_intc, XPAR_AXI_INTC_0_DEVICE_ID);
	if (status != XST_SUCCESS) return XST_FAILURE;

	return XST_SUCCESS;
}

void initialize_gpio(){
	if (XST_DEVICE_NOT_FOUND == XGpio_Initialize(&gpSwitch, GPIO_SW_DEVICE_ID)) UCOS_Print("Erreur init gpio\n");
	XGpio_InterruptGlobalEnable(&gpSwitch);
	XGpio_InterruptEnable(&gpSwitch, XGPIO_IR_MASK);
}

void init_interrupt() {
	 KAL_ERR kal_err;
	 CPU_INT32U tick_rate;
	 UCOS_IntInit();                                             /* Initialize interrupt controller.                     */

	 tick_rate = OS_CFG_TICK_RATE_HZ;
	 UCOS_TmrTickInit(tick_rate);                                /* Configure and enable OS tick interrupt.              */
	 KAL_Init(DEF_NULL, &kal_err);
	 initialize_gpio(); 										// Initialize gpio
	 initialize_axi_intc(); 									// Initialise le axi_intc
	 UCOS_StdInOutInit();
}

int connect_gpio_irq(){
	int status = XIntc_Connect(&axi_intc, GPIO_SW_IRQ_ID, gpio_isr, &gpSwitch);
	if (status != XST_SUCCESS) return status;
	XIntc_Enable(&axi_intc, GPIO_SW_IRQ_ID);
	return XST_SUCCESS;
}

int connect_fit_timer_1s_irq(){
	int status = XIntc_Connect(&axi_intc, FIT_1S_IRQ_ID, fit_1s_isr, NULL);

	if (status != XST_SUCCESS) return status;

	XIntc_Enable(&axi_intc, FIT_1S_IRQ_ID);
	return XST_SUCCESS;
}

int connect_fit_timer_3s_irq(){
	int status = XIntc_Connect(&axi_intc, FIT_3S_IRQ_ID, fit_3s_isr, NULL);
	if (status != XST_SUCCESS) return status;

	XIntc_Enable(&axi_intc, FIT_3S_IRQ_ID);
	return XST_SUCCESS;
}

void connect_axi(){
	CPU_BOOLEAN succes = UCOS_IntVectSet (PL_INTC_IRQ_ID, 1, 0, (UCOS_INT_FNCT_PTR)XIntc_DeviceInterruptHandler, (void*)(uint32_t)axi_intc.CfgPtr->DeviceId);
	if(succes != DEF_OK) UCOS_Print ("Connect axi : FAILED \n");
	succes = UCOS_IntSrcEn(PL_INTC_IRQ_ID);
	if(succes != DEF_OK) UCOS_Print ("Enable axi : FAILED \n");
	connect_gpio_irq();
	connect_fit_timer_1s_irq();
	connect_fit_timer_3s_irq();

	XIntc_Start(&axi_intc, XIN_REAL_MODE);
}

void cleanup(){
	// Deconnecte et desactive toutes les interruptions
	disconnect_intc_irq();
	disconnect_fit_timer_1s_irq();
	disconnect_fit_timer_3s_irq();
}

void disconnect_intc_irq() {
	XIntc_Disable(&axi_intc, PL_INTC_IRQ_ID);
	XIntc_Disconnect(&axi_intc, PL_INTC_IRQ_ID);
}

void disconnect_fit_timer_1s_irq() {
	XIntc_Disable(&axi_intc, FIT_1S_IRQ_ID);
	XIntc_Disconnect(&axi_intc, FIT_1S_IRQ_ID);
}

void disconnect_fit_timer_3s_irq() {
	XIntc_Disable(&axi_intc, FIT_3S_IRQ_ID);
	XIntc_Disconnect(&axi_intc, FIT_3S_IRQ_ID);
}

///////////////////////////////////////////////////////////////////////////
//						End of Interrupt Section
///////////////////////////////////////////////////////////////////////////
