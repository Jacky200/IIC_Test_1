#ifndef _EEPROM_H
#define _EEPROM_H
#include "stm32f1xx_hal.h"

#define EEPROM_DEV_ADDR     (0xA0)
#define EEPROM_WR           (0x00)
#define EEPROM_RD           (0x01)

#define EEPROM_WORD_ADDR_SIZE   (0x08)


void EEPROM_WriteByte(uint16_t addr,uint8_t data);
void EEPROM_ReadByte(uint16_t addr,uint8_t *pdata);
void EEPROM_Write_NByte(uint16_t addr , uint8_t *pdata,uint16_t size);
void EEPROM_Read_NByte(uint16_t addr,uint8_t *pdata,uint16_t size);
#endif
