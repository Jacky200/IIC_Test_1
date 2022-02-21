#ifndef _I2C_H
#define _I2C_H
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#define ACK     (0)
#define NACK     (1)

#define SCL_PIN     GPIO_PIN_6
#define SCL_PORT    GPIOB
#define SCL_PIN_CLK_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define SDA_PIN     GPIO_PIN_7
#define SDA_PORT    GPIOB
#define SDA_PIN_CLK_EN()    __HAL_RCC_GPIOB_CLK_ENABLE()

#define SCL_H()     HAL_GPIO_WritePin(SCL_PORT,SCL_PIN,GPIO_PIN_SET)
#define SCL_L()     HAL_GPIO_WritePin(SCL_PORT,SCL_PIN,GPIO_PIN_RESET)
#define SCL_INPUT() HAL_GPIO_ReadPin(SCL_PORT,SCL_PIN)      

#define SDA_H()     HAL_GPIO_WritePin(SDA_PORT,SDA_PIN,GPIO_PIN_SET)
#define SDA_L()     HAL_GPIO_WritePin(SDA_PORT,SDA_PIN,GPIO_PIN_RESET)
#define SDA_INPUT() HAL_GPIO_ReadPin(SDA_PORT,SDA_PIN)  

void I2C_Init(void);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_ACK(void);
void I2C_NACK(void);
uint8_t I2C_GetACK(void);
void I2C_SendBYTE(uint8_t data);
uint8_t I2C_ReadBYTE(uint8_t ack);

#endif
