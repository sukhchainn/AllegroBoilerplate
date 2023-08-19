// button.c
#include "button.h"
#include <allegro5/allegro_primitives.h>

Button *create_button(int x, int y, int width, int height, ALLEGRO_COLOR color, const char *text) {
    Button *button = malloc(sizeof(Button));
    if (!button) {
        return NULL;
    }

    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->color = color;
    button->text = text;
    button->pressed = false;

    return button;
}

void draw_button(Button *button, ALLEGRO_FONT *font) {
    al_draw_filled_rectangle(button->x, button->y, button->x + button->width, button->y + button->height, button->color);
    al_draw_text(font, al_map_rgb(255, 255, 255), button->x + button->width / 2, button->y + button->height / 2,
                 ALLEGRO_ALIGN_CENTER, button->text);
}

void destroy_button(Button *button) {
    free(button);
}

bool is_button_clicked(Button *button, int mouseX, int mouseY) {
    return (mouseX >= button->x && mouseX <= button->x + button->width &&
            mouseY >= button->y && mouseY <= button->y + button->height);
}
