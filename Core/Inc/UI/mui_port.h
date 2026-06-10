#pragma once
#include "u8g2.h"
#include "mui.h"
#include "mui_u8g2.h"
#include <stdint.h>

#define BTN_NONE 0
#define BTN_STEP 1

void mui_port_init(void);
void mui_port_tick(void);
uint8_t ui_read_buttons(void);