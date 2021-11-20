/**
  ******************************************************************************
  * @file           : api.h
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Header for api.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Project git: github.com/mobyw/DigitalTubeClock
  *
  ******************************************************************************
  */

#ifndef __API_H_
#define __API_H_

#include "STC15F2K.h"
#include "ds1302.h"

#include <intrins.h>
#include <math.h>

#define SEG_PORT    P2

#define ENABLE      1
#define DISABLE     0

#define ALLOFF      0xff

// ADC 相关配置
#define ADC_POWER   0x80            // ADC 电源控制位
#define ADC_FLAG    0x10            // ADC 完成标志
#define ADC_START   0x08            // ADC 起始控制位
#define ADC_SPEEDLL 0x00            // ADC 速度 540 个时钟
#define ADC_SPEEDL  0x20            // ADC 速度 360 个时钟
#define ADC_SPEEDH  0x40            // ADC 速度 180 个时钟
#define ADC_SPEEDHH 0x60            // ADC 速度  90 个时钟

#define set_bit(x,y)  x|=(1<<y)
#define reset_bit(x,y)  x&=~(1<<y)

extern unsigned char num_seg_sel[];
extern unsigned char led_array[8];
extern unsigned char display_array[4];
extern char blink;

sbit SEG_COM1 = P3 ^ 3;
sbit SEG_COM2 = P3 ^ 4;
sbit SEG_COM3 = P3 ^ 5;
sbit SEG_COM4 = P3 ^ 6;
sbit LED_COM1 = P3 ^ 7;
sbit LED_COM2 = P1 ^ 2;
sbit LED_COM3 = P1 ^ 3;
sbit LED_COM4 = P1 ^ 4;
sbit LED_COM5 = P1 ^ 5;
sbit LED_COM6 = P1 ^ 6;
sbit LED_COM7 = P1 ^ 7;
sbit LED_COM8 = P5 ^ 4;

// 短暂非精确延时
void delay(unsigned int t);

// BCD 转换 HEX
unsigned char bcd2hex(unsigned char bcd);

// 定时器初始化
void timerInit();

// 系统初始化
void systemInit();

// 清除 LED 显示
void ledAllOff();

// 点亮全部 LED
void ledAllOn();

// 指定 LED 打开显示
// index: LED 索引 0-59
void setLed(unsigned char index);

// 指定 LED 关闭显示
// index: LED 索引 0-59
void resetLed(unsigned char index);
    
// 数码管与 LED 显示
void display();

// 使能 ADC 转换
void enableAdc();

// 禁用 ADC 转换
void disableAdc();

// 读取 ADC 通道的值
// ch: ADC 通道序号
unsigned int getAdcResult(unsigned char ch);

// 读取温度
unsigned int getTemperature(void);

// 读取光敏电阻分压
unsigned int getLight(void);

#endif // __API_H_
