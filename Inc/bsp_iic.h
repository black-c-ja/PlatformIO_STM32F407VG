#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "main.h"

#define IIC_SCL_PIN GPIO_PIN_6
#define IIC_SDA_PIN GPIO_PIN_7
#define IIC_SCL_PORT GPIOB
#define IIC_SDA_PORT GPIOB
#define IIC_SCL_H HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_SET)
#define IIC_SCL_L HAL_GPIO_WritePin(IIC_SCL_PORT, IIC_SCL_PIN, GPIO_PIN_RESET)
#define IIC_SDA_H HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_SET)
#define IIC_SDA_L HAL_GPIO_WritePin(IIC_SDA_PORT, IIC_SDA_PIN, GPIO_PIN_RESET)
#define IIC_SDA_READ HAL_GPIO_ReadPin(IIC_SDA_PORT, IIC_SDA_PIN)

void MX_IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NAck(void);
uint8_t IIC_WaitAck(void);
void IIC_SendByte(uint8_t byte);
uint8_t IIC_ReadByte(uint8_t ack);

#endif
