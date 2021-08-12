#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H
#include <stdbool.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
    
    void log_init(void);
    void timers_init(void);
    void buttons_leds_init(bool * erase_bonds);
    void power_management_init();
    void ble_stack_init(void);
    void scheduler_init(void);
    void gap_params_init(void);
    void gatt_init(void);
    void advertising_init(void);
    void services_init(void);
    void sensor_simulator_init(void);
    void conn_params_init(void);
    void buffer_init(void);
    void peer_manager_init(void);

    // Start execution.
    void timers_start(void);
    void advertising_start(bool erase_bonds);
    void idle_state_handle(void);

#endif