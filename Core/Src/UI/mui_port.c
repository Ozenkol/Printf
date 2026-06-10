#include "UI/mui_port.h"
#include "main.h"
#include <stdint.h>

static u8g2_t myDisplay;
static mui_t myUI;
static uint8_t is_redraw = 1;

static uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

static uint8_t u8x8_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

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

muif_t muif_list[] = {
    MUIF_BUTTON("B0", mui_u8g2_btn_goto_wm_fi),
    MUIF_RO("HR", mui_hrule),
    // Use "BN" for the exit button
};
fds_t fds_data[] = {
    MUI_FORM(1)
    MUI_XY("HR", 0,15)
};
uint8_t muif_list_size = sizeof(muif_list) / sizeof(muif_t);

void mui_port_init(void)
{
    u8g2_Setup_sh1106_128x64_noname_f(&myDisplay, U8G2_R0, u8x8_spi,u8x8_gpio_and_delay);  // Инициализация u8g2-структуры
    u8g2_InitDisplay(&myDisplay); // Отправка последовательности инициализации на дисплей, после чего дисплей переходит в спящий режим
    u8g2_SetPowerSave(&myDisplay, 0); // Пробуждение дисплея
    u8g2_ClearBuffer(&myDisplay);
    mui_Init(&myUI, &myDisplay, fds_data, muif_list, muif_list_size);
    mui_GotoForm(
      &myUI,
      0, /* form id */
      0  /* initial cursor position */
    );

    is_redraw = 0;   

}

void mui_port_tick(void)
{
    if (!mui_IsFormActive(&myUI)) return;

    if (!is_redraw) {
        u8g2_FirstPage(&myDisplay);
        do {
            mui_Draw(&myUI);
        } while (u8g2_NextPage(&myDisplay));
        is_redraw = 1;
    }

    switch (ui_read_buttons()) {
        case BTN_NONE:
            mui_SendSelect(&myUI);
            is_redraw = 1;
            break;
        case BTN_STEP:
            mui_NextField(&myUI);
            is_redraw = 1;
            break;
    }
}