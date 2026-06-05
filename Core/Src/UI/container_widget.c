void comntainer_render(Widget *w, u8g2_t *u8g2) {
    // Рендеринг контейнера (можно добавить фон, рамки и т.д.)
    // Затем рендерим дочерние виджеты
    for (Widget *child = w->children; child != NULL; child = child->next) {
        if (child->dirty) {
            child->render(child, u8g2);
        }
    }
}