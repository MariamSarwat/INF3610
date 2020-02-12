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
	int status;

	/**
	* TODO : Initialiser le axi_intc. Retournez une erreur si c'est necessaire.
	*/


	return XST_SUCCESS;
}


void initialize_gpio()
{
	/**
	* TODO : Initialiser le gpio
	*/
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

int connect_gpio_irq()
{
	/**
	* TODO : Connecter le gpio avec son interruption. Retournez une erreur si c'est necessaire.
	*/
}


int connect_fit_timer_1s_irq() {
	int status;

	/**
	* TODO : Connecter le axi_intc a l'interruption du fit_timer qui se provoque a chaque secondes. Retournez une erreur si c'est necessaire.
	*/

	return XST_SUCCESS;
}

int connect_fit_timer_3s_irq() {
	int status;
	/**
	* TODO : Connecter le axi_intc a l'interruption du fit_timer qui se provoque a chaque 3 secondes. Retournez une erreur si c'est necessaire.
	*/
	
	return XST_SUCCESS;
}

void connect_axi() {

	/**
	* TODO : Connectez l'interruption du PL au gic. En d'autre mot, connectez le axi_intc au gic. 
	*/



	XIntc_Start(&axi_intc, XIN_REAL_MODE);


}

void cleanup() {
	/*
	 * Deconnecte et desactive toutes les interruptions.
	 */

	disconnect_intc_irq();
	disconnect_fit_timer_1s_irq();
	disconnect_fit_timer_3s_irq();
}

void disconnect_intc_irq() {
	/**
	* TODO : Desactiver l'interruption du PL
	*/
}

void disconnect_fit_timer_1s_irq() {
	/*
	 * TODO : Desactiver et deconnecter l'interruption du fit_timer qui s'execute a chaque secondes.
	 */

}

void disconnect_fit_timer_3s_irq() {
	/*
	 * TODO : Desactiver et deconnecter l'interruption du fit_timer qui s'execute a chaque 3 secondes.
	 */
}

///////////////////////////////////////////////////////////////////////////
//						End of Interrupt Section
///////////////////////////////////////////////////////////////////////////
