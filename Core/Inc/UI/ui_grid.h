#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <u8g2.h>

#define GRID_ROWS 3
#define GRID_COLS 3

typedef struct {
    //position
    uint8_t x, y;
    //size
    uint8_t width, height;

    float cells[GRID_ROWS][GRID_COLS];

    bool isDirty;
} GridWidget;

void grid_init(GridWidget *grid, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void grid_set_cell(GridWidget *grid, uint8_t row, uint8_t col, float value);
void grid_render(GridWidget *grid, u8g2_t *u8g2);