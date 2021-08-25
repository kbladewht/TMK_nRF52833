#include <stdint.h>
#include <stdbool.h>

void board_init(void);
void indicator_led_active(void);
void indicator_led_inactive(void);
bool indicator_led_is_active(void);
uint32_t board_wake_up_buttom_prepare(void);