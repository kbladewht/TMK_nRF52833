#include "kb_nrf_keyboard.h"
#include "ble_service.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

bool kb_send_report(report_keyboard_t *report) {
    uint64_t result =0;
    for (uint8_t i = 0; i < KEYBOARD_REPORT_SIZE; i++) {
        result |= report->raw[i] << (8 * i);
    }
    NRF_LOG_INFO("reporting %X", result);
    keys_send(8, report->raw);
    return true;
}