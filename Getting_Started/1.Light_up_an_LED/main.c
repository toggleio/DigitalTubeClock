/**
  ******************************************************************************
  * @file           : main.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Light up an LED
  ******************************************************************************
  */

#include <STC15F2K60S2.H>
#include <intrins.h>

sbit LED_A = P3 ^ 7;
sbit LED_K = P2 ^ 0;

void Delay1s() // @11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 43;
    j = 6;
    k = 203;
    do
    {
        do
        {
            while (--k);
        } while (--j);
    } while (--i);
}

int main()
{
    // 此处代码仅执行一次
    
    LED_A = 1;
    
    /*
    P33 = 0;
    P34 = 0;
    P35 = 0;
    P36 = 0;
    //*/
    
    /*
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
        
        LED_K = 0;
        Delay1s();
        
        LED_K = 1;
        Delay1s();
    }
}
