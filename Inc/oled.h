//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
/******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include "main.h"
#include "stdlib.h"	    	
//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,RESET)//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,SET)

//使用4线串行接口时使用 

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,RESET)//CLK
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,RESET)//DIN
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,SET)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawBoow(uint8_t x, uint8_t y);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,int32_t num,uint8_t len);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



