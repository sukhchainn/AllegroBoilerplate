// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef struct {
    int x, y, width, height;
    ALLEGRO_COLOR color;
    const char *text;
    bool pressed;
} Button;

Button *create_button(int x, int y, int width, int height, ALLEGRO_COLOR color, const char *text);
void draw_button(Button *button, ALLEGRO_FONT *font);
void destroy_button(Button *button);
bool is_button_clicked(Button *button, int mouseX, int mouseY);

#endif // BUTTON_H
