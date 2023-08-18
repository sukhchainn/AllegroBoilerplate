// user_interface.c
#include "user_interface.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h> // For error handling
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

ALLEGRO_FONT *font;
int counter = 0;
bool buttonPressed = false;

void initialize_user_interface() {
    font = al_create_builtin_font();
}

void draw_user_interface() {
    // Draw the button
    al_draw_filled_rectangle(10, 10, 110, 40, al_map_rgb(100, 100, 100));
    al_draw_text(font, al_map_rgb(255, 255, 255), 60, 20, ALLEGRO_ALIGN_CENTER, "Click Me");

    // Draw the counter
    al_draw_textf(font, al_map_rgb(0, 0, 0), al_get_display_width(al_get_current_display()) - 10, 10, ALLEGRO_ALIGN_RIGHT, "Count: %d", counter);
}

void handle_user_interface_events(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.mouse.button == 1) { // Left mouse button
            int mouseX = event.mouse.x;
            int mouseY = event.mouse.y;

            if (mouseX >= 10 && mouseX <= 110 && mouseY >= 10 && mouseY <= 40) {
                // Button is clicked
                buttonPressed = true;
            }
        }
    }
}

void update_user_interface() {
    if (buttonPressed) {
        counter++;
        buttonPressed = false;
    }
}

void destroy_user_interface() {
    al_destroy_font(font);
}