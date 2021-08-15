#ifndef CONFIG_H
#define CONFIG_H

#define KB_NRF_DEBUG

#include "custom_board.h"

/*TMK Config*/

/* USB Device descriptor parameter */
#define VENDOR_ID 0xFEED
#define PRODUCT_ID 0x6464
#define DEVICE_VER 0x0001
#define MANUFACTURER TMK
#define PRODUCT SCKB
#define DESCRIPTION     Self Config Keyboard with nRF52833

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 15
/*matrix pin config*/
/*row pins*/
static const uint8_t row_pins[MATRIX_ROWS] = {
    MTXP1, MTXP2, MTXP3, MTXP4, MTXP5, MTXP6};
/*col pins*/
static const uint8_t col_pins[MATRIX_COLS] = {
    MTXP7, MTXP8, MTXP9, MTXP10, MTXP11, MTXP12, MTXP13, MTXP14, MTXP15, MTXP16, MTXP17, MTXP18, MTXP19, MTXP20, MTXP21};

/*matrix diodes direction*/
#define DIODES_DIRECTION 0 /*0: cols to rows; 1: rows to cols*/

/*Backlight config*/
#define BACKLIGHT_LEVELS 3

/*Debounce config*/
/*sample interval (ms)*/
#define DEBOUNCE_SAMPLE_INTERVAL 1
/*times of valid sample to output a key*/
#define DEBOUNCE_SAMPLE_TIMES 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)))

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
#define NO_DEBUG

/* disable print */
#define NO_PRINT

// temp comment
/* disable action features */
#define NO_ACTION_LAYER
#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

#endif