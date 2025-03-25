#include "bsp_iic.h"

void MX_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin =  IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(IIC_SCL_PORT, &GPIO_InitStruct);

    IIC_SDA_H;
    IIC_SCL_H;
}

static void IIC_Delay(void)
{
    volatile int i = 30;
    while (i--)
        __NOP();
}

void IIC_Start(void)
{
    IIC_SDA_H;
    IIC_SCL_H;
    IIC_Delay();
    IIC_SDA_L;
    IIC_Delay();
    IIC_SCL_L;
}

void IIC_Stop(void)
{
    IIC_SDA_L;
    IIC_SCL_H;
    IIC_Delay();
    IIC_SDA_H;
    IIC_Delay();
}

void IIC_Ack(void)
{
    IIC_SDA_L;
    IIC_Delay();
    IIC_SCL_H;
    IIC_Delay();
    IIC_SCL_L;
    IIC_Delay();
    IIC_SDA_H;
}

void IIC_NAck(void)
{
    IIC_SDA_H;
    IIC_Delay();
    IIC_SCL_H;
    IIC_Delay();
    IIC_SCL_L;
    IIC_Delay();
}

uint8_t IIC_WaitAck(void)
{
    IIC_SDA_H;
    IIC_Delay();
    IIC_SCL_H;
    IIC_Delay();
    if (IIC_SDA_READ)
    {
        IIC_SCL_L;
        return 1;
    }
    IIC_SCL_L;
    return 0;
}

void IIC_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--)
    {
        if (byte & 0x80)
            IIC_SDA_H;
        else
            IIC_SDA_L;
        byte <<= 1;
        IIC_Delay();
        IIC_SCL_H;
        IIC_Delay();
        IIC_SCL_L;
    }
}

uint8_t IIC_ReadByte(uint8_t ack)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    IIC_SDA_H;
    while (i--)
    {
        byte <<= 1;
        IIC_SCL_H;
        IIC_Delay();
        if (IIC_SDA_READ)
            byte++;
        IIC_SCL_L;
        IIC_Delay();
    }
    if (ack)
        IIC_Ack();
    else
        IIC_NAck();
    return byte;
}
