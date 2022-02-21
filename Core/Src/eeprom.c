#include "eeprom.h"
#include <stdio.h>
#include"gpio.h"
#include "usart.h"
#include "i2c.h"
#include "timer.h"

//EEPROMдһ���ֽ�
void EEPROM_WriteByte(uint16_t addr,uint8_t data){
    //��ʼ�ź�
    I2C_Start();
    
    //����Ҫд���豸��ַ
    I2C_SendBYTE(EEPROM_DEV_ADDR | EEPROM_WR);
    
    //Ҫд�����ݵ�ַ
    if(EEPROM_WORD_ADDR_SIZE==0x08){
        I2C_SendBYTE((uint8_t)(addr & 0x00FF));
    }
    else{
        I2C_SendBYTE((uint8_t)(addr>>8));
        I2C_SendBYTE((uint8_t)(addr & 0x00FF));
    }
    
    //д����
    I2C_SendBYTE(data);
    
    //ֹͣ�ź�
    I2C_Stop();
}

//��EEPROM��һ���ֽ�
void EEPROM_ReadByte(uint16_t addr,uint8_t *pdata){
    //��ʼ�ź�
    I2C_Start();
    
    //�豸��ַ
    I2C_SendBYTE(EEPROM_DEV_ADDR | EEPROM_WR);
    
    //��ȡ�����ݵĵ�ַ
    if(EEPROM_WORD_ADDR_SIZE==0x08){
        I2C_SendBYTE((uint8_t)(addr & 0x00FF));
    }else{
        I2C_SendBYTE((uint8_t)(addr>>8));
        I2C_SendBYTE((uint8_t)(addr & 0x00ff));
    }
    
    //��ʼ����
    I2C_Start();
    
    //�豸�ĵ�ַ������Ϊ��ģʽ
    I2C_SendBYTE(EEPROM_DEV_ADDR | EEPROM_RD);
    
    //��ȡ���ݣ���Ϊ��ȡ���ݲ���Ҫ��Ӧ�ź�
    *pdata=I2C_ReadBYTE(NACK);
    
    //����
    I2C_Stop();
}

//дN���ֽ�
void EEPROM_Write_NByte(uint16_t addr , uint8_t *pdata,uint16_t size){
    uint16_t i=0;
    
    for(i=0;i<size;i++){
        EEPROM_WriteByte(addr,pdata[i]);
        addr++;
        HAL_Delay(10);//ÿ���ڼ���Ҫ�ӳ�5ms�������ӳ�10ms��ֹ����
    }
}
//����������
void EEPROM_Read_NByte(uint16_t addr,uint8_t *pdata,uint16_t size){
    
    uint16_t i=0;
    for(i=0;i<size;i++){
        EEPROM_ReadByte(addr,&pdata[i]);
        addr++;
    }
}
