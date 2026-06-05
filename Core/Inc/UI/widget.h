#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include "u8g2.h"
#include <stdbool.h>

typedef enum {
    EVT_BUTTON_PRESS
} UiEvent;

typedef struct Widget Widget;

struct Widget {
    void (*on_event)(Widget *w, UiEvent event);
    void (*render)(Widget *w, u8g2_t *u8g2);
    void (*on_focus)(Widget *w, bool gained);

    bool dirty;
    bool focused;
    void *state;
};

#endif