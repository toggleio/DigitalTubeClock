/**
  ******************************************************************************
  * @file           : main.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Read pin
  ******************************************************************************
  */

#include <STC15F2K60S2.H>
#include <intrins.h>

#define LED_PORT P2

sbit LED_A = P3 ^ 7;
sbit LED_K = P2 ^ 0;

sbit BTN_1 = P1 ^ 0;
sbit BTN_2 = P1 ^ 1;

int main()
{
    // 此处代码仅执行一次
    
    // 配置推挽输出 增大驱动能力
    P3M0 = 0xff;
    P3M1 = 0x00;
    
    LED_A = 1;
    
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
    
    while(1)
    {
        // 此处代码循环执行
        
        if(BTN_1 == 0)
        {
            LED_K = 0;
        }
        else
        {
            LED_K = 1;
        }
        
        if(BTN_2 == 0)
        {
            LED_PORT = 0xaa; // 10101010
        }
        else
        {
            LED_PORT = 0xff;
        }
    }
}
