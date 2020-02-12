/******************************************************************************
*
* Copyright (C) 2010 - 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include <Source/os.h>
#include <app_cfg.h>

#include "xparameters.h"
#include "ucos_int.h"

#include "platform_config.h"
#include "metal/io.h"
#include "metal/device.h"
#include "metal/sys.h"

#define INTC_DEVICE_ID		XPAR_SCUGIC_0_DEVICE_ID

#define IPI_IRQ_VECT_ID         65

const metal_phys_addr_t metal_phys[] = {
	0xFF310000, /**< base IPI address */
	0x3ED00000, /**< shared memory0 description base address */
	0x3ED10000, /**< shared memory0 description base address */
	0x3ED20000, /**< shared memory base address */
};

struct metal_device metal_dev_table[] = {
	{
		/* IPI device */
		"ff310000.ipi",
		NULL,
		1,
		{
			{
				(void *)0xFF310000,
				&metal_phys[0],
				0x1000,
				(sizeof(metal_phys_addr_t) << 3),
				(unsigned long)(-1),
				METAL_UNCACHED,
				{NULL},
			}
		},
		{NULL},
		1,
		(void *)IPI_IRQ_VECT_ID,

	},
	{
		/* Shared memory0 management device */
		"3ed00000.shm_desc",
		NULL,
		1,
		{
			{
				(void *)0x3ED00000,
				&metal_phys[1],
				0x1000,
				(sizeof(metal_phys_addr_t) << 3),
				(unsigned long)(-1),
				METAL_UNCACHED | METAL_SHARED_MEM,
				{NULL},
			}
		},
		{NULL},
		0,
		NULL,

	},
	{
		/* Shared memory1 management device */
		"3ed10000.shm_desc",
		NULL,
		1,
		{
			{
				(void *)0x3ED10000,
				&metal_phys[2],
				0x1000,
				(sizeof(metal_phys_addr_t) << 3),
				(unsigned long)(-1),
				METAL_UNCACHED | METAL_SHARED_MEM,
				{NULL},
			}
		},
		{NULL},
		0,
		NULL,

	},
	{
		/* Shared memory management device */
		"3ed20000.shm",
		NULL,
		1,
		{
			{
				(void *)0x3ED20000,
				&metal_phys[3],
				0x100000,
				(sizeof(metal_phys_addr_t) << 3),
				(unsigned long)(-1),
				METAL_UNCACHED | METAL_SHARED_MEM,
				{NULL},
			}
		},
		{NULL},
		0,
		NULL,

	},
};

extern void metal_irq_isr(int irq);
static void ucos_metal_irq_isr(void *p_arg, CPU_INT32U src);


/**
 * @brief init_irq() - Initialize GIC and connect IPI interrupt
 *        This function will initialize the GIC and connect the IPI
 *        interrupt.    UCOS_IntIPIInit();
 *
 *
 * @return 0 - succeeded, non-0 for failures
 */
int init_irq()
{
    UCOS_IntIPIInit();
    UCOS_IntIPIEn(9);

    /* Install the libmetal ISR as the IPI vector */
    UCOS_IntVectSet (IPI_IRQ_VECT_ID, 0, 0, ucos_metal_irq_isr, (void *)0);
	UCOS_IntSrcEn(IPI_IRQ_VECT_ID);

	return 0;
}

/**
 * @brief ucos_metal_irq_isr() - Wrapper for metal_irq_isr()
 *        to work with the uC/OS BSP.
 */
static void ucos_metal_irq_isr(void *p_arg, CPU_INT32U src)
{
    (void)p_arg;
    (void)src;
    metal_irq_isr(IPI_IRQ_VECT_ID);
}

/**
 * @brief platform_register_metal_device() - Register libmetal devices.
 *        This function register the libmetal generic bus, and then
 *        register the IPI, shared memory descriptor and shared memory
 *        devices to the libmetal generic bus.
 *
 * @return 0 - succeeded, non-zero for failures.
 */
int platform_register_metal_device (void)
{
	unsigned int i;
	int ret;
	struct metal_device *dev;
	metal_bus_register(&metal_generic_bus);
	for (i = 0; i < sizeof(metal_dev_table)/sizeof(struct metal_device);
	     i++) {
		dev = &metal_dev_table[i];
		UCOS_Printf("registering: %d, name=%s\n", i, dev->name);
		ret = metal_register_generic_device(dev);
		if (ret)
			return ret;
	}
	return 0;
}

/**
 * @brief sys_init() - Register libmetal devices.
 *        This function register the libmetal generic bus, and then
 *        register the IPI, shared memory descriptor and shared memory
 *        devices to the libmetal generic bus.
 *
 * @return 0 - succeeded, non-zero for failures.
 */
int sys_init()
{
	struct metal_init_params metal_param = METAL_INIT_DEFAULTS;
	int ret;

	if (init_irq()) {
	    UCOS_Printf("Failed to initialize interrupt\n");
	}
	/** Register the device */
	metal_init(&metal_param);
	ret = platform_register_metal_device();
	if (ret) {
		UCOS_Printf(
			"%s: failed to register device: %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

void wait_for_interrupt(void) {
    asm volatile("wfi");
}
