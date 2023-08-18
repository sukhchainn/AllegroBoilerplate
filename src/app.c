#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h> // For error handling
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "drawing.h"
#include "user_interface.h"

int init();

int main() {
    if (!init()) {
        return -1;
    }

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to create display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    if (!event_queue) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to create event queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Initialize drawing and UI components
    initialize_drawing();
    initialize_user_interface();

    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Handle F11 key press
            if (event.keyboard.keycode == ALLEGRO_KEY_F11) {
                al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW);
                al_resize_display(display, al_get_display_width(display), al_get_display_height(display)); // Refresh the display
            }
            // Handle Esc key press
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false; // Exit the loop
            }
        }

        // Other event handling (user input)
        handle_user_interface_events(event);

        al_clear_to_color(al_map_rgb(255, 255, 255));

        // Draw the canvas and UI elements
        draw_canvas();
        update_user_interface();
        draw_user_interface();

        al_flip_display();
    }

    // Clean up and destroy components
    destroy_user_interface();
    destroy_drawing();

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();

    return 0;
}

int init() {
    if (!al_init()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_install_keyboard()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Keyboard!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_install_mouse()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Mouse!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Primitives Addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    if (!al_init_font_addon()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize font addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    if (!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize ttf addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    return 1;
}