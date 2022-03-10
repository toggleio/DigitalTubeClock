/**
  ******************************************************************************
  * @file           : main.h
  * @version        : v1.0
  * @author         : Xiaohei
  * @date           : 2021-11-20
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Project git: github.com/mobyw/DigitalTubeClock
  *
  ******************************************************************************
  */

#ifndef __MAIN_H_
#define __MAIN_H_

#include "STC15F2K.h"
#include "ds1302.h"
#include "api.h"

#include <intrins.h>
#include <math.h>

#define _SPACE 10
#define _C 11

sbit bell = P3 ^ 0;

sbit BTN1 = P1 ^ 0;
sbit BTN2 = P1 ^ 1;

#endif // __MAIN_H_
