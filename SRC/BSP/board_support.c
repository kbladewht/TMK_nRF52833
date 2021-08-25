#include <stdint.h>
#include <stdbool.h>

#include "config.h"

#include "board_support.h"

#include "nrf_gpio.h"
#include "nordic_common.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


// setting VDD and GPIO voltage to 3.3V when power via VDDH
static void gpio_output_voltage_setup(void)
{
    // Configure UICR_REGOUT0 register only if it is set to default value.
    if ((NRF_UICR->REGOUT0 & UICR_REGOUT0_VOUT_Msk) ==
        (UICR_REGOUT0_VOUT_DEFAULT << UICR_REGOUT0_VOUT_Pos))
    {
        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        NRF_UICR->REGOUT0 = (NRF_UICR->REGOUT0 & ~((uint32_t)UICR_REGOUT0_VOUT_Msk)) |
                            (UICR_REGOUT0_VOUT_3V3 << UICR_REGOUT0_VOUT_Pos);

        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        // System reset is needed to update UICR registers.
        NVIC_SystemReset();
    }
}

//Indicator led
//Config the pin as output and lit it for 100ms
static void indicator_led_init(void)
{
    nrf_gpio_cfg_output(STATE_LED);
    if(STATE_LED_ACTIVE){
        nrf_gpio_pin_set(STATE_LED);
        nrf_delay_ms(100);
        nrf_gpio_pin_clear(STATE_LED);
    }
    else{
        nrf_gpio_pin_clear(STATE_LED);
        nrf_delay_ms(100);
        nrf_gpio_pin_set(STATE_LED);
    }
    NRF_LOG_INFO("Indicator led init");
}

void indicator_led_active(void)
{
    if(STATE_LED_ACTIVE){
        nrf_gpio_pin_set(STATE_LED);
    }
    else{
        nrf_gpio_pin_clear(STATE_LED);
    }
}

void indicator_led_inactive(void)
{
    if(STATE_LED_ACTIVE){
        nrf_gpio_pin_clear(STATE_LED);
    }
    else{
        nrf_gpio_pin_set(STATE_LED);
    }
}

bool indicator_led_is_active(void)
{
    uint32_t state = nrf_gpio_pin_read(STATE_LED);
    if(state == STATE_LED_ACTIVE){
        return true;
    }
    else{
        return false;
    }
}


//Init board func
void board_init()
{
    gpio_output_voltage_setup();
    indicator_led_init();
}


//Board wakeup prepare: setting gpios to sense
uint32_t board_wake_up_buttom_prepare(void)
{
    static nrf_gpio_pin_pull_t col_pull_config;
    static nrf_gpio_pin_sense_t col_sense_config;
    static bool row_state;
    if(DIODES_DIRECTION){ //当二极管方向是从行到列时，列开启下拉电阻，上升沿感知，行输出高电平
        col_pull_config = NRF_GPIO_PIN_PULLDOWN;
        col_sense_config = NRF_GPIO_PIN_SENSE_HIGH;
        row_state = true;
    }
    else{
        col_pull_config = NRF_GPIO_PIN_PULLUP;
        col_sense_config = NRF_GPIO_PIN_SENSE_LOW;
        row_state = false;
    }
    //设置列为sense
    for(uint8_t idx = 0; idx < MATRIX_COLS; idx++){
        nrf_gpio_cfg_sense_input(col_pins[idx], col_pull_config, col_sense_config);
    }
    //设置行输出状态
    for(uint8_t idx = 0; idx < MATRIX_ROWS; idx++){
        nrf_gpio_cfg_output(row_pins[idx]);
        if(row_state){
            nrf_gpio_pin_set(row_pins[idx]);
        }
        else{
            nrf_gpio_pin_clear(row_pins[idx]);
        }
    }
    return NRF_SUCCESS;
}