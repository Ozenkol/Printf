#pragma once

#include "widget.h"

typedef struct {
    const char *label;
    void (*on_press)(void);
} ButtonState;

void button_on_event(Widget *w, UiEvent event);
void button_render(Widget *w, u8g2_t *u8g2);
void button_on_focus(Widget *w, bool gained);