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
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   sensor.c
 *
 * Project:
 * --------
 *  
 *
 * Description:
 * ------------
 *   Source code of Sensor driver
 *
 *
 * Author:
 * -------
 *   Leo Lee
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by CC/CQ. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:$
 * $Modtime:$
 * $Log:$
 *
 * [GC2155MIPIYUV V1.0.0]
 * 8.17.2012 Leo.Lee
 * .First Release
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by GalaxyCoreinc. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
//#include <windows.h>
//#include <memory.h>
//#include <nkintr.h>
//#include <ceddk.h>
//#include <ceddk_exp.h>

//#include "kal_release.h"
//#include "i2c_exp.h"
//#include "gpio_exp.h"
//#include "msdk_exp.h"
//#include "msdk_sensor_exp.h"
//#include "msdk_isp_exp.h"
//#include "base_regs.h"
//#include "Sensor.h"
//#include "camera_sensor_para.h"
//#include "CameraCustomized.h"

#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <asm/atomic.h>
//#include <mach/mt6516_pll.h>

#include "kd_camera_hw.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_feature.h"

#include "gc2155mipi_yuv_Sensor.h"
#include "gc2155mipi_yuv_Camera_Sensor_para.h"
#include "gc2155mipi_yuv_CameraCustomized.h"

#define GC2155MIPIYUV_DEBUG
#ifdef GC2155MIPIYUV_DEBUG
#define SENSORDB printk
#else
#define SENSORDB(x,...)
#endif

  // #define  scaler_preview
#define GC2155MIPI_Full_Preview

#define  GC2155MIPI_SET_PAGE0    GC2155MIPI_write_cmos_sensor(0xfe,0x00)
#define  GC2155MIPI_SET_PAGE1    GC2155MIPI_write_cmos_sensor(0xfe,0x01)
#define  GC2155MIPI_SET_PAGE2    GC2155MIPI_write_cmos_sensor(0xfe,0x02)
#define  GC2155MIPI_SET_PAGE3    GC2155MIPI_write_cmos_sensor(0xfe,0x03)


extern int iReadRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u8 * a_pRecvData, u16 a_sizeRecvData, u16 i2cId);
extern int iWriteRegI2C(u8 *a_pSendData , u16 a_sizeSendData, u16 i2cId);
/*************************************************************************
* FUNCTION
*    GC2155MIPI_write_cmos_sensor
*
* DESCRIPTION
*    This function wirte data to CMOS sensor through I2C
*
* PARAMETERS
*    addr: the 16bit address of register
*    para: the 8bit value of register
*
* RETURNS
*    None
*
* LOCAL AFFECTED
*
*************************************************************************/
static void GC2155MIPI_write_cmos_sensor(kal_uint8 addr, kal_uint8 para)
{
kal_uint8 out_buff[2];

    out_buff[0] = addr;
    out_buff[1] = para;

    iWriteRegI2C((u8*)out_buff , (u16)sizeof(out_buff), GC2155MIPI_WRITE_ID); 

#if (defined(__GC2155MIPI_DEBUG_TRACE__))
  if (sizeof(out_buff) != rt) printk("I2C write %x, %x error\n", addr, para);
#endif
}

/*************************************************************************
* FUNCTION
*    GC2155MIPI_read_cmos_sensor
*
* DESCRIPTION
*    This function read data from CMOS sensor through I2C.
*
* PARAMETERS
*    addr: the 16bit address of register
*
* RETURNS
*    8bit data read through I2C
*
* LOCAL AFFECTED
*
*************************************************************************/
static kal_uint8 GC2155MIPI_read_cmos_sensor(kal_uint8 addr)
{
  kal_uint8 in_buff[1] = {0xFF};
  kal_uint8 out_buff[1];
  
  out_buff[0] = addr;

    if (0 != iReadRegI2C((u8*)out_buff , (u16) sizeof(out_buff), (u8*)in_buff, (u16) sizeof(in_buff), GC2155MIPI_WRITE_ID)) {
        SENSORDB("ERROR: GC2155MIPI_read_cmos_sensor \n");
    }

#if (defined(__GC2155MIPI_DEBUG_TRACE__))
  if (size != rt) printk("I2C read %x error\n", addr);
#endif

  return in_buff[0];
}


/*******************************************************************************
* // Adapter for Winmo typedef 
********************************************************************************/
#define Sleep(ms) mdelay(ms)
#define RETAILMSG(x,...)
#define TEXT


/*******************************************************************************
* // End Adapter for Winmo typedef 
********************************************************************************/
/* Global Valuable */

static kal_uint32 zoom_factor = 0; 

static kal_bool GC2155MIPI_VEDIO_encode_mode = KAL_FALSE; //Picture(Jpeg) or Video(Mpeg4)
static kal_bool GC2155MIPI_sensor_cap_state = KAL_FALSE; //Preview or Capture

static kal_uint16 GC2155MIPI_exposure_lines=0, GC2155MIPI_extra_exposure_lines = 0;

static kal_uint16 GC2155MIPI_Capture_Shutter=0;
static kal_uint16 GC2155MIPI_Capture_Extra_Lines=0;

kal_uint32 GC2155MIPI_capture_pclk_in_M=520,GC2155MIPI_preview_pclk_in_M=390,GC2155MIPI_PV_dummy_pixels=0,GC2155MIPI_PV_dummy_lines=0,GC2155MIPI_isp_master_clock=0;

static kal_uint32  GC2155MIPI_sensor_pclk=390;

static kal_uint32 Preview_Shutter = 0;
static kal_uint32 Capture_Shutter = 0;

MSDK_SENSOR_CONFIG_STRUCT GC2155MIPISensorConfigData;

kal_uint16 GC2155MIPI_read_shutter(void)
{
	return  (GC2155MIPI_read_cmos_sensor(0x03) << 8)|GC2155MIPI_read_cmos_sensor(0x04) ;
} /* GC2155MIPI read_shutter */



static void GC2155MIPI_write_shutter(kal_uint32 shutter)
{

	if(shutter < 1)	
 	return;

	GC2155MIPI_write_cmos_sensor(0x03, (shutter >> 8) & 0x1f);
	GC2155MIPI_write_cmos_sensor(0x04, shutter & 0xff);
}    /* GC2155MIPI_write_shutter */


static void GC2155MIPI_set_mirror_flip(kal_uint8 image_mirror)
{
	kal_uint8 GC2155MIPI_HV_Mirror;

	switch (image_mirror) 
	{
		case IMAGE_NORMAL:
			GC2155MIPI_HV_Mirror = 0x14; 
		    break;
		case IMAGE_H_MIRROR:
			GC2155MIPI_HV_Mirror = 0x15;
		    break;
		case IMAGE_V_MIRROR:
			GC2155MIPI_HV_Mirror = 0x16; 
		    break;
		case IMAGE_HV_MIRROR:
			GC2155MIPI_HV_Mirror = 0x17;
		    break;
		default:
		    break;
	}
	GC2155MIPI_write_cmos_sensor(0x17, GC2155MIPI_HV_Mirror);
}

static void GC2155MIPI_set_AE_mode(kal_bool AE_enable)
{
	kal_uint8 temp_AE_reg = 0;

	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	if (AE_enable == KAL_TRUE)
	{
		// turn on AEC/AGC
		GC2155MIPI_write_cmos_sensor(0xb6, 0x01);
	}
	else
	{
		// turn off AEC/AGC
		GC2155MIPI_write_cmos_sensor(0xb6, 0x00);
	}
}


static void GC2155MIPI_set_AWB_mode(kal_bool AWB_enable)
{
	kal_uint8 temp_AWB_reg = 0;

	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	temp_AWB_reg = GC2155MIPI_read_cmos_sensor(0x82);
	if (AWB_enable == KAL_TRUE)
	{
		//enable Auto WB
		temp_AWB_reg = temp_AWB_reg | 0x02;
	}
	else
	{
		//turn off AWB
		temp_AWB_reg = temp_AWB_reg & 0xfd;
	}
	GC2155MIPI_write_cmos_sensor(0x82, temp_AWB_reg);
}


/*************************************************************************
* FUNCTION
*	GC2155MIPI_night_mode
*
* DESCRIPTION
*	This function night mode of GC2155MIPI.
*
* PARAMETERS
*	none
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void GC2155MIPI_night_mode(kal_bool enable)
{
	
		/* ==Video Preview, Auto Mode, use 39MHz PCLK, 30fps; Night Mode use 39M, 15fps */
		if (GC2155MIPI_sensor_cap_state == KAL_FALSE) 
		{
			if (enable) 
			{
				if (GC2155MIPI_VEDIO_encode_mode == KAL_TRUE) 
				{
					GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
					GC2155MIPI_write_cmos_sensor(0x3c, 0x60);
					GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
				}
				else 
				{
					GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
					GC2155MIPI_write_cmos_sensor(0x3c, 0x60);
					GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
				}
			}
			else 
			{
				/* when enter normal mode (disable night mode) without light, the AE vibrate */
				if (GC2155MIPI_VEDIO_encode_mode == KAL_TRUE) 
				{
					GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
					GC2155MIPI_write_cmos_sensor(0x3c, 0x40);
					GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
				}
				else 
				{
					GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
					GC2155MIPI_write_cmos_sensor(0x3c, 0x40);
					GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
				}
		}
	}
}	/* GC2155MIPI_night_mode */



/*************************************************************************
* FUNCTION
*	GC2155MIPI_GetSensorID
*
* DESCRIPTION
*	This function get the sensor ID
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
static kal_uint32 GC2155MIPI_GetSensorID(kal_uint32 *sensorID)

{
	   int  retry = 3; 
    // check if sensor ID correct
    do {
	
	*sensorID=((GC2155MIPI_read_cmos_sensor(0xf0) << 8) | GC2155MIPI_read_cmos_sensor(0xf1));	
	 if (*sensorID == GC2155MIPI_SENSOR_ID)
            break; 

	SENSORDB("GC2155MIPI_GetSensorID:%x \n",*sensorID);
	retry--;

	  } while (retry > 0);
	
	if (*sensorID != GC2155MIPI_SENSOR_ID) {		
		*sensorID = 0xFFFFFFFF;		
		return ERROR_SENSOR_CONNECT_FAIL;
	}
   return ERROR_NONE;
}   /* GC2155MIPIOpen  */

static void GC2155MIPI_Sensor_Init(void)
{
	zoom_factor = 0; 
	SENSORDB("GC2155MIPI_Sensor_Init");
	GC2155MIPI_write_cmos_sensor(0xfe, 0xf0);
	GC2155MIPI_write_cmos_sensor(0xfe, 0xf0);
	GC2155MIPI_write_cmos_sensor(0xfe, 0xf0);
	GC2155MIPI_write_cmos_sensor(0xfc, 0x06);
	GC2155MIPI_write_cmos_sensor(0xf6, 0x00);
	GC2155MIPI_write_cmos_sensor(0xf7, 0x1d);
	GC2155MIPI_write_cmos_sensor(0xf8, 0x84);
	GC2155MIPI_write_cmos_sensor(0xfa, 0x00);
	GC2155MIPI_write_cmos_sensor(0xf9, 0x8e);
	GC2155MIPI_write_cmos_sensor(0xf2, 0x00);
	/////////////////////////////////////////////////
	//////////////////ISP reg//////////////////////
	////////////////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	GC2155MIPI_write_cmos_sensor(0x03 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x04 , 0xe2);
	GC2155MIPI_write_cmos_sensor(0x09 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x0a , 0x00);
	GC2155MIPI_write_cmos_sensor(0x0b , 0x00);
	GC2155MIPI_write_cmos_sensor(0x0c , 0x00);
	GC2155MIPI_write_cmos_sensor(0x0d , 0x04);
	GC2155MIPI_write_cmos_sensor(0x0e , 0xc0);
	GC2155MIPI_write_cmos_sensor(0x0f , 0x06);
	GC2155MIPI_write_cmos_sensor(0x10 , 0x50);
	GC2155MIPI_write_cmos_sensor(0x12 , 0x2e);
	GC2155MIPI_write_cmos_sensor(0x17 , 0x14); // 0x14
	GC2155MIPI_write_cmos_sensor(0x18 , 0x02);
	GC2155MIPI_write_cmos_sensor(0x19 , 0x0f);
	GC2155MIPI_write_cmos_sensor(0x1a , 0x01);
	GC2155MIPI_write_cmos_sensor(0x1b , 0x4b);
	GC2155MIPI_write_cmos_sensor(0x1c , 0x07);
	GC2155MIPI_write_cmos_sensor(0x1d , 0x10);
	GC2155MIPI_write_cmos_sensor(0x1e , 0x98);
	GC2155MIPI_write_cmos_sensor(0x1f , 0x78);
	GC2155MIPI_write_cmos_sensor(0x20 , 0x05);
	GC2155MIPI_write_cmos_sensor(0x21 , 0x40);
	GC2155MIPI_write_cmos_sensor(0x22 , 0xf0);
	GC2155MIPI_write_cmos_sensor(0x24 , 0x16);
	GC2155MIPI_write_cmos_sensor(0x25 , 0x01);
	GC2155MIPI_write_cmos_sensor(0x26 , 0x10);
	GC2155MIPI_write_cmos_sensor(0x2d , 0x40);
	GC2155MIPI_write_cmos_sensor(0x30 , 0x01);
	GC2155MIPI_write_cmos_sensor(0x31 , 0x90);
	GC2155MIPI_write_cmos_sensor(0x33 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x34 , 0x01);
	/////////////////////////////////////////////////
	//////////////////ISP reg////////////////////
	/////////////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	GC2155MIPI_write_cmos_sensor(0x80 , 0xff);
	GC2155MIPI_write_cmos_sensor(0x81 , 0x2c);
	GC2155MIPI_write_cmos_sensor(0x82 , 0xfa);
	GC2155MIPI_write_cmos_sensor(0x83 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x84 , 0x03); //01
	GC2155MIPI_write_cmos_sensor(0x85 , 0x08);
	GC2155MIPI_write_cmos_sensor(0x86 , 0x06);
	GC2155MIPI_write_cmos_sensor(0x89 , 0x03);
	GC2155MIPI_write_cmos_sensor(0x8a , 0x00);
	GC2155MIPI_write_cmos_sensor(0x8b , 0x00);
	GC2155MIPI_write_cmos_sensor(0xb0 , 0x55);
	GC2155MIPI_write_cmos_sensor(0xc3 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xc4 , 0x80);
	GC2155MIPI_write_cmos_sensor(0xc5 , 0x90);
	GC2155MIPI_write_cmos_sensor(0xc6 , 0x38);
	GC2155MIPI_write_cmos_sensor(0xc7 , 0x40);
	GC2155MIPI_write_cmos_sensor(0xec , 0x02);
	GC2155MIPI_write_cmos_sensor(0xed , 0x04);
	GC2155MIPI_write_cmos_sensor(0xee , 0x60);
	GC2155MIPI_write_cmos_sensor(0xef , 0x90);
	GC2155MIPI_write_cmos_sensor(0xb6 , 0x01);
	GC2155MIPI_write_cmos_sensor(0x90 , 0x01);
	GC2155MIPI_write_cmos_sensor(0x91 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x92 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x93 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x94 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x95 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x96 , 0xb0);
	GC2155MIPI_write_cmos_sensor(0x97 , 0x06);
	GC2155MIPI_write_cmos_sensor(0x98 , 0x40);
	/////////////////////////////////////////
	/////////// , 0xBLK ////////////////////////
	/////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	GC2155MIPI_write_cmos_sensor(0x18 , 0x02);
	GC2155MIPI_write_cmos_sensor(0x40 , 0x42);
	GC2155MIPI_write_cmos_sensor(0x41 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x43 , 0x54);
	GC2155MIPI_write_cmos_sensor(0x5e , 0x00);
	GC2155MIPI_write_cmos_sensor(0x5f , 0x00);
	GC2155MIPI_write_cmos_sensor(0x60 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x61 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x62 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x63 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x64 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x65 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x66 , 0x20);
	GC2155MIPI_write_cmos_sensor(0x67 , 0x20);
	GC2155MIPI_write_cmos_sensor(0x68 , 0x20);
	GC2155MIPI_write_cmos_sensor(0x69 , 0x20);
	GC2155MIPI_write_cmos_sensor(0x6a , 0x04);
	GC2155MIPI_write_cmos_sensor(0x6b , 0x04);
	GC2155MIPI_write_cmos_sensor(0x6c , 0x04);
	GC2155MIPI_write_cmos_sensor(0x6d , 0x04);
	GC2155MIPI_write_cmos_sensor(0x6e , 0x04);
	GC2155MIPI_write_cmos_sensor(0x6f , 0x04);
	GC2155MIPI_write_cmos_sensor(0x70 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x71 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x72 , 0xf0);
	GC2155MIPI_write_cmos_sensor(0x7e , 0x3c);
	GC2155MIPI_write_cmos_sensor(0x7f , 0x00);
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	////////////////////////////////////////
	/////////// AEC ////////////////////////
	////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
	GC2155MIPI_write_cmos_sensor(0x01 , 0x08);
	GC2155MIPI_write_cmos_sensor(0x02 , 0xc0);
	GC2155MIPI_write_cmos_sensor(0x03 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x04 , 0x90);
	GC2155MIPI_write_cmos_sensor(0x05 , 0x30);
	GC2155MIPI_write_cmos_sensor(0x06 , 0x98);
	GC2155MIPI_write_cmos_sensor(0x07 , 0x28);
	GC2155MIPI_write_cmos_sensor(0x08 , 0x6c);
	GC2155MIPI_write_cmos_sensor(0x09 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x0a , 0xc2);
	GC2155MIPI_write_cmos_sensor(0x0b , 0x11);
	GC2155MIPI_write_cmos_sensor(0x0c , 0x10);
	GC2155MIPI_write_cmos_sensor(0x13 , 0x2a);
	GC2155MIPI_write_cmos_sensor(0x17 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x1c , 0x11);
	GC2155MIPI_write_cmos_sensor(0x1e , 0x61);
	GC2155MIPI_write_cmos_sensor(0x1f , 0x30);
	GC2155MIPI_write_cmos_sensor(0x20 , 0x40);
	GC2155MIPI_write_cmos_sensor(0x22 , 0x80);
	GC2155MIPI_write_cmos_sensor(0x23 , 0x20);

	GC2155MIPI_write_cmos_sensor(0x12 , 0x35);
	GC2155MIPI_write_cmos_sensor(0x15 , 0x50);
	GC2155MIPI_write_cmos_sensor(0x10 , 0x31);
	GC2155MIPI_write_cmos_sensor(0x3e , 0x28);
	GC2155MIPI_write_cmos_sensor(0x3f , 0xe0);
	GC2155MIPI_write_cmos_sensor(0x40 , 0xe0);
	GC2155MIPI_write_cmos_sensor(0x41 , 0x08);

	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0x0f , 0x05);
	/////////////////////////////
	//////// INTPEE /////////////
	/////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0x90 , 0x6c);
	GC2155MIPI_write_cmos_sensor(0x91 , 0x02);
	GC2155MIPI_write_cmos_sensor(0x92 , 0x44);
	GC2155MIPI_write_cmos_sensor(0x97 , 0x64);
	GC2155MIPI_write_cmos_sensor(0x98 , 0x88);
	GC2155MIPI_write_cmos_sensor(0x9d , 0x08);
	GC2155MIPI_write_cmos_sensor(0xa2 , 0x11);
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	/////////////////////////////
	//////// DNDD///////////////
	/////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0x80 , 0xc1);
	GC2155MIPI_write_cmos_sensor(0x81 , 0x08);
	GC2155MIPI_write_cmos_sensor(0x82 , 0x05);
	GC2155MIPI_write_cmos_sensor(0x83 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x84 , 0x0a);
	GC2155MIPI_write_cmos_sensor(0x86 , 0x80);
	GC2155MIPI_write_cmos_sensor(0x87 , 0x30);
	GC2155MIPI_write_cmos_sensor(0x88 , 0x15);
	GC2155MIPI_write_cmos_sensor(0x89 , 0x80);
	GC2155MIPI_write_cmos_sensor(0x8a , 0x60);
	GC2155MIPI_write_cmos_sensor(0x8b , 0x30);
	/////////////////////////////////////////
	/////////// ASDE ////////////////////////
	/////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
	GC2155MIPI_write_cmos_sensor(0x21 , 0x14);
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0x3c , 0x06);
	GC2155MIPI_write_cmos_sensor(0x3d , 0x40);
	GC2155MIPI_write_cmos_sensor(0x48 , 0x30);
	GC2155MIPI_write_cmos_sensor(0x49 , 0x06);
	GC2155MIPI_write_cmos_sensor(0x4b , 0x08);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x20);
	GC2155MIPI_write_cmos_sensor(0xa3 , 0x40);
	GC2155MIPI_write_cmos_sensor(0xa4 , 0x20);
	GC2155MIPI_write_cmos_sensor(0xa5 , 0x40);
	GC2155MIPI_write_cmos_sensor(0xa6 , 0x80);
	GC2155MIPI_write_cmos_sensor(0xab , 0x20);
	GC2155MIPI_write_cmos_sensor(0xae , 0x0c);
	GC2155MIPI_write_cmos_sensor(0xb3 , 0x42);
	GC2155MIPI_write_cmos_sensor(0xb4 , 0x24);
	GC2155MIPI_write_cmos_sensor(0xb6 , 0x50);
	GC2155MIPI_write_cmos_sensor(0xb7 , 0x01);
	GC2155MIPI_write_cmos_sensor(0xb9 , 0x25); 
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);	 
	///////////////////gamma1////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0x10 , 0x0a);
	GC2155MIPI_write_cmos_sensor(0x11 , 0x12);
	GC2155MIPI_write_cmos_sensor(0x12 , 0x19);
	GC2155MIPI_write_cmos_sensor(0x13 , 0x1f);
	GC2155MIPI_write_cmos_sensor(0x14 , 0x2c);
	GC2155MIPI_write_cmos_sensor(0x15 , 0x38);
	GC2155MIPI_write_cmos_sensor(0x16 , 0x42);
	GC2155MIPI_write_cmos_sensor(0x17 , 0x4e);
	GC2155MIPI_write_cmos_sensor(0x18 , 0x63);
	GC2155MIPI_write_cmos_sensor(0x19 , 0x76);
	GC2155MIPI_write_cmos_sensor(0x1a , 0x87);
	GC2155MIPI_write_cmos_sensor(0x1b , 0x96);
	GC2155MIPI_write_cmos_sensor(0x1c , 0xa2);
	GC2155MIPI_write_cmos_sensor(0x1d , 0xb8);
	GC2155MIPI_write_cmos_sensor(0x1e , 0xcb);
	GC2155MIPI_write_cmos_sensor(0x1f , 0xd8);
	GC2155MIPI_write_cmos_sensor(0x20 , 0xe2);
	GC2155MIPI_write_cmos_sensor(0x21 , 0xe9);
	GC2155MIPI_write_cmos_sensor(0x22 , 0xf0);
	GC2155MIPI_write_cmos_sensor(0x23 , 0xf8);
	GC2155MIPI_write_cmos_sensor(0x24 , 0xfd);
	GC2155MIPI_write_cmos_sensor(0x25 , 0xff);
	///////////////////gamma2////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0x26 , 0x0d);
	GC2155MIPI_write_cmos_sensor(0x27 , 0x12);
	GC2155MIPI_write_cmos_sensor(0x28 , 0x17);
	GC2155MIPI_write_cmos_sensor(0x29 , 0x1c);
	GC2155MIPI_write_cmos_sensor(0x2a , 0x27);
	GC2155MIPI_write_cmos_sensor(0x2b , 0x34);
	GC2155MIPI_write_cmos_sensor(0x2c , 0x44);
	GC2155MIPI_write_cmos_sensor(0x2d , 0x55);
	GC2155MIPI_write_cmos_sensor(0x2e , 0x6e);
	GC2155MIPI_write_cmos_sensor(0x2f , 0x81);
	GC2155MIPI_write_cmos_sensor(0x30 , 0x91);
	GC2155MIPI_write_cmos_sensor(0x31 , 0x9c);
	GC2155MIPI_write_cmos_sensor(0x32 , 0xaa);
	GC2155MIPI_write_cmos_sensor(0x33 , 0xbb);
	GC2155MIPI_write_cmos_sensor(0x34 , 0xca);
	GC2155MIPI_write_cmos_sensor(0x35 , 0xd5);
	GC2155MIPI_write_cmos_sensor(0x36 , 0xe0);
	GC2155MIPI_write_cmos_sensor(0x37 , 0xe7);
	GC2155MIPI_write_cmos_sensor(0x38 , 0xed);
	GC2155MIPI_write_cmos_sensor(0x39 , 0xf6);
	GC2155MIPI_write_cmos_sensor(0x3a , 0xfb);
	GC2155MIPI_write_cmos_sensor(0x3b , 0xff);
	/////////////////////////////////////////////// 
	///////////YCP /////////////////////// 
	/////////////////////////////////////////////// 
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0xd1 , 0x28);
	GC2155MIPI_write_cmos_sensor(0xd2 , 0x28);
	GC2155MIPI_write_cmos_sensor(0xdd , 0x14);
	GC2155MIPI_write_cmos_sensor(0xde , 0x88);
	GC2155MIPI_write_cmos_sensor(0xed , 0x80);
	////////////////////////////
	//////// LSC ///////////////
	////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
	GC2155MIPI_write_cmos_sensor(0xc2 , 0x15);
	GC2155MIPI_write_cmos_sensor(0xc3 , 0x0c);
	GC2155MIPI_write_cmos_sensor(0xc4 , 0x0b);
	GC2155MIPI_write_cmos_sensor(0xc8 , 0x12);
	GC2155MIPI_write_cmos_sensor(0xc9 , 0x0b);
	GC2155MIPI_write_cmos_sensor(0xca , 0x07);
	GC2155MIPI_write_cmos_sensor(0xbc , 0x4d);
	GC2155MIPI_write_cmos_sensor(0xbd , 0x2d);
	GC2155MIPI_write_cmos_sensor(0xbe , 0x29);
	GC2155MIPI_write_cmos_sensor(0xb6 , 0x3f);
	GC2155MIPI_write_cmos_sensor(0xb7 , 0x28);
	GC2155MIPI_write_cmos_sensor(0xb8 , 0x2a);
	GC2155MIPI_write_cmos_sensor(0xc5 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xc6 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xc7 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xcb , 0x00);
	GC2155MIPI_write_cmos_sensor(0xcc , 0x00);
	GC2155MIPI_write_cmos_sensor(0xcd , 0x00);
	GC2155MIPI_write_cmos_sensor(0xbf , 0x0e);
	GC2155MIPI_write_cmos_sensor(0xc0 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xc1 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xb9 , 0x08);
	GC2155MIPI_write_cmos_sensor(0xba , 0x00);
	GC2155MIPI_write_cmos_sensor(0xbb , 0x00);
	GC2155MIPI_write_cmos_sensor(0xaa , 0x01);
	GC2155MIPI_write_cmos_sensor(0xab , 0x0f);
	GC2155MIPI_write_cmos_sensor(0xac , 0x0d);
	GC2155MIPI_write_cmos_sensor(0xad , 0x00);
	GC2155MIPI_write_cmos_sensor(0xae , 0x06);
	GC2155MIPI_write_cmos_sensor(0xaf , 0x08);
	GC2155MIPI_write_cmos_sensor(0xb0 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xb1 , 0x06);
	GC2155MIPI_write_cmos_sensor(0xb2 , 0x02);
	GC2155MIPI_write_cmos_sensor(0xb3 , 0x01);
	GC2155MIPI_write_cmos_sensor(0xb4 , 0x08);
	GC2155MIPI_write_cmos_sensor(0xb5 , 0x05);
	GC2155MIPI_write_cmos_sensor(0xd0 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd1 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd2 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd6 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd7 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd8 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd9 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xda , 0x00);
	GC2155MIPI_write_cmos_sensor(0xdb , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd3 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd4 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xd5 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xa4 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xa5 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xa6 , 0x77);
	GC2155MIPI_write_cmos_sensor(0xa7 , 0x77);
	GC2155MIPI_write_cmos_sensor(0xa8 , 0x77);
	GC2155MIPI_write_cmos_sensor(0xa9 , 0x77);
	GC2155MIPI_write_cmos_sensor(0xa1 , 0x80);
	GC2155MIPI_write_cmos_sensor(0xa2 , 0x80);

	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
	GC2155MIPI_write_cmos_sensor(0xdc , 0x35);
	GC2155MIPI_write_cmos_sensor(0xdd , 0x28);
	GC2155MIPI_write_cmos_sensor(0xdf , 0x0c);
	GC2155MIPI_write_cmos_sensor(0xe0 , 0x70);
	GC2155MIPI_write_cmos_sensor(0xe1 , 0x80);
	GC2155MIPI_write_cmos_sensor(0xe2 , 0x80);
	GC2155MIPI_write_cmos_sensor(0xe3 , 0x80);
	GC2155MIPI_write_cmos_sensor(0xe6 , 0x90);
	GC2155MIPI_write_cmos_sensor(0xe7 , 0x50);
	GC2155MIPI_write_cmos_sensor(0xe8 , 0x90);
	GC2155MIPI_write_cmos_sensor(0xe9 , 0x60);
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	///////////////////////////////////////////////
	/////////// AWB////////////////////////
	///////////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4f , 0x00);
	GC2155MIPI_write_cmos_sensor(0x4f , 0x00);
	GC2155MIPI_write_cmos_sensor(0x4b , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4f , 0x00);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x50);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x70);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x4f);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x6f);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x8f);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x6e);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x8e);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xae);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xce);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x6d);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x8d);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xad);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xcd);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x6c);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x8c);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xac);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xcc);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xec);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x6b);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x8b);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xab);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x03);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xaa);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x04);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xca);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x04);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xa9);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x04);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xeb);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x05);
	GC2155MIPI_write_cmos_sensor(0xa1 , 0x80);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x0b);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x05);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x01);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xea);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x05);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x0a);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x05);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x29);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x06);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x49);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x06);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x48);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x00);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0x87);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x07);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xa7);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x07);
	GC2155MIPI_write_cmos_sensor(0x4c , 0x02);
	GC2155MIPI_write_cmos_sensor(0x4d , 0xc7);
	GC2155MIPI_write_cmos_sensor(0x4e , 0x07);
	GC2155MIPI_write_cmos_sensor(0x4f , 0x01);
	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);

	GC2155MIPI_write_cmos_sensor(0x50 , 0x80);
	GC2155MIPI_write_cmos_sensor(0x51 , 0xa8);
	GC2155MIPI_write_cmos_sensor(0x52 , 0x57);
	GC2155MIPI_write_cmos_sensor(0x54 , 0x38);
	GC2155MIPI_write_cmos_sensor(0x55 , 0xc7);
	GC2155MIPI_write_cmos_sensor(0x56 , 0x0e);
	GC2155MIPI_write_cmos_sensor(0x58 , 0x08);
	GC2155MIPI_write_cmos_sensor(0x5b , 0x00);
	GC2155MIPI_write_cmos_sensor(0x5c , 0x74);
	GC2155MIPI_write_cmos_sensor(0x5d , 0x8b);
	GC2155MIPI_write_cmos_sensor(0x61 , 0xd3);
	GC2155MIPI_write_cmos_sensor(0x62 , 0x90);
	GC2155MIPI_write_cmos_sensor(0x63 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x65 , 0x04);
	GC2155MIPI_write_cmos_sensor(0x67 , 0xb2);
	GC2155MIPI_write_cmos_sensor(0x68 , 0xac);
	GC2155MIPI_write_cmos_sensor(0x69 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x6a , 0xb2);
	GC2155MIPI_write_cmos_sensor(0x6b , 0xac);
	GC2155MIPI_write_cmos_sensor(0x6c , 0xdc);
	GC2155MIPI_write_cmos_sensor(0x6d , 0xb0);
	GC2155MIPI_write_cmos_sensor(0x6e , 0x30);
	GC2155MIPI_write_cmos_sensor(0x6f , 0x50);
	GC2155MIPI_write_cmos_sensor(0x70 , 0x05); 
	GC2155MIPI_write_cmos_sensor(0x71 , 0x80); 
	GC2155MIPI_write_cmos_sensor(0x72 , 0x80); 
	GC2155MIPI_write_cmos_sensor(0x73 , 0x30); 
	GC2155MIPI_write_cmos_sensor(0x74 , 0x01); 
	GC2155MIPI_write_cmos_sensor(0x75 , 0x01); 
	GC2155MIPI_write_cmos_sensor(0x7f , 0x08);
	GC2155MIPI_write_cmos_sensor(0x76 , 0x70); 
	GC2155MIPI_write_cmos_sensor(0x77 , 0x48); 
	GC2155MIPI_write_cmos_sensor(0x78 , 0xa0); 
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	//////////////////////////////////////////
	///////////CC////////////////////////
	//////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x02);
	GC2155MIPI_write_cmos_sensor(0xc0 , 0x01);
	GC2155MIPI_write_cmos_sensor(0xc1 , 0x4e);
	GC2155MIPI_write_cmos_sensor(0xc2 , 0xf8);
	GC2155MIPI_write_cmos_sensor(0xc3 , 0x00);
	GC2155MIPI_write_cmos_sensor(0xc4 , 0xec);
	GC2155MIPI_write_cmos_sensor(0xc5 , 0x45);
	GC2155MIPI_write_cmos_sensor(0xc6 , 0xee);
	GC2155MIPI_write_cmos_sensor(0xc7 , 0x45);
	GC2155MIPI_write_cmos_sensor(0xc8 , 0xf8);
	GC2155MIPI_write_cmos_sensor(0xc9 , 0x02);
	GC2155MIPI_write_cmos_sensor(0xca , 0xfe);
	GC2155MIPI_write_cmos_sensor(0xcb , 0x42);
	GC2155MIPI_write_cmos_sensor(0xcc , 0x00);
	GC2155MIPI_write_cmos_sensor(0xcd , 0x45);
	GC2155MIPI_write_cmos_sensor(0xce , 0xf0);
	GC2155MIPI_write_cmos_sensor(0xcf , 0x00);
	GC2155MIPI_write_cmos_sensor(0xe3 , 0xf0);
	GC2155MIPI_write_cmos_sensor(0xe4 , 0x45);
	GC2155MIPI_write_cmos_sensor(0xe5 , 0xe8); 
	//////////////////////////////////////
	///////////  OUTPUT   ////////////////
	//////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	GC2155MIPI_write_cmos_sensor(0xf2, 0x00);

	//////////////frame rate 50Hz/////////
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
	GC2155MIPI_write_cmos_sensor(0x05 , 0x01);
	GC2155MIPI_write_cmos_sensor(0x06 , 0x56);
	GC2155MIPI_write_cmos_sensor(0x07 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x08 , 0x32);
	GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
	GC2155MIPI_write_cmos_sensor(0x25 , 0x00);
	GC2155MIPI_write_cmos_sensor(0x26 , 0xfa); 
	GC2155MIPI_write_cmos_sensor(0x27 , 0x04); 
	GC2155MIPI_write_cmos_sensor(0x28 , 0xe2); //20fps 
	GC2155MIPI_write_cmos_sensor(0x29 , 0x05); 
	GC2155MIPI_write_cmos_sensor(0x2a , 0xdc); //14fps 
	GC2155MIPI_write_cmos_sensor(0x2b , 0x07); 
	GC2155MIPI_write_cmos_sensor(0x2c , 0xd0); //10fps
	GC2155MIPI_write_cmos_sensor(0x2d , 0x0b); 
	GC2155MIPI_write_cmos_sensor(0x2e , 0xb8); //8fps
	GC2155MIPI_write_cmos_sensor(0xfe , 0x00);

	/////////////////////////////////////////////////////
	//////////////////////   MIPI   /////////////////////
	/////////////////////////////////////////////////////
	GC2155MIPI_write_cmos_sensor(0xfe, 0x03);
	GC2155MIPI_write_cmos_sensor(0x01, 0x07);
	GC2155MIPI_write_cmos_sensor(0x02, 0x22);
	GC2155MIPI_write_cmos_sensor(0x03, 0x12);
	GC2155MIPI_write_cmos_sensor(0x04, 0x01);
	GC2155MIPI_write_cmos_sensor(0x05, 0x00);
	GC2155MIPI_write_cmos_sensor(0x06, 0x88);
	GC2155MIPI_write_cmos_sensor(0x10, 0x95);
	GC2155MIPI_write_cmos_sensor(0x11, 0x1e);
	GC2155MIPI_write_cmos_sensor(0x12, 0x80);
	GC2155MIPI_write_cmos_sensor(0x13, 0x0c);
	GC2155MIPI_write_cmos_sensor(0x15, 0x12);
	GC2155MIPI_write_cmos_sensor(0x17, 0xf0);
	
	GC2155MIPI_write_cmos_sensor(0x21, 0x01);
	GC2155MIPI_write_cmos_sensor(0x22, 0x02);
	GC2155MIPI_write_cmos_sensor(0x23, 0x01);
	GC2155MIPI_write_cmos_sensor(0x24, 0x10);
	GC2155MIPI_write_cmos_sensor(0x29, 0x02);
	GC2155MIPI_write_cmos_sensor(0x2a, 0x03);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	
}


static void GC2155MIPI_Sensor_SVGA(void)
{
	SENSORDB("GC2155MIPI_Sensor_SVGA");
	GC2155MIPI_write_cmos_sensor(0xf7, 0x35);
	GC2155MIPI_write_cmos_sensor(0xfd, 0x01);
	//// crop window             
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	GC2155MIPI_write_cmos_sensor(0x90, 0x01);
	GC2155MIPI_write_cmos_sensor(0x91, 0x00);
	GC2155MIPI_write_cmos_sensor(0x92, 0x00);
	GC2155MIPI_write_cmos_sensor(0x93, 0x00);
	GC2155MIPI_write_cmos_sensor(0x94, 0x00);
	GC2155MIPI_write_cmos_sensor(0x95, 0x02);
	GC2155MIPI_write_cmos_sensor(0x96, 0x58);
	GC2155MIPI_write_cmos_sensor(0x97, 0x03);
	GC2155MIPI_write_cmos_sensor(0x98, 0x20);
	GC2155MIPI_write_cmos_sensor(0x99, 0x11);
	GC2155MIPI_write_cmos_sensor(0x9a, 0x06);
	//// AWB                     
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	GC2155MIPI_write_cmos_sensor(0xec, 0x01);
	GC2155MIPI_write_cmos_sensor(0xed, 0x02);
	GC2155MIPI_write_cmos_sensor(0xee, 0x30);
	GC2155MIPI_write_cmos_sensor(0xef, 0x48);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
	GC2155MIPI_write_cmos_sensor(0x74, 0x00);
	//// AEC                     
	GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
	GC2155MIPI_write_cmos_sensor(0x01, 0x04);
	GC2155MIPI_write_cmos_sensor(0x02, 0x60);
	GC2155MIPI_write_cmos_sensor(0x03, 0x02);
	GC2155MIPI_write_cmos_sensor(0x04, 0x48);
	GC2155MIPI_write_cmos_sensor(0x05, 0x18);
	GC2155MIPI_write_cmos_sensor(0x06, 0x4c);
	GC2155MIPI_write_cmos_sensor(0x07, 0x14);
	GC2155MIPI_write_cmos_sensor(0x08, 0x36);
	GC2155MIPI_write_cmos_sensor(0x0a, 0xc0);
	GC2155MIPI_write_cmos_sensor(0x21, 0x14);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	//// mipi
	GC2155MIPI_write_cmos_sensor(0xfe, 0x03);
	GC2155MIPI_write_cmos_sensor(0x12, 0x40);
	GC2155MIPI_write_cmos_sensor(0x13, 0x06);
	GC2155MIPI_write_cmos_sensor(0x04, 0x01);
	GC2155MIPI_write_cmos_sensor(0x05, 0x00);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
}

static void GC2155MIPI_Sensor_2M(void)
{
	SENSORDB("GC2155MIPI_Sensor_2M");
	GC2155MIPI_write_cmos_sensor(0xf7, 0x1d);	
	GC2155MIPI_write_cmos_sensor(0xfd, 0x00); 
	//// crop window           
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	GC2155MIPI_write_cmos_sensor(0x90, 0x01); 
	GC2155MIPI_write_cmos_sensor(0x91, 0x00);
	GC2155MIPI_write_cmos_sensor(0x92, 0x00);
	GC2155MIPI_write_cmos_sensor(0x93, 0x00);
	GC2155MIPI_write_cmos_sensor(0x94, 0x00);
	GC2155MIPI_write_cmos_sensor(0x95, 0x04);
	GC2155MIPI_write_cmos_sensor(0x96, 0xb0);
	GC2155MIPI_write_cmos_sensor(0x97, 0x06);
	GC2155MIPI_write_cmos_sensor(0x98, 0x40);
	GC2155MIPI_write_cmos_sensor(0x99, 0x11); 
	GC2155MIPI_write_cmos_sensor(0x9a, 0x06);
	//// AWB                   
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	GC2155MIPI_write_cmos_sensor(0xec, 0x02); 
	GC2155MIPI_write_cmos_sensor(0xed, 0x04);
	GC2155MIPI_write_cmos_sensor(0xee, 0x60);
	GC2155MIPI_write_cmos_sensor(0xef, 0x90);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
	GC2155MIPI_write_cmos_sensor(0x74, 0x01); 
	//// AEC                    
	GC2155MIPI_write_cmos_sensor(0xfe, 0x01);
	GC2155MIPI_write_cmos_sensor(0x01, 0x08);
	GC2155MIPI_write_cmos_sensor(0x02, 0xc0);
	GC2155MIPI_write_cmos_sensor(0x03, 0x04);
	GC2155MIPI_write_cmos_sensor(0x04, 0x90);
	GC2155MIPI_write_cmos_sensor(0x05, 0x30);
	GC2155MIPI_write_cmos_sensor(0x06, 0x98);
	GC2155MIPI_write_cmos_sensor(0x07, 0x28);
	GC2155MIPI_write_cmos_sensor(0x08, 0x6c);
	GC2155MIPI_write_cmos_sensor(0x0a, 0xc2);
	GC2155MIPI_write_cmos_sensor(0x21, 0x14);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
	//// mipi
	GC2155MIPI_write_cmos_sensor(0xfe, 0x03);
	GC2155MIPI_write_cmos_sensor(0x12, 0x80);
	GC2155MIPI_write_cmos_sensor(0x13, 0x0c);
	GC2155MIPI_write_cmos_sensor(0x04, 0x01);
	GC2155MIPI_write_cmos_sensor(0x05, 0x00);
	GC2155MIPI_write_cmos_sensor(0xfe, 0x00);

}


/*****************************************************************************/
/* Windows Mobile Sensor Interface */
/*****************************************************************************/
/*************************************************************************
* FUNCTION
*	GC2155MIPIOpen
*
* DESCRIPTION
*	This function initialize the registers of CMOS sensor
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 GC2155MIPIOpen(void)
{
	volatile signed char i;
	kal_uint16 sensor_id=0;

	zoom_factor = 0; 
	Sleep(10);


	//  Read sensor ID to adjust I2C is OK?
	for(i=0;i<3;i++)
	{
		sensor_id=((GC2155MIPI_read_cmos_sensor(0xf0) << 8) | GC2155MIPI_read_cmos_sensor(0xf1));   
		SENSORDB("GC2155MIPI_Open, sensor_id:%x \n",sensor_id);
		if (sensor_id != GC2155MIPI_SENSOR_ID)
		return ERROR_SENSOR_CONNECT_FAIL;
	}
	
		SENSORDB("GC2155MIPI Sensor Read ID OK \r\n");
		GC2155MIPI_Sensor_Init();
	
	Preview_Shutter =GC2155MIPI_read_shutter();
	
	return ERROR_NONE;
}	/* GC2155MIPIOpen() */

/*************************************************************************
* FUNCTION
*	GC2155MIPIClose
*
* DESCRIPTION
*	This function is to turn off sensor module power.
*
* PARAMETERS
*	None
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 GC2155MIPIClose(void)
{
//	CISModulePowerOn(FALSE);
	return ERROR_NONE;
}	/* GC2155MIPIClose() */

/*************************************************************************
* FUNCTION
*	GC2155MIPIPreview
*
* DESCRIPTION
*	This function start the sensor preview.
*
* PARAMETERS
*	*image_window : address pointer of pixel numbers in one period of HSYNC
*  *sensor_config_data : address pointer of line numbers in one period of VSYNC
*
* RETURNS
*	None
*
* GLOBALS AFFECTED
*
*************************************************************************/
UINT32 GC2155MIPIPreview(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
	kal_uint8 iTemp, temp_AE_reg, temp_AWB_reg;
	kal_uint16 iDummyPixels = 0, iDummyLines = 0, iStartX = 0, iStartY = 0;

	SENSORDB("GC2155MIPIPrevie\n");

	GC2155MIPI_sensor_cap_state = KAL_FALSE;

	//GC2155MIPI_write_cmos_sensor(0xfa, 0x00);
 	 GC2155MIPI_write_shutter(Preview_Shutter);
#ifdef GC2155MIPI_Full_Preview
	GC2155MIPI_Sensor_2M();
#else
	GC2155MIPI_Sensor_SVGA();
#endif

	GC2155MIPI_set_AE_mode(KAL_TRUE); 

	memcpy(&GC2155MIPISensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* GC2155MIPIPreview() */




UINT32 GC2155MIPICapture(MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *image_window,
					  MSDK_SENSOR_CONFIG_STRUCT *sensor_config_data)
{
    volatile kal_uint32 shutter = GC2155MIPI_exposure_lines, temp_reg;
    kal_uint8 temp_AE_reg, temp;
    kal_uint16 AE_setting_delay = 0;

    SENSORDB("GC2155MIPICapture\n");

  if(GC2155MIPI_sensor_cap_state == KAL_FALSE)
 	{

 	#ifdef GC2155MIPI_Full_Preview

		 GC2155MIPI_Sensor_2M();

	#else
    // turn off AEC/AGC
	     GC2155MIPI_set_AE_mode(KAL_FALSE);

	    shutter = GC2155MIPI_read_shutter();
	    Preview_Shutter = shutter;

	   //GC2155MIPI_write_cmos_sensor(0xfa, 0x11);
	   GC2155MIPI_Sensor_2M();


	  Capture_Shutter = shutter / 2; 
        
        // set shutter
        //GC2155MIPI_write_shutter(Capture_Shutter);
        #endif
	Sleep(200);
      }

     GC2155MIPI_sensor_cap_state = KAL_TRUE;

	image_window->GrabStartX=1;
        image_window->GrabStartY=1;
        image_window->ExposureWindowWidth=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH - image_window->GrabStartX;
        image_window->ExposureWindowHeight=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT -image_window->GrabStartY;    	 

    memcpy(&GC2155MIPISensorConfigData, sensor_config_data, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* GC2155MIPICapture() */



UINT32 GC2155MIPIGetResolution(MSDK_SENSOR_RESOLUTION_INFO_STRUCT *pSensorResolution)
{
	pSensorResolution->SensorFullWidth=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;
	pSensorResolution->SensorFullHeight=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;
	#ifdef GC2155MIPI_Full_Preview
	pSensorResolution->SensorPreviewWidth=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;
	pSensorResolution->SensorPreviewHeight=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;
	pSensorResolution->SensorVideoWidth=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;
	pSensorResolution->SensorVideoHeight=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;
	#else
	pSensorResolution->SensorPreviewWidth=GC2155MIPI_IMAGE_SENSOR_PV_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;
	pSensorResolution->SensorPreviewHeight=GC2155MIPI_IMAGE_SENSOR_PV_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;
	pSensorResolution->SensorVideoWidth=GC2155MIPI_IMAGE_SENSOR_PV_WIDTH - 2 * IMAGE_SENSOR_START_GRAB_X;
	pSensorResolution->SensorVideoHeight=GC2155MIPI_IMAGE_SENSOR_PV_HEIGHT - 2 * IMAGE_SENSOR_START_GRAB_Y;
	#endif
	return ERROR_NONE;
}	/* GC2155MIPIGetResolution() */

UINT32 GC2155MIPIGetInfo(MSDK_SCENARIO_ID_ENUM ScenarioId,
					  MSDK_SENSOR_INFO_STRUCT *pSensorInfo,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	#ifdef GC2155MIPI_Full_Preview
	pSensorInfo->SensorPreviewResolutionX=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH;
	pSensorInfo->SensorPreviewResolutionY=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT;
	#else
	pSensorInfo->SensorPreviewResolutionX=GC2155MIPI_IMAGE_SENSOR_PV_WIDTH;
	pSensorInfo->SensorPreviewResolutionY=GC2155MIPI_IMAGE_SENSOR_PV_HEIGHT;
	#endif
	pSensorInfo->SensorFullResolutionX=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH;
	pSensorInfo->SensorFullResolutionY=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT;

	pSensorInfo->SensorCameraPreviewFrameRate=30;
	pSensorInfo->SensorVideoFrameRate=30;
	pSensorInfo->SensorStillCaptureFrameRate=10;
	pSensorInfo->SensorWebCamCaptureFrameRate=15;
	pSensorInfo->SensorResetActiveHigh=FALSE;
	pSensorInfo->SensorResetDelayCount=1;
	pSensorInfo->SensorOutputDataFormat=SENSOR_OUTPUT_FORMAT_YUYV;
	pSensorInfo->SensorClockPolarity=SENSOR_CLOCK_POLARITY_LOW;	/*??? */
	pSensorInfo->SensorClockFallingPolarity=SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorHsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorVsyncPolarity = SENSOR_CLOCK_POLARITY_LOW;
	pSensorInfo->SensorInterruptDelayLines = 1;
	pSensorInfo->CaptureDelayFrame = 4; 
	pSensorInfo->PreviewDelayFrame = 1; 
	pSensorInfo->VideoDelayFrame = 0; 
       pSensorInfo->YUVAwbDelayFrame = 2;  // add by lanking
	pSensorInfo->YUVEffectDelayFrame = 2;  // add by lanking
	pSensorInfo->SensorMasterClockSwitch = 0; 
	pSensorInfo->SensorDrivingCurrent = ISP_DRIVING_6MA;


	pSensorInfo->SensroInterfaceType = SENSOR_INTERFACE_TYPE_MIPI;

	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
                     pSensorInfo->SensorGrabStartX = 2; 
                     pSensorInfo->SensorGrabStartY = 2;

			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;		
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 14;
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;  // 0 is default 1x
			pSensorInfo->SensorHightSampling = 0;   // 0 is default 1x 
			pSensorInfo->SensorPacketECCOrder = 1;

	
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount= 0;
			pSensorInfo->SensorClockFallingCount= 2;
			pSensorInfo->SensorPixelClockCount= 3;
			pSensorInfo->SensorDataLatchCount= 2;
                     pSensorInfo->SensorGrabStartX = 2; 
                     pSensorInfo->SensorGrabStartY = 2;

			pSensorInfo->SensorMIPILaneNumber = SENSOR_MIPI_2_LANE;		
			pSensorInfo->MIPIDataLowPwr2HighSpeedTermDelayCount = 0; 
			pSensorInfo->MIPIDataLowPwr2HighSpeedSettleDelayCount = 14;
			pSensorInfo->MIPICLKLowPwr2HighSpeedTermDelayCount = 0;
			pSensorInfo->SensorWidthSampling = 0;  // 0 is default 1x
			pSensorInfo->SensorHightSampling = 0;   // 0 is default 1x 
			pSensorInfo->SensorPacketECCOrder = 1;

			
		break;
		default:
			pSensorInfo->SensorClockFreq=24;
			pSensorInfo->SensorClockDividCount=3;
			pSensorInfo->SensorClockRisingCount=0;
			pSensorInfo->SensorClockFallingCount=2;
			pSensorInfo->SensorPixelClockCount=3;
			pSensorInfo->SensorDataLatchCount=2;
                     pSensorInfo->SensorGrabStartX = 2; 
                     pSensorInfo->SensorGrabStartY = 2;             
			
		break;
	}
	memcpy(pSensorConfigData, &GC2155MIPISensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
	return ERROR_NONE;
}	/* GC2155MIPIGetInfo() */


UINT32 GC2155MIPIControl(MSDK_SCENARIO_ID_ENUM ScenarioId, MSDK_SENSOR_EXPOSURE_WINDOW_STRUCT *pImageWindow,
					  MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData)
{
	switch (ScenarioId)
	{
		case MSDK_SCENARIO_ID_CAMERA_PREVIEW:
		case MSDK_SCENARIO_ID_VIDEO_PREVIEW:
			GC2155MIPIPreview(pImageWindow, pSensorConfigData);
		break;
		case MSDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG:
			GC2155MIPICapture(pImageWindow, pSensorConfigData);
		break;
		default:
		    break; 
	}
	return TRUE;
}	/* GC2155MIPIControl() */

BOOL GC2155MIPI_set_param_wb(UINT16 para)
{
	switch (para)
	{
		case AWB_MODE_AUTO:
			GC2155MIPI_write_cmos_sensor(0xb3, 0x61);
			GC2155MIPI_write_cmos_sensor(0xb4, 0x40);
			GC2155MIPI_write_cmos_sensor(0xb5, 0x61);
			GC2155MIPI_set_AWB_mode(KAL_TRUE);
		break;
		case AWB_MODE_CLOUDY_DAYLIGHT: //cloudy
			GC2155MIPI_set_AWB_mode(KAL_FALSE);
			GC2155MIPI_write_cmos_sensor(0xb3, 0x58);
			GC2155MIPI_write_cmos_sensor(0xb4, 0x40);
			GC2155MIPI_write_cmos_sensor(0xb5, 0x50);
		break;
		case AWB_MODE_DAYLIGHT: //sunny
			GC2155MIPI_set_AWB_mode(KAL_FALSE);
			GC2155MIPI_write_cmos_sensor(0xb3, 0x70);
			GC2155MIPI_write_cmos_sensor(0xb4, 0x40);
			GC2155MIPI_write_cmos_sensor(0xb5, 0x50);
		break;
		case AWB_MODE_INCANDESCENT: //office
			GC2155MIPI_set_AWB_mode(KAL_FALSE);
			GC2155MIPI_write_cmos_sensor(0xb3, 0x50);
			GC2155MIPI_write_cmos_sensor(0xb4, 0x40);
			GC2155MIPI_write_cmos_sensor(0xb5, 0xa8);
		break;
		case AWB_MODE_TUNGSTEN: //home
			GC2155MIPI_set_AWB_mode(KAL_FALSE);
			GC2155MIPI_write_cmos_sensor(0xb3, 0xa0);
			GC2155MIPI_write_cmos_sensor(0xb4, 0x45);
			GC2155MIPI_write_cmos_sensor(0xb5, 0x40);
		break;
		case AWB_MODE_FLUORESCENT:
			GC2155MIPI_set_AWB_mode(KAL_FALSE);
			GC2155MIPI_write_cmos_sensor(0xb3, 0x72);
			GC2155MIPI_write_cmos_sensor(0xb4, 0x40);
			GC2155MIPI_write_cmos_sensor(0xb5, 0x5b);
		break;	
		default:
		return FALSE;
	}
	return TRUE;
} /* GC2155MIPI_set_param_wb */

BOOL GC2155MIPI_set_param_effect(UINT16 para)
{
	kal_uint32 ret = KAL_TRUE;
	switch (para)
	{
		case MEFFECT_OFF:
			GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
			GC2155MIPI_write_cmos_sensor(0x83, 0xe0);
		break;

		case MEFFECT_SEPIA:
			GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
			GC2155MIPI_write_cmos_sensor(0x83, 0x82);
		break;  

		case MEFFECT_NEGATIVE:		
			GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
			GC2155MIPI_write_cmos_sensor(0x83, 0x01);
		break; 

		case MEFFECT_SEPIAGREEN:		
			GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
			GC2155MIPI_write_cmos_sensor(0x83, 0x52);
		break;

		case MEFFECT_SEPIABLUE:	
			GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
			GC2155MIPI_write_cmos_sensor(0x83, 0x62);
		break;

		case MEFFECT_MONO:				
			GC2155MIPI_write_cmos_sensor(0xfe, 0x00);
			GC2155MIPI_write_cmos_sensor(0x83, 0x12);
		break;

		default:
		return FALSE;
	}

	return ret;
} /* GC2155MIPI_set_param_effect */

BOOL GC2155MIPI_set_param_banding(UINT16 para)
{
    switch (para)
    {
        case AE_FLICKER_MODE_50HZ:
			
		GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
		GC2155MIPI_write_cmos_sensor(0x05 , 0x01);
		GC2155MIPI_write_cmos_sensor(0x06 , 0x56);
		GC2155MIPI_write_cmos_sensor(0x07 , 0x00);
		GC2155MIPI_write_cmos_sensor(0x08 , 0x32);
		GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
		GC2155MIPI_write_cmos_sensor(0x25 , 0x00);
		GC2155MIPI_write_cmos_sensor(0x26 , 0xfa); 
		GC2155MIPI_write_cmos_sensor(0x27 , 0x04); 
		GC2155MIPI_write_cmos_sensor(0x28 , 0xe2); //20fps 
		GC2155MIPI_write_cmos_sensor(0x29 , 0x05); 
		GC2155MIPI_write_cmos_sensor(0x2a , 0xdc); //16fps 
		GC2155MIPI_write_cmos_sensor(0x2b , 0x07); 
		GC2155MIPI_write_cmos_sensor(0x2c , 0xd0); //14fps 
		GC2155MIPI_write_cmos_sensor(0x2d , 0x0b); 
		GC2155MIPI_write_cmos_sensor(0x2e , 0xb8); //8fps
		GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
            break;

        case AE_FLICKER_MODE_60HZ:
		GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
		GC2155MIPI_write_cmos_sensor(0x05 , 0x01);
		GC2155MIPI_write_cmos_sensor(0x06 , 0x58);
		GC2155MIPI_write_cmos_sensor(0x07 , 0x00);
		GC2155MIPI_write_cmos_sensor(0x08 , 0x32);
		GC2155MIPI_write_cmos_sensor(0xfe , 0x01);
		GC2155MIPI_write_cmos_sensor(0x25 , 0x00);
		GC2155MIPI_write_cmos_sensor(0x26 , 0xd0); 
		GC2155MIPI_write_cmos_sensor(0x27 , 0x04); 
		GC2155MIPI_write_cmos_sensor(0x28 , 0xe0); //20fps 
		GC2155MIPI_write_cmos_sensor(0x29 , 0x05); 
		GC2155MIPI_write_cmos_sensor(0x2a , 0xb0); //16fps 
		GC2155MIPI_write_cmos_sensor(0x2b , 0x06); 
		GC2155MIPI_write_cmos_sensor(0x2c , 0x80); //14fps 
		GC2155MIPI_write_cmos_sensor(0x2d , 0x0b); 
		GC2155MIPI_write_cmos_sensor(0x2e , 0x60); //8fps
		GC2155MIPI_write_cmos_sensor(0xfe , 0x00);
            break;

          default:
              return FALSE;
    }

    return TRUE;
} /* GC2155MIPI_set_param_banding */

BOOL GC2155MIPI_set_param_exposure(UINT16 para)
{
	switch (para)
	{
		case AE_EV_COMP_n13:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x10);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_n10:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x15);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_n07:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x20);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_n03:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x25);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_00:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x38);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_03:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x35);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_07:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x40);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_10:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x45);
			GC2155MIPI_SET_PAGE0;
		break;
		case AE_EV_COMP_13:
			GC2155MIPI_SET_PAGE1;
			GC2155MIPI_write_cmos_sensor(0x13,0x50);
			GC2155MIPI_SET_PAGE0;
		break;
		default:
		return FALSE;
	}
	return TRUE;
} /* GC2155MIPI_set_param_exposure */

UINT32 GC2155MIPIYUVSensorSetting(FEATURE_ID iCmd, UINT32 iPara)
{
//   if( GC2155MIPI_sensor_cap_state == KAL_TRUE)
//	   return TRUE;

	switch (iCmd) {
	case FID_SCENE_MODE:	    
//	    printk("Set Scene Mode:%d\n", iPara); 
	    if (iPara == SCENE_MODE_OFF)
	    {
	        GC2155MIPI_night_mode(0); 
	    }
	    else if (iPara == SCENE_MODE_NIGHTSCENE)
	    {
               GC2155MIPI_night_mode(1); 
	    }	    
	    break; 	    
	case FID_AWB_MODE:
	    printk("Set AWB Mode:%d\n", iPara); 	    
           GC2155MIPI_set_param_wb(iPara);
	break;
	case FID_COLOR_EFFECT:
	    printk("Set Color Effect:%d\n", iPara); 	    	    
           GC2155MIPI_set_param_effect(iPara);
	break;
	case FID_AE_EV:
           printk("Set EV:%d\n", iPara); 	    	    
           GC2155MIPI_set_param_exposure(iPara);
	break;
	case FID_AE_FLICKER:
          printk("Set Flicker:%d\n", iPara); 	    	    	    
           GC2155MIPI_set_param_banding(iPara);
	break;
        case FID_AE_SCENE_MODE: 
            if (iPara == AE_MODE_OFF) {
                GC2155MIPI_set_AE_mode(KAL_FALSE);
            }
            else {
                GC2155MIPI_set_AE_mode(KAL_TRUE);
	    }
            break; 
	case FID_ZOOM_FACTOR:
	    zoom_factor = iPara; 
        break; 
	default:
	break;
	}
	return TRUE;
}   /* GC2155MIPIYUVSensorSetting */

UINT32 GC2155MIPIYUVSetVideoMode(UINT16 u2FrameRate)
{
    kal_uint8 iTemp;
    /* to fix VSYNC, to fix frame rate */
    //printk("Set YUV Video Mode \n");  

    if (u2FrameRate == 30)
    {
    }
    else if (u2FrameRate == 15)       
    {
    }
    else 
    {
        printk("Wrong frame rate setting \n");
    }
    GC2155MIPI_VEDIO_encode_mode = KAL_TRUE; 
        
    return TRUE;
}

UINT32 GC2155MIPIFeatureControl(MSDK_SENSOR_FEATURE_ENUM FeatureId,
							 UINT8 *pFeaturePara,UINT32 *pFeatureParaLen)
{
	UINT16 *pFeatureReturnPara16=(UINT16 *) pFeaturePara;
	UINT16 *pFeatureData16=(UINT16 *) pFeaturePara;
	UINT32 *pFeatureReturnPara32=(UINT32 *) pFeaturePara;
	UINT32 *pFeatureData32=(UINT32 *) pFeaturePara;
	MSDK_SENSOR_CONFIG_STRUCT *pSensorConfigData=(MSDK_SENSOR_CONFIG_STRUCT *) pFeaturePara;
	MSDK_SENSOR_REG_INFO_STRUCT *pSensorRegData=(MSDK_SENSOR_REG_INFO_STRUCT *) pFeaturePara;

	switch (FeatureId)
	{
		case SENSOR_FEATURE_GET_RESOLUTION:
			*pFeatureReturnPara16++=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH;
			*pFeatureReturnPara16=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PERIOD:
		#ifdef GC2155MIPI_Full_Preview
			*pFeatureReturnPara16++=GC2155MIPI_IMAGE_SENSOR_FULL_WIDTH;
			*pFeatureReturnPara16=GC2155MIPI_IMAGE_SENSOR_FULL_HEIGHT;
		#else	
			*pFeatureReturnPara16++=GC2155MIPI_IMAGE_SENSOR_PV_WIDTH;
			*pFeatureReturnPara16=GC2155MIPI_IMAGE_SENSOR_PV_HEIGHT;
		#endif
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_GET_PIXEL_CLOCK_FREQ:
			//*pFeatureReturnPara32 = GC2155MIPI_sensor_pclk/10;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_SET_ESHUTTER:
		break;
		case SENSOR_FEATURE_SET_NIGHTMODE:
			GC2155MIPI_night_mode((BOOL) *pFeatureData16);
		break;
		case SENSOR_FEATURE_SET_GAIN:
		case SENSOR_FEATURE_SET_FLASHLIGHT:
		break;
		case SENSOR_FEATURE_SET_ISP_MASTER_CLOCK_FREQ:
			GC2155MIPI_isp_master_clock=*pFeatureData32;
		break;
		case SENSOR_FEATURE_SET_REGISTER:
			GC2155MIPI_write_cmos_sensor(pSensorRegData->RegAddr, pSensorRegData->RegData);
		break;
		case SENSOR_FEATURE_GET_REGISTER:
			pSensorRegData->RegData = GC2155MIPI_read_cmos_sensor(pSensorRegData->RegAddr);
		break;
		case SENSOR_FEATURE_GET_CONFIG_PARA:
			memcpy(pSensorConfigData, &GC2155MIPISensorConfigData, sizeof(MSDK_SENSOR_CONFIG_STRUCT));
			*pFeatureParaLen=sizeof(MSDK_SENSOR_CONFIG_STRUCT);
		break;
		case SENSOR_FEATURE_SET_CCT_REGISTER:
		case SENSOR_FEATURE_GET_CCT_REGISTER:
		case SENSOR_FEATURE_SET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_ENG_REGISTER:
		case SENSOR_FEATURE_GET_REGISTER_DEFAULT:

		case SENSOR_FEATURE_CAMERA_PARA_TO_SENSOR:
		case SENSOR_FEATURE_SENSOR_TO_CAMERA_PARA:
		case SENSOR_FEATURE_GET_GROUP_INFO:
		case SENSOR_FEATURE_GET_ITEM_INFO:
		case SENSOR_FEATURE_SET_ITEM_INFO:
		case SENSOR_FEATURE_GET_ENG_INFO:
		break;
		case SENSOR_FEATURE_GET_GROUP_COUNT:
                        *pFeatureReturnPara32++=0;
                        *pFeatureParaLen=4;	    
		    break; 
		case SENSOR_FEATURE_GET_LENS_DRIVER_ID:
			// get the lens driver ID from EEPROM or just return LENS_DRIVER_ID_DO_NOT_CARE
			// if EEPROM does not exist in camera module.
			*pFeatureReturnPara32=LENS_DRIVER_ID_DO_NOT_CARE;
			*pFeatureParaLen=4;
		break;
		case SENSOR_FEATURE_CHECK_SENSOR_ID:
			 GC2155MIPI_GetSensorID(pFeatureData32);
			 break;
		case SENSOR_FEATURE_SET_YUV_CMD:
		       //printk("GC2155MIPI YUV sensor Setting:%d, %d \n", *pFeatureData32,  *(pFeatureData32+1));
			GC2155MIPIYUVSensorSetting((FEATURE_ID)*pFeatureData32, *(pFeatureData32+1));
		break;
		case SENSOR_FEATURE_SET_VIDEO_MODE:
		       GC2155MIPIYUVSetVideoMode(*pFeatureData16);
		       break; 
		default:
			break;			
	}
	return ERROR_NONE;
}	/* GC2155MIPIFeatureControl() */


SENSOR_FUNCTION_STRUCT	SensorFuncGC2155MIPI=
{
	GC2155MIPIOpen,
	GC2155MIPIGetInfo,
	GC2155MIPIGetResolution,
	GC2155MIPIFeatureControl,
	GC2155MIPIControl,
	GC2155MIPIClose
};

UINT32 GC2155MIPI_YUV_SensorInit(PSENSOR_FUNCTION_STRUCT *pfFunc)
{
	/* To Do : Check Sensor status here */
	if (pfFunc!=NULL)
		*pfFunc=&SensorFuncGC2155MIPI;

	return ERROR_NONE;
}	/* SensorInit() */
