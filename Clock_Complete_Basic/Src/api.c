/**
  ******************************************************************************
  * @file           : api.c
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Api program body
  ******************************************************************************
  * @attention
  *
  * Project git: github.com/mobyw/DigitalTubeClock
  *
  ******************************************************************************
  */

#include "api.h"

// 数码管段选码
// 0 1 2 3 4 5 6 7 8 9 SPACE C
unsigned char num_seg_sel[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xc6};

// 周围 LED 显示数组
unsigned char led_array[8] = {0};

// 数码管显示内容
// 从左到右下标分别为 0 1 2 3
unsigned char display_array[4] = {0};

// 冒号闪烁控制标志
char blink = 1;

// 存储 ADC 转换结果
unsigned int adc_result = 0;

// 短暂非精确延时
void delay(unsigned int t)
{
    unsigned char i;
    while (t--)
        for (i = 0; i < 100; i++)
            ;
}

// BCD 转 HEX
unsigned char bcd2hex(unsigned char bcd)
{
    unsigned char temp;

    temp = bcd & 0x0f;
    bcd = bcd >> 4;
    bcd %= 0xf;

    return (bcd * 10 + temp);
}

// 定时器初始化
void timerInit()
{
    TMOD = 0x01;
    TL0 = (65536 - 20000) / 256; // 设置定时初值
    TH0 = (65536 - 20000) % 256; // 设置定时初值
    ET0 = 1;
    TR0 = 1;
    EA = 1;
}

// 系统初始化
void systemInit()
{
    // 定时器初始化
    timerInit();

    // 外设初始化
    ds1302_init();

    // 端口初始化
    P1M0 = 0xff;
    P1M1 = 0x00;
    P2M0 = 0xff;
    P2M1 = 0x00;
    P3M0 = 0xf8;
    P3M1 = 0x00;
    P5M0 = 0x10;
    P5M1 = 0x00;
}

// 清除 LED 显示
void ledAllOff()
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        // 全部熄灭
        led_array[i] = 0xff;
    }
}

// 点亮全部 LED
void ledAllOn()
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        // 全部打开
        led_array[i] = 0x00;
    }
}

// 指定 LED 打开显示
// index: LED 索引 0-59
void setLed(unsigned char index)
{
    index = (index + 59) % 60;
    reset_bit(led_array[index / 8], index % 8);
}

// 指定 LED 关闭显示
// index: LED 索引 0-59
void resetLed(unsigned char index)
{
    index = (index + 59) % 60;
    set_bit(led_array[index / 8], index % 8);
}

// 数码管与 LED 显示
void display()
{
    // -- 数码管显示时间
    // 第一位
    SEG_COM1 = ENABLE;
    SEG_PORT = num_seg_sel[display_array[0]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM1 = DISABLE;

    // 第二位
    // 判断冒号显示时最高位复位
    SEG_COM2 = ENABLE;
    if (blink == 0)
        SEG_PORT = num_seg_sel[display_array[1]] - 0x80;
    if (blink == 1)
        SEG_PORT = num_seg_sel[display_array[1]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM2 = DISABLE;

    // 第三位
    SEG_COM3 = ENABLE;
    SEG_PORT = num_seg_sel[display_array[2]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM3 = DISABLE;

    // 第四位
    SEG_COM4 = ENABLE;
    SEG_PORT = num_seg_sel[display_array[3]];
    delay(2);
    SEG_PORT = ALLOFF;
    SEG_COM4 = DISABLE;

    // -- LED 显示
    // 1-8
    LED_COM1 = ENABLE;
    SEG_PORT = led_array[0];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM1 = DISABLE;

    // 9-16
    LED_COM2 = ENABLE;
    SEG_PORT = led_array[1];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM2 = DISABLE;

    // 17-24
    LED_COM3 = ENABLE;
    SEG_PORT = led_array[2];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM3 = DISABLE;

    // 25-32
    LED_COM4 = ENABLE;
    SEG_PORT = led_array[3];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM4 = DISABLE;

    // 33-40
    LED_COM5 = ENABLE;
    SEG_PORT = led_array[4];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM5 = DISABLE;

    // 41-48
    LED_COM6 = ENABLE;
    SEG_PORT = led_array[5];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM6 = DISABLE;

    // 49-56
    LED_COM7 = ENABLE;
    SEG_PORT = led_array[6];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM7 = DISABLE;

    // 57-60
    LED_COM8 = ENABLE;
    SEG_PORT = led_array[7];
    delay(2);
    SEG_PORT = ALLOFF;
    LED_COM8 = DISABLE;

    // 消隐 关闭所有显示
    SEG_COM1 = SEG_COM2 = SEG_COM3 = SEG_COM4 = ENABLE;
    LED_COM1 = LED_COM2 = LED_COM3 = LED_COM4 = ENABLE;
    LED_COM5 = LED_COM6 = LED_COM7 = LED_COM8 = ENABLE;
    SEG_PORT = ALLOFF;
    SEG_COM1 = SEG_COM2 = SEG_COM3 = SEG_COM4 = DISABLE;
    LED_COM1 = LED_COM2 = LED_COM3 = LED_COM4 = DISABLE;
    LED_COM5 = LED_COM6 = LED_COM7 = LED_COM8 = DISABLE;
}

// 使能 ADC 转换
void enableAdc()
{
    P1ASF = 0x03;
}

// 禁用 ADC 转换
void disableAdc()
{
    P1ASF = 0x00;
}

// 读取 ADC 通道的值
// ch: ADC 通道序号
unsigned int getAdcResult(unsigned char ch)
{
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
    _nop_(); // Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    _nop_(); // Must wait before inquiry
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
        ;                   // Wait complete flag
    ADC_CONTR &= ~ADC_FLAG; // Close ADC

    return (ADC_RES * 4 + ADC_RESL); // Return ADC result.
}

// 读取温度
unsigned int getTemperature(void)
{
    adc_result = getAdcResult(0);
    return (unsigned int)((3950.0 / (11.33657 + log(6.04 * (float)adc_result / (1024.0 - (float)adc_result))) - 278.15) * 100);
}

// 读取光敏电阻分压
unsigned int getLight(void)
{
    adc_result = getAdcResult(1);

    if (adc_result < 100)
    {
        return 8;
    }
    if (adc_result > 100 && adc_result < 300)
    {
        return 10;
    }
    if (adc_result > 300 && adc_result < 500)
    {
        return 20;
    }
    if (adc_result > 500 && adc_result < 600)
    {
        return 40;
    }
    if (adc_result > 600 && adc_result < 700)
    {
        return 60;
    }
    if (adc_result > 700 && adc_result < 800)
    {
        return 90;
    }
    if (adc_result > 800 && adc_result < 900)
    {
        return 120;
    }
    if (adc_result > 900)
    {
        return 150;
    }

    return 8;
}
