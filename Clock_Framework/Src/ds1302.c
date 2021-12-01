/**
  ******************************************************************************
  * @file            ds1302.c
  * @version         v1.0
  * @author          Xiaohei
  * @date            2021-11-20
  * @brief           DS1302 driver program body
  ******************************************************************************
  * @attention
  *
  * Project git: github.com/mobyw/DigitalTubeClock
  *
  ******************************************************************************
  */

#include "ds1302.h"

unsigned char time_sec = 0;     ///< 当前时间（秒）
unsigned char time_min = 0;     ///< 当前时间（分）
unsigned char time_hour = 0;    ///< 当前时间（时）

unsigned char alarm_min = 0;    ///< 闹钟时间（分）
unsigned char alarm_hour = 0;   ///< 闹钟时间（时）

unsigned char code write_addr[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8c, 0x8a}; ///< 时钟芯片寄存器写地址（秒 分 时 日 月 年 星期）
unsigned char code read_addr[] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8d, 0x8b};  ///< 时钟芯片寄存器读地址（秒 分 时 日 月 年 星期）

/**
 * @brief 初始时间设置
 * 
 * 依次为： 秒 分 时 日 月 年 星期
 * 
 * 使用 BCD 码, 如 55 即为 0x55
 */
unsigned char code init_ds[] = {0x56, 0x34, 0x12, 0x10, 0x10, 0x21, 0x07};

/// 写数据

/**
 * @brief           DS1302 写寄存器
 * 
 * @param   addr    写入寄存器地址
 * @param   dat     写入寄存器数据
 */
void ds1302_write(unsigned char addr, unsigned char dat)
{
    unsigned char i;

    rst = 1; // 复位线拉高

    for (i = 0; i < 8; i++)
    {            // 低位在前
        clk = 0; // 时钟线拉低开始写数据
        io = addr & 0x01;
        addr >>= 1; // 地址右移一位
        clk = 1;    // 时钟线拉高
    }

    for (i = 0; i < 8; i++)
    {
        clk = 0; // 时钟线拉低开始写数据
        io = dat & 0x01;
        dat >>= 1; // 数据右移一位
        clk = 1;   // 时钟线拉高
    }

    rst = 0; // 复位线合低
    clk = 0;
    io = 0;
}


/**
 * @brief           DS1302 读寄存器
 * 
 * @param   addr    读取寄存器地址
 * @return          读取寄存器数据
 */
unsigned char ds1302_read(unsigned char addr)
{
    unsigned char value, i;

    rst = 1; // 复位线拉高

    for (i = 0; i < 8; i++)
    {            // 低位在前
        clk = 0; // 时钟线拉低开始写数据
        io = addr & 0x01;
        addr >>= 1; // 地址右移一位
        clk = 1;    // 时钟线拉高
    }

    for (i = 0; i < 8; i++)
    {
        clk = 0; // 时钟线拉低开始读数据
        value >>= 1;
        if (io == 1)
            value |= 0x80;
        clk = 1; // 时钟线拉高
    }

    rst = 0; // 复位线拉低
    clk = 0;
    io = 0;

    return value; // 返回数据
}

/**
 * @brief   读取时间
 */
void read_time()
{
    time_sec = ds1302_read(read_addr[0]);  // 读秒
    time_min = ds1302_read(read_addr[1]);  // 读分
    time_hour = ds1302_read(read_addr[2]); // 读时

//  date_day    = ds1302_read(read_addr[3]);    // 读日
//  date_month  = ds1302_read(read_addr[4]);    // 读月
//  date_year   = ds1302_read(read_addr[5]);    // 读年
//  date_week   = ds1302_read(read_addr[6]);    // 读星期
}

/**
 * @brief   写入时间（时 分）
 */
void write_time()
{
    ds1302_write(0x8e, 0x00); // 打开写保护

//  ds1302_write(write_addr[0], time_sec);   // 写秒
    ds1302_write(write_addr[1], time_min);   // 写分
    ds1302_write(write_addr[2], time_hour);  // 写时
//  ds1302_write(write_addr[3], date_day);   // 写日
//  ds1302_write(write_addr[4], date_month); // 写月
//  ds1302_write(write_addr[5], date_year);  // 写年
//  ds1302_write(write_addr[6], date_week);  // 写星期

//  ds1302_write(0xc2, alarm_hour); // 写闹钟时
//  ds1302_write(0xc4, alarm_min);  // 写闹钟分

    ds1302_write(0x8e, 0x80); // 关闭写保护
}

/* 
// 读取闹钟时间
void read_alarm()
{
    alarm_hour = ds1302_read(0xc3);   // 读闹钟时
    alarm_min = ds1302_read(0xc5);    // 读闹钟分
}
*/

/**
 * @brief           写 DS1302 RAM
 * 
 * @param   addr    写入寄存器地址
 * @param   dat     写入八位数据  
 */
void ds1302_writeram(unsigned char addr, unsigned char dat)
{
    addr <<= 1;   // 地址从第二位开始
    addr &= 0xfe; // 最低位复位 写命令
    addr |= 0xc0; // 地址最高两位为 1

    ds1302_write(0x8e, 0x00);
    ds1302_write(addr, dat);
    ds1302_write(0x8e, 0x80);
}

/**
 * @brief           读 DS1302 RAM
 * 
 * @param   addr    读取寄存器地址
 * @return          读取到的八位数据
 */
unsigned char ds1302_readram(unsigned char addr)
{
    addr <<= 1;   // 地址从第二位开始
    addr |= 0x01; // 最高位置位 读命令
    addr |= 0xc0; // 地址最高两位为 1

    return (ds1302_read(addr));
}

/**
 * @brief   初始化 DS1302 时间
 */
void ds1302_init()
{
    unsigned char i;

    ds1302_init_io();

    i = ds1302_readram(30);

    if (i != 3)
    {
        i = 3;

        ds1302_writeram(30, i);
        ds1302_write(0x8e, 0x00); //打开写保护

        for (i = 0; i < 3; i++)
        {
            ds1302_write(write_addr[i], init_ds[i]); // 最高位置位 使能芯片
        }

        ds1302_write(0x8e, 0x80); // 关写保护
    }
}


/**
 * @brief   DS1302 IO 初始化
 */
void ds1302_init_io()
{
    // 第一次读写 IO 拉低
    rst = 0;
    clk = 0;
    io = 0;
}
