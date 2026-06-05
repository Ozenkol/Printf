#include <stdbool.h>
#include "UI/widget.h"
#include "UI/button_widget.h"

void button_on_event(Widget *w, UiEvent event) {
    ButtonState *state = w->state;
    if (event == EVT_BUTTON_PRESS && state->on_press) {
        state->on_press();
    }
}

void button_render(Widget *w, u8g2_t *u8g2) {
    ButtonState *state = w->state;
    if (w->focused) {
        u8g2_DrawRBox(u8g2, 10, 20, 108, 18, 3);
        
    } else {
        u8g2_DrawRFrame(u8g2, 10, 20, 108, 18, 3);
        u8g2_SetFont(u8g2, u8g2_font_6x12_t_cyrillic);      // ~8 px high
        u8g2_DrawUTF8(u8g2, 14, 33, state->label);
    }
    w->dirty = false;
}