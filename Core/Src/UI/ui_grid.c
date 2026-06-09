#include "UI/ui_grid.h"
#include <stdio.h>

static void draw_cell(u8g2_t *u8g2, uint8_t cx, uint8_t cy, uint8_t cw, uint8_t ch, float val) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%.1f", val);

    uint8_t tw = u8g2_GetStrWidth(u8g2, buf);
    uint8_t tx = cx + (cw - tw) / 2;
    uint8_t ty = cy + (ch - u8g2_GetMaxCharHeight(u8g2)) / 2 + u8g2_GetMaxCharHeight(u8g2);

    u8g2_DrawStr(u8g2, tx, ty, buf);
}

void grid_init(GridWidget *grid, uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    grid->x = x;
    grid->y = y;
    grid->width = width;
    grid->height = height;
    grid->isDirty = true;

    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            grid->cells[r][c] = 0.0f;
        }
    }
}

void grid_set_cell(GridWidget *grid, uint8_t row, uint8_t col, float value) {
    if (row >= GRID_ROWS || col >= GRID_COLS) return;
    grid->cells[row][col] = value;
    grid->isDirty = true;
}

void grid_render(GridWidget *grid, u8g2_t *u8g2) {
    if (!grid->isDirty) return;

    uint8_t cell_w = grid->width / GRID_COLS;
    uint8_t cell_h = grid->height / GRID_ROWS;

    u8g2_DrawFrame(u8g2, grid->x, grid->y, grid->width, grid->height);

    // Draw grid lines
    for (int r = 0; r <= GRID_ROWS; r++) {
        u8g2_DrawLine(u8g2, grid->x, grid->y + r * cell_h, grid->x + grid->width, grid->y + r * cell_h);
    }
    for (int c = 0; c <= GRID_COLS; c++) {
        u8g2_DrawLine(u8g2, grid->x + c * cell_w, grid->y, grid->x + c * cell_w, grid->y + grid->height);
    }

    // Draw cell values
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            draw_cell(u8g2, grid->x + c * cell_w, grid->y + r * cell_h, cell_w, cell_h, grid->cells[r][c]);
        }
    }

    grid->isDirty = false;
}