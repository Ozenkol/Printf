#include "UI/mui_port.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t last_raw;
    uint8_t stable;
    uint32_t last_change_ms;
} BtnState;

#define DEBOUNCE_TIME_MS 30

static BtnState btn_state = {0, 0, 0};

static uint8_t debounce(BtnState *b, uint8_t raw)
{
    if (raw != b->last_raw) {
        b->last_raw = raw;
        b->last_change_ms = HAL_GetTick();
    }
    if ((HAL_GetTick() - b->last_change_ms) > DEBOUNCE_TIME_MS) {
        if (raw != b->stable) {
            b->stable = raw;
            return (raw == 0);
        }
    }
    return 0;
}

uint8_t ui_read_buttons(void) {
    uint8_t sel = HAL_GPIO_ReadPin(Input_signal_GPIO_Port, Input_signal_Pin);
    printf("Raw button state: %d\n", sel);
    if (debounce(&btn_state, sel)) {
        return BTN_STEP; // Step button pressed
    }
    return BTN_NONE; // No button pressed
}