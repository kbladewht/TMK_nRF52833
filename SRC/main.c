/**
 * Copyright (c) 2012 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup ble_sdk_app_hids_keyboard_main main.c
 * @{
 * @ingroup ble_sdk_app_hids_keyboard
 * @brief HID Keyboard Sample Application main file.
 *
 * This file contains is the source code for a sample application using the HID, Battery and Device
 * Information Services for implementing a simple keyboard functionality.
 * Pressing Button 0 will send text 'hello' to the connected peer. On receiving output report,
 * it toggles the state of LED 2 on the mother board based on whether or not Caps Lock is on.
 * This application uses the @ref app_scheduler.
 *
 * Also it would accept pairing requests from any peer device.
 */

//Std Headers
#include <stdint.h>
#include <string.h>
//Log Headers
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
//SDK Headers
#include "nrf_pwr_mgmt.h"
#include "app_timer.h"
#include "app_scheduler.h"
#include "nrf_log_ctrl.h"
//TMK Headers
#include "keyboard.h"
#include "host.h"
//Own Headers
#include "board_support.h"
#include "ble_service.h"
#include "kb_nrf_driver.h"
#include "nrf_pwr_mgmt.h"

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module.
 */
static void timers_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    app_sched_execute();
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

static void keyboard_scan_handler(void* p_context)
{
    keyboard_task();
}

APP_TIMER_DEF(m_keyboard_scan_timer_id);

void init_and_start_scan_timer(void)
{
    ret_code_t err_code = app_timer_create(&m_keyboard_scan_timer_id,
        APP_TIMER_MODE_REPEATED,
        keyboard_scan_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_keyboard_scan_timer_id,  APP_TIMER_TICKS(10), NULL);
    APP_ERROR_CHECK(err_code);

}


/**@brief Function for application main entry.
 */
int main(void)
{
    bool erase_bonds = false;

    // Initialize.
    log_init();

    board_init();

    timers_init();
    power_management_init();
     
    ble_stack_init();
    scheduler_init();
    gap_params_init();
    gatt_init();

    advertising_init();
    conn_params_init();
    peer_manager_init();

    buffer_init();
    services_init();
    // Start execution.
    NRF_LOG_INFO("TMK Keyboard Start");

    keyboard_init();
    host_set_driver(&kb_nrf_driver);
    init_and_start_scan_timer();

    advertising_start(erase_bonds);

    kb_power_meas_init();
    kb_power_meas_start();
    //power management init and start

    // Enter main loop.
    for (;;)
    {
        idle_state_handle();
    }
}


