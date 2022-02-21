#include "eeprom.h"
#include <stdio.h>
#include"gpio.h"
#include "usart.h"
#include "i2c.h"
#include "timer.h"

//EEPROM写一个字节
void EEPROM_WriteByte(uint16_t addr,uint8_t data){
    //开始信号
    I2C_Start();
    
    //发送要写的设备地址
    I2C_SendBYTE(EEPROM_DEV_ADDR | EEPROM_WR);
    
    //要写的数据地址
    if(EEPROM_WORD_ADDR_SIZE==0x08){
        I2C_SendBYTE((uint8_t)(addr & 0x00FF));
    }
    else{
        I2C_SendBYTE((uint8_t)(addr>>8));
        I2C_SendBYTE((uint8_t)(addr & 0x00FF));
    }
    
    //写数据
    I2C_SendBYTE(data);
    
    //停止信号
    I2C_Stop();
}

//读EEPROM的一个字节
void EEPROM_ReadByte(uint16_t addr,uint8_t *pdata){
    //开始信号
    I2C_Start();
    
    //设备地址
    I2C_SendBYTE(EEPROM_DEV_ADDR | EEPROM_WR);
    
    //读取的数据的地址
    if(EEPROM_WORD_ADDR_SIZE==0x08){
        I2C_SendBYTE((uint8_t)(addr & 0x00FF));
    }else{
        I2C_SendBYTE((uint8_t)(addr>>8));
        I2C_SendBYTE((uint8_t)(addr & 0x00ff));
    }
    
    //开始传输
    I2C_Start();
    
    //设备的地址且设置为读模式
    I2C_SendBYTE(EEPROM_DEV_ADDR | EEPROM_RD);
    
    //读取数据，因为读取数据不需要回应信号
    *pdata=I2C_ReadBYTE(NACK);
    
    //结束
    I2C_Stop();
}

//写N个字节
void EEPROM_Write_NByte(uint16_t addr , uint8_t *pdata,uint16_t size){
    uint16_t i=0;
    
    for(i=0;i<size;i++){
        EEPROM_WriteByte(addr,pdata[i]);
        addr++;
        HAL_Delay(10);//每次期间需要延迟5ms，这里延迟10ms防止出错
    }
}
//连续读数据
void EEPROM_Read_NByte(uint16_t addr,uint8_t *pdata,uint16_t size){
    
    uint16_t i=0;
    for(i=0;i<size;i++){
        EEPROM_ReadByte(addr,&pdata[i]);
        addr++;
    }
}
