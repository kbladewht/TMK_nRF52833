/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "kb_nrf_led.h"
#include "stdint.h"
#include "led.h"
#include "board_support.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

static uint8_t kb_led_stat = 0;

void led_set(uint8_t usb_led)
{
    kb_led_stat = usb_led;
    if(usb_led & 1 << USB_LED_CAPS_LOCK){
        indicator_led_active();
    }
    else{
        indicator_led_inactive();
    }
    //NRF_LOG_INFO("led set");
}
