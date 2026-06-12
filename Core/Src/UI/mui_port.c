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

extern uint8_t counter;
float   threshold   = 25.0f;

muif_t muif_list[] = {
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_8x13_t_cyrillic),
    MUIF_BUTTON("B0", mui_u8g2_btn_goto_wm_fi),
    MUIF_RO("HR", mui_hrule),
    MUIF_RO(".L", mui_u8g2_draw_text),
    MUIF_U8G2_U8_MIN_MAX("TM", &counter, 0, 100,
                      mui_u8g2_u8_min_max_wm_mse_pi),
    MUIF_BUTTON("GC", mui_u8g2_goto_form_w1_pi),
    MUIF_RO("GP", mui_u8g2_goto_data)



    // Use "BN" for the exit button
};
fds_t fds_data[] = {
    MUI_FORM(0)
    MUI_STYLE(0)
    MUI_LABEL(5, 12, "Главное меню")
    MUI_XY("HR", 0,15)
    MUI_DATA("GP", 
        MUI_10 "Начать измерения|"
        MUI_11 "Поменять язык|"
        MUI_13 "Настройки|"
    )
    MUI_XYA("GC", 5, 30, 0)
    MUI_XYA("GC", 5, 42, 1)
    MUI_XYA("GC", 5, 54, 2)

    MUI_FORM(10)
    MUI_STYLE(0)
    MUI_LABEL(5, 12, "Меню измерений")
    MUI_XY("HR", 0,15)
    MUI_LABEL(5, 30, "Давления:")

    MUI_FORM(11)
    MUI_STYLE(0)
    MUI_LABEL(5, 12, "Выберите язык")
    MUI_XY("HR", 0,15)
    MUI_LABEL(5, 30, "Язык:")

    MUI_FORM(13)
    MUI_STYLE(0)
    MUI_LABEL(5, 12, "Настройки")
    MUI_XY("HR", 0,15)
    MUI_LABEL(5, 30, "Время")
    MUI_LABEL(5, 40, "с начала вкл.:")
    

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
            break;
        case BTN_STEP:
            printf("Button pressed! Counter: %d\n", counter);
            mui_NextField(&myUI);
            is_redraw = 1;
            break;
        case BTN_ENTER:
            printf("Button pressed! Counter: %d\n", counter);
            mui_SendSelect(&myUI);     // tells MUI to activate the current field
            is_redraw = 1;
    }

    // ── draw only when something changed ────────────────────
    if (!is_redraw) return;

    u8g2_ClearBuffer(&myDisplay);
    mui_Draw(&myUI);
    u8g2_SendBuffer(&myDisplay);

    is_redraw = 0;
}