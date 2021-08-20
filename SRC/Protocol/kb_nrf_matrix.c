#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "matrix.h"
#include "nrf_gpio.h"
#include "wait.h"
#include "config.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

static matrix_row_t matrix[MATRIX_ROWS];
static void select_row(uint8_t row);
static void unselect_row(uint8_t row);
static void unselect_rows(void);


/*
矩阵的去抖方法：
每隔特定时间间隔(DEBOUNCE_SAMPLE_INTERVAL)对进行一次检测，并记录结果
当连续足够次数(DEBOUNCE_SAMPLE_TIMES)中都检测到该按键触发，则认为按键触发
通过定长队列记录按键历史状态
*/

/*定长队列定义与相关方法(初始化、新元素进入、按键状态输出)*/
/*队列定义*/
typedef struct fixLenQueue
{   
    matrix_row_t rowFrames[DEBOUNCE_SAMPLE_TIMES];
    uint8_t pointer;
}fixLenQueue;

/*队列初始化*/
static void queInit(fixLenQueue *queuePointer)
{
    for(uint8_t i = 0; i < DEBOUNCE_SAMPLE_TIMES; i++){
        queuePointer->rowFrames[i] = 0;
    }
    queuePointer->pointer = 0;
}

/*新扫描帧替换*/
static matrix_row_t queInsert(fixLenQueue *queuePointer, matrix_row_t row)
{
    matrix_row_t drop = queuePointer->rowFrames[queuePointer->pointer];
    queuePointer->rowFrames[queuePointer->pointer] = row;
    queuePointer->pointer++;
    if(queuePointer->pointer==DEBOUNCE_SAMPLE_TIMES){
        queuePointer->pointer = 0;
    }
    return drop;
}

/*多重采样比对，返回去抖结果*/
static matrix_row_t queResult(fixLenQueue queue)
{
    matrix_row_t result = queue.rowFrames[0];
    for(uint8_t i = 1; i < DEBOUNCE_SAMPLE_TIMES; i++){
        result = result & queue.rowFrames[i];
    }
    return result;
}

/*定义变量*/
static matrix_row_t matrix[MATRIX_ROWS];
static fixLenQueue rowQueues[MATRIX_ROWS];


inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}


#define LED_ON()    do { /*palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)
#define LED_OFF()   do { /*palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)
#define LED_TGL()   do { /*palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13);*/} while (0)

/*Column pins init*/
/*将列引脚设置为输入*/
static void init_cols(void)
{
    for(uint8_t idx=0; idx<MATRIX_COLS; idx++){
        if(DIODES_DIRECTION == 0){//二极管方向为从列到行，则开启列的上拉电阻，读取到低电平意味着触发
            nrf_gpio_cfg_input((uint32_t)col_pins[idx], NRF_GPIO_PIN_PULLUP);
        }
        else{//二极管方向为从行到列，则开启列的下拉电阻，读取到高电平意味着触发
            nrf_gpio_cfg_input((uint32_t)col_pins[idx], NRF_GPIO_PIN_PULLDOWN);
        }
    }
}
/*Row pins init*/
/*将行引脚设置为输出*/
static void init_rows(void)
{
    for(uint8_t idx=0; idx<MATRIX_ROWS; idx++){
        if(DIODES_DIRECTION == 0){//当二极管方向为从列到行时
            nrf_gpio_cfg_output((uint32_t)row_pins[idx]);
            nrf_gpio_pin_set((uint32_t)row_pins[idx]);/*初始状态为高电平*/
        }
        else{
            nrf_gpio_cfg_output((uint32_t)row_pins[idx]);
            nrf_gpio_pin_clear((uint32_t)row_pins[idx]);/*初始状态为低电平*/
        }
    }
}

/*矩阵初始化*/
void matrix_init(void)
{
    #ifdef KB_NRF_DEBUG
    NRF_LOG_INFO("matrix init");
    #endif
    init_cols();
    init_rows();
    for(uint8_t i = 0; i < MATRIX_ROWS; i++){
        matrix[i] = 0;
        queInit(&rowQueues[i]);
    }
}

/*读取列状态*/
static matrix_row_t read_cols(void)
{
    matrix_row_t result = 0;
    for(uint8_t i = 0; i<MATRIX_COLS; i++){
        uint32_t colStat = nrf_gpio_pin_read(((uint32_t)col_pins[i]));
        if(DIODES_DIRECTION == 0){ //二极管方向为列到行
            if(colStat==0){ //读到低电平
                result |= 1 << i;
            }
        }
        else{ //二极管为行到列
            if(colStat==1){ //读到高电平
                result |= 1 << i;
            }
        }
    }
    return result;
}

/*行电平操作*/
static void select_row(uint8_t row)
{
    if(DIODES_DIRECTION == 0){//列到行时，对特定行进行扫描时将改行置低
        nrf_gpio_pin_clear((uint32_t)row_pins[row]);
    }
    else{
        nrf_gpio_pin_set((uint32_t)row_pins[row]);
    }
}

static void unselect_row(uint8_t row) //反向操作
{
    if(DIODES_DIRECTION == 0){
        nrf_gpio_pin_set((uint32_t)row_pins[row]);
    }
    else{
        nrf_gpio_pin_clear((uint32_t)row_pins[row]);
    }
}

/*读矩阵*/
uint8_t matrix_scan(void)
{
    for(uint8_t i = 0; i < MATRIX_ROWS; i++){
        select_row(i);
        wait_us(25);// wait until value stable
        matrix_row_t cols = read_cols();
        queInsert(&rowQueues[i], cols);
        matrix[i] = queResult(rowQueues[i]);
        unselect_row(i);
        wait_us(25);
    }
    wait_ms(DEBOUNCE_SAMPLE_INTERVAL);//每隔一个INTERVAL执行一次
    return 1;
}

void matrix_print(void)
{
    #ifdef KB_NRF_DEBUG
      NRF_LOG_INFO("matrix changed");
    #endif 
}

