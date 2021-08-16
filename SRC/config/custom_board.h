/*The file is for my own custom board*/

#ifndef CUSTOM_BOARD_H
#define CUSTOM_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"


//不要NRF的LED和按键配置
#define LED_NUMBER 0
#define BUTTONS_NUMBER 0


//State indicatior LED gpio
#define STATE_LED NRF_GPIO_PIN_MAP(0, 25)
#define STATE_LED_ACTIVE 1 //当需要亮灯时该pin输出高，使板子上对应的nmos导通

//矩阵脚，板子上的30PINFPC母座从左往右数第四个开始定义为第一个，共21个
#define MTXP1 NRF_GPIO_PIN_MAP(0, 10)
#define MTXP2 NRF_GPIO_PIN_MAP(0, 9)
#define MTXP3 NRF_GPIO_PIN_MAP(1, 6)
#define MTXP4 NRF_GPIO_PIN_MAP(1, 4)
#define MTXP5 NRF_GPIO_PIN_MAP(0, 13)
#define MTXP6 NRF_GPIO_PIN_MAP(0, 24)
#define MTXP7 NRF_GPIO_PIN_MAP(1, 2)
#define MTXP8 NRF_GPIO_PIN_MAP(1, 0)
#define MTXP9 NRF_GPIO_PIN_MAP(0, 22)
#define MTXP10 NRF_GPIO_PIN_MAP(0, 20)
#define MTXP11 NRF_GPIO_PIN_MAP(0, 17)
#define MTXP12 NRF_GPIO_PIN_MAP(0, 15)
#define MTXP13 NRF_GPIO_PIN_MAP(0, 7)
#define MTXP14 NRF_GPIO_PIN_MAP(0, 12)
#define MTXP15 NRF_GPIO_PIN_MAP(0, 4)
#define MTXP16 NRF_GPIO_PIN_MAP(0, 8)
#define MTXP17 NRF_GPIO_PIN_MAP(0, 26)
#define MTXP18 NRF_GPIO_PIN_MAP(1, 9)
#define MTXP19 NRF_GPIO_PIN_MAP(0, 5)
#define MTXP20 NRF_GPIO_PIN_MAP(0, 30)
#define MTXP21 NRF_GPIO_PIN_MAP(0, 31)

//读编码器用
#define ENCPA NRF_GPIO_PIN_MAP(0, 29)
#define ENCPB NRF_GPIO_PIN_MAP(0, 2)
#define ENCPS NRF_GPIO_PIN_MAP(1, 5)

//输出PWM信号控制单色背光灯引脚
#define BLCP NRF_GPIO_PIN_MAP(0, 28)

//用哪个Analog通道读电池电压
#define BATTERY_PIN NRF_SAADC_INPUT_AIN1

#ifdef __cplusplus
}
#endif

#endif