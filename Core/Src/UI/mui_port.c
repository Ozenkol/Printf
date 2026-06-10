#include "UI/mui_port.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>

static u8g2_t myDisplay;
static mui_t myUI;
static uint8_t is_redraw = 0;

extern uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

extern uint8_t u8x8_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

uint8_t mui_hrule(mui_t *ui, uint8_t msg)
{
  u8g2_t *u8g2 = mui_get_U8g2(ui);
  switch(msg)
  {
    case MUIF_MSG_DRAW:
      u8g2_DrawHLine(u8g2, 0, mui_get_y(ui), u8g2_GetDisplayWidth(u8g2));
      break;
  }
  return 0;
}

uint8_t temperature = 58;    // your variable
float   threshold   = 25.0f;

muif_t muif_list[] = {
    MUIF_BUTTON("B0", mui_u8g2_btn_goto_wm_fi),
    MUIF_RO("HR", mui_hrule),
    MUIF_RO(".L", mui_u8g2_draw_text),
    MUIF_U8G2_U8_MIN_MAX("TM", &temperature, 0, 100,
                      mui_u8g2_u8_min_max_wm_mse_pi)

    // Use "BN" for the exit button
};
fds_t fds_data[] = {
    MUI_FORM(0)
    MUI_XY("HR", 0,15)
    MUI_LABEL(9, 30, "Temp:")

};
uint8_t muif_list_size = sizeof(muif_list) / sizeof(muif_t);

void mui_port_init(void)
{
    u8g2_Setup_sh1106_128x64_noname_f(&myDisplay, U8G2_R0,
                                       u8x8_spi,
                                       u8x8_gpio_and_delay);
    u8g2_InitDisplay(&myDisplay);
    u8g2_SetPowerSave(&myDisplay, 0);

    u8g2_SetFont(&myDisplay, u8g2_font_6x10_tr);

    // clear both RAM buffer AND display GDDRAM
    u8g2_ClearBuffer(&myDisplay);
    u8g2_SendBuffer(&myDisplay);    // ← push zeroed RAM to display NOW

    mui_Init(&myUI, &myDisplay, fds_data, muif_list, muif_list_size);
    mui_GotoForm(&myUI, 0, 0);
    mui_Draw(&myUI);

    is_redraw = 1;
}

void mui_port_tick(void)
{
    if (!mui_IsFormActive(&myUI)) return;

    // ── buttons FIRST — always, regardless of redraw state ──
    switch (ui_read_buttons()) {
        case BTN_NONE:                    // ← do NOTHING on no press
            printf("No button pressed.\n");
            break;
        case BTN_STEP:
            mui_NextField(&myUI);
            is_redraw = 1;
            break;
    }

    // ── draw only when something changed ────────────────────
    if (!is_redraw) return;

    u8g2_ClearBuffer(&myDisplay);
    mui_Draw(&myUI);
    u8g2_SendBuffer(&myDisplay);

    is_redraw = 0;
}