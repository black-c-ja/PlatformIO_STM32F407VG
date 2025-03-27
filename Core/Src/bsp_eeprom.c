#include "bsp_eeprom.h"
#include "bsp_iic.h"

void MX_EEPROM_Init(void)
{
    MX_IIC_Init();
    uint8_t data = EEPROM_CHECK_DATA;
    EEPROM_WriteByte(EEPROM_CHECK_ADDR, data);
    HAL_Delay(10);
    EEPROM_ReadByte(EEPROM_CHECK_ADDR, &data);
    if (data != EEPROM_CHECK_DATA) {
        printf("EEPROM check failed!\r\n");
    } else {
        printf("EEPROM check success!\r\n"); 
    }
}

EEPROM_StatusTypeDef EEPROM_WriteByte(uint16_t addr, uint8_t data)
{
    if(addr >= EEPROM_SIZE) return EEPROM_ERROR;
    
    IIC_Start();
    IIC_SendByte(EEPROM_ADDR);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(addr >> 8);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(addr);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(data);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_Stop();
    return EEPROM_OK;
}

EEPROM_StatusTypeDef EEPROM_ReadByte(uint16_t addr, uint8_t *data)
{
    if(addr >= EEPROM_SIZE) return EEPROM_ERROR;

    IIC_Start();
    IIC_SendByte(EEPROM_ADDR);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(addr >> 8);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(addr);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_Start();
    IIC_SendByte(EEPROM_ADDR + 1);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    *data = IIC_ReadByte(0);
    IIC_Stop();
    return EEPROM_OK;
}

EEPROM_StatusTypeDef EEPROM_WriteNBytes(uint16_t addr, uint8_t *data, uint16_t len)
{
    if(addr + len > EEPROM_SIZE) return EEPROM_ERROR;

    uint16_t i;
    for (i = 0; i < len; i++) {
        if (EEPROM_WriteByte(addr + i, data[i]) == EEPROM_ERROR)
            return EEPROM_ERROR;
        HAL_Delay(10);
    }
    return EEPROM_OK;
}

EEPROM_StatusTypeDef EEPROM_ReadNBytes(uint16_t addr, uint8_t *data, uint16_t len)
{
    if(addr + len > EEPROM_SIZE) return EEPROM_ERROR;

    IIC_Start();
    IIC_SendByte(EEPROM_ADDR);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(addr >> 8);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_SendByte(addr);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    IIC_Start();
    IIC_SendByte(EEPROM_ADDR + 1);
    if (IIC_WaitAck() == 1) {
        IIC_Stop();
        return EEPROM_ERROR;
    }
    uint16_t i;
    for (i = 0; i < len - 1; i++) {
        data[i] = IIC_ReadByte(1);
    }
    data[i] = IIC_ReadByte(0);
    IIC_Stop();
    
    return EEPROM_OK;
}
