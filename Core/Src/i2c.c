#include <stdio.h>
#include "i2c.h"
#include "main.h"
#include "timer.h"
#include "usart.h"

#define I2C_Delay()     us_timer_delay(5)

//初始化模拟I2C的引脚为输出状态且SCL/SDA都初始为高电平
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

//配置SDA的引脚为输出。

static void I2C_SDA_OUT(void){
    GPIO_InitTypeDef GPIO_InitStruct={0};
    
    GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDA_PIN;
    
    HAL_GPIO_Init(SDA_PORT,&GPIO_InitStruct);
}
//配置SDA引脚为输入模式
static void I2C_SDA_IN(void){
    GPIO_InitTypeDef GPIO_InitStruct={0};
    GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin=SDA_PIN;
    
    HAL_GPIO_Init(SDA_PORT,&GPIO_InitStruct);
}
//开始信号
void I2C_Start(void){
    I2C_SDA_OUT();//设置为输出模式
    
    SCL_H();//时钟线输出为高
    I2C_Delay();//延迟5ms
    
    SDA_H();//数据线输出为高
    I2C_Delay();
    
    SDA_L();//数据线输出低，由高到底表示开始信号
    I2C_Delay();
}
//结束信号
void I2C_Stop(void){
    I2C_SDA_OUT();//输出模式
    
    SDA_L();
    I2C_Delay();
    
    SCL_H();
    I2C_Delay();
    
    SDA_H();
    I2C_Delay();
}
//发出应答信号函数
void I2C_ACK(void){
    I2C_SDA_OUT();//设置为接收模式
    
    SCL_L();//时钟线输出为低
    I2C_Delay();
    
    SDA_L();
    I2C_Delay();
    
    SCL_H();
    I2C_Delay();
    
    SCL_L();
    I2C_Delay();
    
}
//发出非应答信号
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

//等待从机的应答信号
uint8_t I2C_GetACK(void){
    uint8_t time=0;
    
    I2C_SDA_IN();//设置为输入模式
    
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

//发送一个字节的数据
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
    SCL_L();//发完数据
    I2C_Delay();
    I2C_GetACK();
}

//读取一个字节的数据

uint8_t I2C_ReadBYTE(uint8_t ack){
    uint8_t cnt=0;
    uint8_t data=0xFF;//确定data的值
    
    SCL_L();
    I2C_Delay();
    
    for(cnt=0;cnt<8;cnt++){
        SCL_H();  //SCL高，读取数据
        I2C_Delay();
        
        data<<=1;
        
        if(SDA_INPUT()){
            data |=0x01;
        }
        SCL_L();
        I2C_Delay();
    }
    //发送应答信号，低为应答，高为非应答
    if(ack==0){
        I2C_ACK();
    }
    else{
        I2C_NACK();
    }
    return data;
}



