#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H

#include "main.h"

#define EEPROM_TYPE AT24C64
#define EEPROM_ADDR 0xA0
#define EEPROM_SIZE 8192
#define EEPROM_CHECK_DATA 0x55
#define EEPROM_CHECK_ADDR 8191

typedef enum
{
    EEPROM_OK = 0,
    EEPROM_ERROR = 1,
    EEPROM_BUSY = 2,
    EEPROM_TIMEOUT = 3
}EEPROM_StatusTypeDef;

void MX_EEPROM_Init(void);
EEPROM_StatusTypeDef EEPROM_WriteByte(uint16_t addr, uint8_t data);
EEPROM_StatusTypeDef EEPROM_ReadByte(uint16_t addr, uint8_t *data);
EEPROM_StatusTypeDef EEPROM_WriteNBytes(uint16_t addr, uint8_t *data, uint16_t len);
EEPROM_StatusTypeDef EEPROM_ReadNBytes(uint16_t addr, uint8_t *data, uint16_t len);

#endif
