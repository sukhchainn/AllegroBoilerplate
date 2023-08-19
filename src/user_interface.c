// user_interface.c
#include "user_interface.h"
#include "button.h" // Include the button header

ALLEGRO_FONT *font;
int counter = 0;
bool buttonPressed = false;
Button *clickButton; // Declare the button

void initialize_user_interface() {
    font = al_create_builtin_font();
    clickButton = create_button(10, 10, 100, 30, al_map_rgb(100, 100, 100), "Click Me");
}

void draw_user_interface() {
    // Draw the button
    draw_button(clickButton, font);

    // Draw the counter
    al_draw_textf(font, al_map_rgb(0, 0, 0), al_get_display_width(al_get_current_display()) - 10, 10, ALLEGRO_ALIGN_RIGHT, "Count: %d", counter);
}

void handle_user_interface_events(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (event.mouse.button == 1) { // Left mouse button
            int mouseX = event.mouse.x;
            int mouseY = event.mouse.y;

            if (is_button_clicked(clickButton, mouseX, mouseY)) {
                // Button is clicked
                clickButton->pressed = true;
            }
        }
    }
}

void update_user_interface() {
    if (clickButton->pressed) {
        counter++;
        clickButton->pressed = false;
    }
}

void destroy_user_interface() {
    al_destroy_font(font);
    destroy_button(clickButton);
}
