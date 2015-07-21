/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2008
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
 *****************************************************************************/

#ifdef BUILD_LK
	#include <platform/mt_gpio.h>
	#include <platform/mt_pmic.h>
	
	#define Lcd_Log printf
#else
    #include <linux/string.h>
	#include <linux/kernel.h>
	#include <mach/mt_gpio.h>
	#include <mach/mt_pm_ldo.h>
	
	#define Lcd_Log printk
#endif

#include "lcm_drv.h"

// ---------------------------------------------------------------------------
//  Local Constants
// ---------------------------------------------------------------------------

#define FRAME_WIDTH  (480)
#define FRAME_HEIGHT (854)
#define LCM_ID       (0x9806)
#define GPIO_LCD_ID_PIN GPIO_LCM_ID_PIN

#define   __LCM_TE_ON__
// ---------------------------------------------------------------------------
//  Local Variables
// ---------------------------------------------------------------------------

static LCM_UTIL_FUNCS lcm_util = {0};

#define SET_RESET_PIN(v)    (lcm_util.set_reset_pin((v)))

#define UDELAY(n) (lcm_util.udelay(n))
#define MDELAY(n) (lcm_util.mdelay(n))

//static int  VRH1=50,VRH2=50,VCOM=50;



// ---------------------------------------------------------------------------
//  Local Functions
// ---------------------------------------------------------------------------

static __inline void send_ctrl_cmd(unsigned int cmd)
{
	lcm_util.send_cmd(cmd);
}

static __inline void send_data_cmd(unsigned int data)
{
	lcm_util.send_data(data);
}

static __inline unsigned int read_data_cmd(void)
{
    return lcm_util.read_data();
}

static __inline void set_lcm_register(unsigned int regIndex,
		unsigned int regData)
{
	send_ctrl_cmd(regIndex);
	send_data_cmd(regData);
}

static void sw_clear_panel(unsigned int color)
{
	unsigned short x0, y0, x1, y1, x, y;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

	x0 = (unsigned short)0;
	y0 = (unsigned short)0;
	x1 = (unsigned short)FRAME_WIDTH-1;
	y1 = (unsigned short)FRAME_HEIGHT-1;

	h_X_start=((x0&0xFF00)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0xFF00)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0xFF00)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0xFF00)>>8);
	l_Y_end=(y1&0x00FF);

  send_ctrl_cmd(0x2A);
	send_data_cmd(h_X_start); 	
	send_data_cmd(l_X_start); 	
	send_data_cmd(h_X_end); 	
	send_data_cmd(l_X_end); 

	send_ctrl_cmd(0x2B);
	send_data_cmd(h_Y_start); 
	send_data_cmd(l_Y_start); 
	send_data_cmd(h_Y_end); 
	send_data_cmd(l_Y_end); 

//color=0x8410 ;
// color=0xF800 ;
//color=0xFFFF ;
	send_ctrl_cmd(0x29); 

	send_ctrl_cmd(0x2C);
    for (y = y0; y <= y1; ++ y) {
        for (x = x0; x <= x1; ++ x) {
            lcm_util.send_data(color);
        }
    }
}

static unsigned int lcm_compare_id(void)
{
    unsigned int id = 0;

	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);
	
	send_ctrl_cmd(0xD3);
	read_data_cmd();
	read_data_cmd();
  id = read_data_cmd();

#if defined(BUILD_UBOOT)
	printf("mycat READ LCD ID%s, id = 0x%x\n", __func__, id);
#endif

   id = ((id<<8)&0xff00)+read_data_cmd();
   
#if defined(BUILD_UBOOT)
	printf("mycat READ LCD ID%s, id = 0x%x\n", __func__, id);
#endif
	return (LCM_ID == id)?1:0;
	
}

static int get_lcd_id(void)
{
	mt_set_gpio_mode(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_dir(GPIO_LCD_ID_PIN,0);
	mt_set_gpio_pull_enable(GPIO_LCD_ID_PIN,1);
	mt_set_gpio_pull_select(GPIO_LCD_ID_PIN,1);
	MDELAY(1);
	
	return mt_get_gpio_in(GPIO_LCD_ID_PIN);
}


static void init_lcm_registers(void)
{
send_ctrl_cmd(0xFF); // EXTC Command Set enable register 
send_data_cmd(0xFF); 
send_data_cmd(0x98); 
send_data_cmd(0x26); 

send_ctrl_cmd(0xDF);  
send_data_cmd(0x06); 
send_data_cmd(0x22); 
send_data_cmd(0x20); 

send_ctrl_cmd(0xD7);  
send_data_cmd(0x10); 
send_data_cmd(0x06); 
send_data_cmd(0x40); 
send_data_cmd(0x0F); 

send_ctrl_cmd(0xCE);  
send_data_cmd(0xA0); 
send_data_cmd(0x0E); 
send_data_cmd(0x01); 
send_data_cmd(0x02); 

send_ctrl_cmd(0xBC); // GIP 1 
send_data_cmd(0x20); 
send_data_cmd(0x05); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x01); 
send_data_cmd(0x01); 
send_data_cmd(0x88); 
send_data_cmd(0x04); 
send_data_cmd(0x03); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x01); 
send_data_cmd(0x01); 
send_data_cmd(0x55); 
send_data_cmd(0x53);
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x43); 
send_data_cmd(0x0B); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00); 
send_data_cmd(0x00);

send_ctrl_cmd(0xBD); // GIP 2 
send_data_cmd(0x01); 
send_data_cmd(0x23); 
send_data_cmd(0x45); 
send_data_cmd(0x67); 
send_data_cmd(0x01); 
send_data_cmd(0x23); 
send_data_cmd(0x45); 
send_data_cmd(0x67); 

send_ctrl_cmd(0xBE); // GIP 3 
send_data_cmd(0x02); 
send_data_cmd(0x22); 
send_data_cmd(0x11); 
send_data_cmd(0xAA); 
send_data_cmd(0xBB); 
send_data_cmd(0x66); 
send_data_cmd(0x00); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 
send_data_cmd(0x22); 

send_ctrl_cmd(0x3A);  
send_data_cmd(0x66); 

send_ctrl_cmd(0xB1); //FRAME RATE 60.18Hz (TE ON)
send_data_cmd(0x00); 
send_data_cmd(0x58); 
send_data_cmd(0x03);

send_ctrl_cmd(0xB4); //2 dot
send_data_cmd(0x02); 

send_ctrl_cmd(0xC0);  
send_data_cmd(0x08); 

send_ctrl_cmd(0xC1); //AVDD=2xVCI=5.06, AVEE=-2xVCI=-4.82
send_data_cmd(0x15); 
send_data_cmd(0x8A); 
send_data_cmd(0x78);

send_ctrl_cmd(0xC7); //VCOM=-1.337	
send_data_cmd(0x46); 

send_ctrl_cmd(0x35); //TE ON
send_data_cmd(0x00); 

send_ctrl_cmd(0xF7); //480x854
send_data_cmd(0x01); 

send_ctrl_cmd(0XE0); // Positive Gamma Control 
send_data_cmd(0x00); 
send_data_cmd(0x08); 
send_data_cmd(0x11); 
send_data_cmd(0x0C); 
send_data_cmd(0x0E); 
send_data_cmd(0x0B); 
send_data_cmd(0XC7); 
send_data_cmd(0x06); 
send_data_cmd(0x07); 
send_data_cmd(0x0A); 
send_data_cmd(0x10); 
send_data_cmd(0x11); 
send_data_cmd(0x12); 
send_data_cmd(0x15); 
send_data_cmd(0x0B); 
send_data_cmd(0x00); 

send_ctrl_cmd(0XE1); // Negative Gamma Control 
send_data_cmd(0x00); 
send_data_cmd(0x0E); 
send_data_cmd(0x14); 
send_data_cmd(0x0C); 
send_data_cmd(0x0C); 
send_data_cmd(0x0C); 
send_data_cmd(0x76); 
send_data_cmd(0x04); 
send_data_cmd(0x08); 
send_data_cmd(0x0C); 
send_data_cmd(0x0B); 
send_data_cmd(0x0D); 
send_data_cmd(0x07); 
send_data_cmd(0x18); 
send_data_cmd(0x11); 
send_data_cmd(0x00); 

send_ctrl_cmd(0XED); 
send_data_cmd(0x7F);
send_data_cmd(0x0F);

send_ctrl_cmd(0x11);
MDELAY(120);
send_ctrl_cmd(0x29); 


}

// ---------------------------------------------------------------------------
//  LCM Driver Implementations
// ---------------------------------------------------------------------------

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(LCM_UTIL_FUNCS));
}


static void lcm_get_params(LCM_PARAMS *params)
{
    memset(params, 0, sizeof(LCM_PARAMS));

    params->type   = LCM_TYPE_DBI;
    params->ctrl   = LCM_CTRL_PARALLEL_DBI;
    params->width  = FRAME_WIDTH;
    params->height = FRAME_HEIGHT;
    params->io_select_mode = 1; //note:this para is different between 6573 and 6575
    /* serial host interface configurations */
    
    params->dbi.port                    = 0;
    params->dbi.data_width              = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
    params->dbi.data_format.trans_seq   = LCM_DBI_TRANS_SEQ_MSB_FIRST;
    params->dbi.data_format.padding     = LCM_DBI_PADDING_ON_LSB;
    params->dbi.data_format.format      = LCM_DBI_FORMAT_RGB666;
    params->dbi.data_format.width       = LCM_DBI_DATA_WIDTH_18BITS;
    params->dbi.cpu_write_bits          = LCM_DBI_CPU_WRITE_16_BITS;
    params->dbi.io_driving_current      = LCM_DRIVING_CURRENT_6575_8MA;

    params->dbi.parallel.write_setup    = 2;
    params->dbi.parallel.write_hold     = 2;
    params->dbi.parallel.write_wait     = 6;
    params->dbi.parallel.read_setup     = 5;
    params->dbi.parallel.read_hold      = 1;
    params->dbi.parallel.read_latency   = 18;
    params->dbi.parallel.wait_period    = 6;
    params->dbi.parallel.cs_high_width  = 0; //cycles of cs high level between each transfer
    
#ifdef  __LCM_TE_ON__
	// enable tearing-free
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_VSYNC_ONLY;
    params->dbi.te_edge_polarity        = LCM_POLARITY_RISING;	
 #else 
    params->dbi.te_mode                 = LCM_DBI_TE_MODE_DISABLED;  
#endif
}


static void lcm_init(void)
{

	SET_RESET_PIN(1);
	MDELAY(20);
	SET_RESET_PIN(0);
	MDELAY(20);
	SET_RESET_PIN(1);
	MDELAY(100);

	init_lcm_registers();
	
	sw_clear_panel(0x00);
	MDELAY(10);
}


static void lcm_suspend(void)
{
	send_ctrl_cmd(0x28); // Display off
	MDELAY(10);
	send_ctrl_cmd(0x10); // Sleep in
	MDELAY(120);
}


static void lcm_resume(void)
{
#if 0
	send_ctrl_cmd(0x1100);
	MDELAY(120);
	send_ctrl_cmd(0x2900);
	MDELAY(50);
#else
	//add by xia lei jie for LCD INIT
	lcm_init();
#endif
	

}

static void lcm_update(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height)
{
	unsigned short x0, y0, x1, y1;
	unsigned short h_X_start,l_X_start,h_X_end,l_X_end,h_Y_start,l_Y_start,h_Y_end,l_Y_end;

	x0 = (unsigned short)x;
	y0 = (unsigned short)y;
	x1 = (unsigned short)x+width-1;
	y1 = (unsigned short)y+height-1;

	h_X_start=((x0&0xFF00)>>8);
	l_X_start=(x0&0x00FF);
	h_X_end=((x1&0xFF00)>>8);
	l_X_end=(x1&0x00FF);

	h_Y_start=((y0&0xFF00)>>8);
	l_Y_start=(y0&0x00FF);
	h_Y_end=((y1&0xFF00)>>8);
	l_Y_end=(y1&0x00FF);

  send_ctrl_cmd(0x2A);
	send_data_cmd(h_X_start); 	
	send_data_cmd(l_X_start); 
	send_data_cmd(h_X_end); 
	send_data_cmd(l_X_end); 

	send_ctrl_cmd(0x2B);
	send_data_cmd(h_Y_start); 
	send_data_cmd(l_Y_start); 
	send_data_cmd(h_Y_end); 
	send_data_cmd(l_Y_end); 

	send_ctrl_cmd(0x29); 

	send_ctrl_cmd(0x2C);
}


// ---------------------------------------------------------------------------
//  Get LCM Driver Hooks
// ---------------------------------------------------------------------------
LCM_DRIVER ili9806h_HF_14539_1389_A0_BOE_mcu18_fwvga_lcm_drv = 
{
  .name			= "ili9806h_HF_14539_1389_A0_BOE_mcu18_fwvga",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params     = lcm_get_params,
	.init           = lcm_init,
	.suspend        = lcm_suspend,
	.resume         = lcm_resume,
	.update         = lcm_update,
	.compare_id    = lcm_compare_id,
};


