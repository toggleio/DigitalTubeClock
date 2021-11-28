/**
  ******************************************************************************
  * @file           : main.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Timer interupt
  ******************************************************************************
  */

#include <STC15F2K60S2.H>
#include <intrins.h>

#define LED_PORT P2

sbit LED_A = P3 ^ 7;
sbit LED_K = P2 ^ 0;

unsigned int count = 0; // 0-65535

void Timer0Init(void)   // 1ms@11.0592MHz
{
    AUXR &= 0x7F;       // 定时器时钟12T模式
    TMOD &= 0xF0;       // 设置定时器模式
    TL0 = 0x66;         // 设置定时初值
    TH0 = 0xFC;         // 设置定时初值
    TF0 = 0;            // 清除TF0标志
    TR0 = 1;            // 定时器0开始计时
    ET0 = 1;
    EA = 1;
}

int main()
{
    // 此处代码仅执行一次
    
    LED_A = 1;
    LED_K = 0;
    
    ///*
    P33 = 0;
    P34 = 0;
    P35 = 0;
    P36 = 0;
    //*/
    
    ///*
    P12 = 0;
    P13 = 0;
    P14 = 0;
    P15 = 0;
    P16 = 0;
    P17 = 0;
    P54 = 0;
    //*/
    
    Timer0Init();
    
    while(1)
    {
        // 此处代码循环执行
        
    }
}

void Timer0Interupt(void) interrupt 1
{
    count++;
    
    if(count >= 500)
    {
        count = 0;
        LED_K = !LED_K;
    }
}
