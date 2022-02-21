#include <stdio.h>
#include "i2c.h"
#include "main.h"
#include "timer.h"
#include "usart.h"

#define I2C_Delay()     us_timer_delay(5)

//��ʼ��ģ��I2C������Ϊ���״̬��SCL/SDA����ʼΪ�ߵ�ƽ
void I2C_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct={0};
    SCL_PIN_CLK_EN();
    SDA_PIN_CLK_EN();
    
    GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull=GPIO_NOPULL;
    GPIO_InitStruct.Pin=SCL_PIN;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(SCL_PORT,&GPIO_InitStruct);
    
    GPIO_InitStruct.Pin=SDA_PIN;
    HAL_GPIO_Init(SDA_PORT,&GPIO_InitStruct);
    
    SCL_H();
    SDA_H();
}

//����SDA������Ϊ�����

static void I2C_SDA_OUT(void){
    GPIO_InitTypeDef GPIO_InitStruct={0};
    
    GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDA_PIN;
    
    HAL_GPIO_Init(SDA_PORT,&GPIO_InitStruct);
}
//����SDA����Ϊ����ģʽ
static void I2C_SDA_IN(void){
    GPIO_InitTypeDef GPIO_InitStruct={0};
    GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDA_PIN;
    
    HAL_GPIO_Init(SDA_PORT,&GPIO_InitStruct);
}
//��ʼ�ź�
void I2C_Start(void){
    I2C_SDA_OUT();//����Ϊ���ģʽ
    
    SCL_H();//ʱ�������Ϊ��
    I2C_Delay();//�ӳ�5ms
    
    SDA_H();//���������Ϊ��
    I2C_Delay();
    
    SDA_L();//����������ͣ��ɸߵ��ױ�ʾ��ʼ�ź�
    I2C_Delay();
}
//�����ź�
void I2C_Stop(void){
    I2C_SDA_OUT();//���ģʽ
    
    SDA_L();
    I2C_Delay();
    
    SCL_H();
    I2C_Delay();
    
    SDA_H();
    I2C_Delay();
}
//����Ӧ���źź���
void I2C_ACK(void){
    I2C_SDA_OUT();//����Ϊ����ģʽ
    
    SCL_L();//ʱ�������Ϊ��
    I2C_Delay();
    
    SDA_L();
    I2C_Delay();
    
    SCL_H();
    I2C_Delay();
    
    SCL_L();
    I2C_Delay();
    
}
//������Ӧ���ź�
void I2C_NACK(void){
    I2C_SDA_OUT();
    
    SCL_L();
    I2C_Delay();
    
    SDA_H();
    I2C_Delay();
    
    SCL_H();
    I2C_Delay();
    
    SCL_L();
    I2C_Delay();
}

//�ȴ��ӻ���Ӧ���ź�
uint8_t I2C_GetACK(void){
    uint8_t time=0;
    
    I2C_SDA_IN();//����Ϊ����ģʽ
    
    SCL_L();
    I2C_Delay();
    
    SDA_H();
    I2C_Delay();
    
    SCL_H();
    I2C_Delay();
    
    while(SDA_INPUT()){
        time++;
        if(time>250){
            SCL_L();
            return 1;
        }
    }
    SCL_L();
    return 0;
}

//����һ���ֽڵ�����
void I2C_SendBYTE(uint8_t data){
    uint8_t cnt=0;
    
    I2C_SDA_OUT();
    
    for(cnt=0;cnt<8;cnt++){
        SCL_L();
        I2C_Delay();
        
        if(data & 0x80){
            SDA_H();
        }else{
            SDA_L();
        }
        data<<=1;
        
        SCL_H();
        I2C_Delay();
    }
    SCL_L();//��������
    I2C_Delay();
    I2C_GetACK();
}

//��ȡһ���ֽڵ�����

uint8_t I2C_ReadBYTE(uint8_t ack){
    uint8_t cnt=0;
    uint8_t data=0xFF;//ȷ��data��ֵ
    
    SCL_L();
    I2C_Delay();
    
    for(cnt=0;cnt<8;cnt++){
        SCL_H();  //SCL�ߣ���ȡ����
        I2C_Delay();
        
        data<<=1;
        
        if(SDA_INPUT()){
            data |=0x01;
        }
        SCL_L();
        I2C_Delay();
    }
    //����Ӧ���źţ���ΪӦ�𣬸�Ϊ��Ӧ��
    if(ack==0){
        I2C_ACK();
    }
    else{
        I2C_NACK();
    }
    return data;
}



