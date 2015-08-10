/*
 * Bluxfox Demo Board Setup
 *
 * Copyright 2005-2012 Centec Networks Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */


#ifndef __ASM_P1020_CTC_H
#define __ASM_P1020_CTC_H

/* board type Defines */
enum glb_board_series_e
{
    GLB_SERIES_GREATBELT_DEMO = 0xc,
    GLB_SERIES_E350 = 0x1,
    GLB_SERIES_E580 = 0x10,
    GLB_SERIES_MAX
};
typedef enum glb_board_series_e glb_board_series_t;

#define GLB_BOARD_GREATBELT_DEMO 0x1

#define GLB_BOARD_E350_48T4X2Q   0x1 
#define GLB_BOARD_E350_48T4XG    0x2 
#define GLB_BOARD_E350_24T4XG    0x4
#define GLB_BOARD_E350_8TS12X    0x7
#define GLB_BOARD_E350_24X       0x8

#define GLB_BOARD_E580_24Q       0x1

struct ctc_board_s
{
    u8 board_series ;           /*board series*/
    u8 board_type ;             /*board type*/
    u8 board_ver ;                 /*board version*/
    u8 nor_type;              /*nor flash type*/
    u8 nand_type;              /*nor flash type*/
    u8 ddr_type;                /*ddr type*/
    u8 asic_chip_num;           /*asic chip number*/
    u8 normal_irq_count;         /*normal irq count*/    
    u8 normal_irq_0;         /*normal irq 0 number*/    
    u8 normal_irq_1;         /*normal irq 1 number*/    
    u8 normal_irq_2;         /*normal irq 2 number*/    
    u8 normal_int0_offset;    /*normal interrupt 0 offset*/    
    u8 normal_int0_mask_offset;    /*normal interrupt 0 mask offset*/    
    u8 normal_int1_offset;    /*normal interrupt 1 offset*/    
    u8 normal_int1_mask_offset;    /*normal interrupt 1 mask offset*/
    u8 normal_int2_offset;    /*normal interrupt 2 offset*/    
    u8 normal_int2_mask_offset;    /*normal interrupt 2 mask offset*/
    u8 watchdog_support;    /*whether support watch dog*/    
    u8 watchdog_en_gpio;    /*enable watch dog gpio*/
    u8 watchdog_feed_gpio;    /*feed watch dog gpio*/
    u8 watchdog_is_feed;      /*whether to feed watchdog, bug18151*/
    u8 gpio_date_offset;     /* gpio date register offset */	
    u8 led_offset;  /* kernel monitor led offset */
    void __iomem *epld_logic_addr;/*epld logic address*/
    void __iomem *gpio_logic_addr;/*gpio logic address*/
};
typedef struct ctc_board_s ctc_board_t;


#endif 
