/*
 * CTC Platform P1010 Board Setup
 *
 * Copyright (C) 2005 Centec Networks Inc.  All rights reserved.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/of_platform.h>

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <mm/mmu_decl.h>
#include <asm/prom.h>
#include <asm/udbg.h>
#include <asm/mpic.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>
#include "mpc85xx.h"
#include <asm/ctc_platform.h>

void __iomem *ctc_epld_logic_addr;
void __iomem *gpio_logic_addr;
ctc_board_t ctc_bd_info;
EXPORT_SYMBOL(ctc_bd_info);

void __init p1010_ctc_pic_init(void)
{
    struct device_node *np;
    int ret;
	struct mpic *mpic = mpic_alloc(NULL, 0, MPIC_BIG_ENDIAN |
	  MPIC_SINGLE_DEST_CPU,
	  0, 256, " OpenPIC  ");

	BUG_ON(mpic == NULL);

	mpic_init(mpic);

    for_each_node_by_type(np, "irq0") {         
		if (of_device_is_compatible(np, "fsl,p1010-irq")) {
            ret = irq_of_parse_and_map(np, 0); 
            if(ret == NO_IRQ)
            {
                printk("request irq 0 fail!\n");
            }
		}
	} 

    for_each_node_by_type(np, "irq1") {         
		if (of_device_is_compatible(np, "fsl,p1010-irq")) {
            ret = irq_of_parse_and_map(np, 0);   
            if(ret == NO_IRQ)
            {
                printk("request irq 1 fail!\n");
            }
		}
	} 

    for_each_node_by_type(np, "irq2") {         
		if (of_device_is_compatible(np, "fsl,p1010-irq")) {
            ret = irq_of_parse_and_map(np, 0); 
            if(ret == NO_IRQ)
            {
                printk("request irq 2 fail!\n");
            }
		}
	} 

    for_each_node_by_type(np, "irq3") {         
		if (of_device_is_compatible(np, "fsl,p1010-irq")) {
            ret = irq_of_parse_and_map(np, 0); 
            if(ret == NO_IRQ)
            {
                printk("request irq 3 fail!\n");
            }
		}
	} 
}

static void ctc_sys_led_ctl(void)
{
    u8 led_ctl = 0;
    
    ctc_bd_info.led_offset = 0x5;
    /* system led, amber blinkingSlow 0.5Hz*/
    led_ctl = 0x09;
    iowrite8(led_ctl, ctc_epld_logic_addr + ctc_bd_info.led_offset);
}

static void ctc_gg_rst_ctl(void)
{        
    out_8((ctc_epld_logic_addr + 0x10),0x3);
    mdelay(100);
    
    out_8((ctc_epld_logic_addr + 0x4),0x0);
    mdelay(10);
    out_8((ctc_epld_logic_addr + 0x4),0x8);
    mdelay(100);
    out_8((ctc_epld_logic_addr + 0x4),0xc);
    mdelay(100);
    out_8((ctc_epld_logic_addr + 0x4),0xf);
    mdelay(100);    
}


/*
 * Setup the architecture
 */
static void __init p1010_ctc_setup_arch(void)
{   
	if (ppc_md.progress)
		ppc_md.progress("p1010_ctc_setup_arch()", 0);
	
          
    ctc_epld_logic_addr = ioremap(0xffb00000, 0x1000) ;

    if (ctc_epld_logic_addr == NULL)
    {
        printk(KERN_ERR "#########Can't map epld space!##########\n");              
    }
    else 
    {          
        ctc_bd_info.epld_logic_addr = ctc_epld_logic_addr;
    }

    gpio_logic_addr = ioremap(0xffe0f000, 0x1000) ;

    if (gpio_logic_addr == NULL)
    {
        printk(KERN_ERR "#########Can't map gpio space!##########\n");              
    }
    else 
    {          
        ctc_bd_info.gpio_logic_addr = gpio_logic_addr;
    }
      
    ctc_gg_rst_ctl();
    
    ctc_sys_led_ctl();

    fsl_pci_assign_primary();
    
	printk(KERN_INFO "P1010 CTC board from Centec Networks\n");
}

machine_arch_initcall(p1010_ctc, mpc85xx_common_publish_devices);
machine_arch_initcall(p1010_ctc, swiotlb_setup_bus_notifier);

/*
 * Called very early, device-tree isn't unflattened
 */
static int __init p1010_ctc_probe(void)
{
	unsigned long root = of_get_flat_dt_root();

	if (of_flat_dt_is_compatible(root, "centec,e580"))
		return 1;
	return 0;
}

static void p1010_ctc_restart(char *cmd)
{
    u8 reset_ctl; 
    u32 val; 
    
    local_irq_disable();

    /*for reboot info*/
    reset_ctl = 0xaa;
    iowrite8(reset_ctl, ctc_bd_info.epld_logic_addr+ 0xf7);

    /*power down the whole board*/
    out_8(ctc_bd_info.epld_logic_addr+0x23 , 0);
    out_8(ctc_bd_info.epld_logic_addr+0x23 , 3);
    
    mdelay(3000);

    /* Added by liuht for bug 34125, 2015-06-25 */
    /* enable watchdog but not feed */
    out_be32(ctc_bd_info.gpio_logic_addr , 0x00300000);
    val = in_be32(ctc_bd_info.gpio_logic_addr + 8);
    udelay(100);
    val &= 0xffefffff; /* GPIO 11 low */
    out_be32(ctc_bd_info.gpio_logic_addr + 8, val);
    udelay(100);
    val |= 0x00200000; /* GPIO 10 high */
    out_be32(ctc_bd_info.gpio_logic_addr + 8, val);
}

define_machine(p1010_ctc) {
	.name			= "P1010 CTC",
	.probe			= p1010_ctc_probe,
	.setup_arch		= p1010_ctc_setup_arch,
	.init_IRQ		= p1010_ctc_pic_init,
#ifdef CONFIG_PCI
	.pcibios_fixup_bus	= fsl_pcibios_fixup_bus,
#endif
	.get_irq		= mpic_get_irq,
	.restart		= p1010_ctc_restart,
	.calibrate_decr		= generic_calibrate_decr,
};
