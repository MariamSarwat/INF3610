#include <app_cfg.h>
#include <cpu.h>
#include <ucos_bsp.h>
#include <ucos_int.h>
#include <xparameters.h>
#include <KAL/kal.h>
#include <Source/os.h>
#include <os_cfg_app.h>
#include <xgpiops.h>
#include <xintc.h>
#include <xil_exception.h>

#define GIC_DEVICE_ID	        XPAR_PS7_SCUGIC_0_DEVICE_ID
#define PL_INTC_IRQ_ID          XPS_IRQ_INT_ID
#define FIT_1S_IRQ_ID           XPAR_AXI_INTC_0_FIT_TIMER_0_INTERRUPT_INTR
#define FIT_3S_IRQ_ID			XPAR_AXI_INTC_0_FIT_TIMER_1_INTERRUPT_INTR
#define GPIO_SW_IRQ_ID			XPAR_AXI_INTC_0_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define GPIO_SW_DEVICE_ID		XPAR_AXI_GPIO_0_DEVICE_ID

#define XGPIO_IR_MASK		0x3 /**< Mask of all bits */

extern XGpioPs gpSwitch;    // GPIO Device
extern XIntc axi_intc;		// Axi_intc device

/**
 * Initialisation des interruptions
 */

int initialize_axi_intc();
void initialize_gpio();
void init_interrupt();

/**
 * Connection des IRQ
 */

int connect_gpio_irq();
int connect_fit_timer_1s_irq();
int connect_fit_timer_3s_irq();
void connect_axi();

/**
 * Disconnection IRQ
 */

void cleanup();
void disconnect_intc_irq();
void disconnect_fit_timer_1s_irq();
void disconnect_fit_timer_3s_irq();

/**
 * TODO : Handler prototypes
 */
void fit_1s_isr(void* not_valid);
void fit_3s_isr(void* not_valid);
void gpio_isr(void* data);
