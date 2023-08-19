#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h> // For error handling
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "drawing.h"
#include "user_interface.h"

// Define content dimensions and viewport dimensions
const int contentWidth = 800;
const int contentHeight = 800;
const int viewportWidth = 640;
const int viewportHeight = 480;

int viewportX = 0; // Current viewport X position
int viewportY = 0; // Current viewport Y position

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *eventQueue;


// Clean up and destroy components
void cleanup() {
    destroy_user_interface();
    destroy_drawing();

    al_destroy_event_queue(eventQueue);
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();
}

void draw_content() {
    // Draw your content within the viewport bounds
    // You should offset your drawing by -viewportX and -viewportY to adjust for scrolling
    
    // Draw a triangle to test scrolling
    al_draw_filled_triangle(400 - viewportX, 100 - viewportY, 
                            300 - viewportX, 300 - viewportY, 
                            500 - viewportX, 300 - viewportY, al_map_rgb(0, 0, 255));
    // Draw the canvas and UI elements
    draw_canvas();
    update_user_interface();
    draw_user_interface();
}

void handle_input(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        // Handle close event
    }
    else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
        if (event.mouse.dz != 0) {
            // Handle mouse wheel scrolling
            viewportY -= event.mouse.dz * 20; // Adjust scrolling speed
        }
    }
    // Other event handling (user input)
    handle_user_interface_events(event);
}

// Define a basic view structure
typedef struct {
    int x, y, width, height;
    const char *tag; // HTML tag for identification
} View;

// Define a simple HTML parser
View* parse_html(const char *html) {
    // Parse the HTML and create a view hierarchy
    // Return the root view
    // This is a placeholder for the sake of the example
    View *root = malloc(sizeof(View));
    root->x = 10;
    root->y = 10;
    root->width = 200;
    root->height = 100;
    root->tag = "div";
    return root;
}

// Render a view using Allegro 5
void render_view(View *view) {
    al_draw_filled_rectangle(view->x, view->y, view->x + view->width, view->y + view->height, al_map_rgb(100, 100, 100));
}

View *rootView;
// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <html_file>\n", argv[0]);
//         return -1;
//     }

//     if (!initialize()) {
//         return -1;
//     }

//     rootView = parse_html_file(argv[1]);
//     if (!rootView) {
//         fprintf(stderr, "Failed to parse HTML file.\n");
//         al_destroy_display(display);
//         return -1;
//     }

//     bool running = true;
//     while (running) {
//         ALLEGRO_EVENT event;
//         al_wait_for_event(eventQueue, &event);

//         switch (event.type) {
//             case ALLEGRO_EVENT_DISPLAY_CLOSE:
//                 running = false;
//                 break;
//             case ALLEGRO_EVENT_KEY_DOWN: 
//                 // Handle F11 key press
//                 if (event.keyboard.keycode == ALLEGRO_KEY_F11) {
//                     al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW);
//                     al_resize_display(display, al_get_display_width(display), al_get_display_height(display)); // Refresh the display
//                 }
//                 // Handle Esc key press
//                 if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
//                     running = false; // Exit the loop
//                 }
//                 break;
//             default:
//                 handle_input(event);
//                 break;
//         }

//         al_clear_to_color(al_map_rgb(255, 255, 255));
//         draw_content(); // Draw your scrollable content;
//         // Render the view hierarchy
//         render_view(rootView);
//         al_flip_display();
//     }

    
//     free(rootView);
//     cleanup();
//     return 0;
// }

bool initialize() {
    if (!al_init()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Allegro!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    if (!al_install_keyboard()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Keyboard!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    if (!al_install_mouse()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Mouse!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    if (!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize Primitives Addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    if (!al_init_font_addon()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize font addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    if (!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to initialize ttf addon!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }


    display = al_create_display(viewportWidth, viewportHeight);
    if (!display) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to create display!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    al_init_primitives_addon();

    eventQueue = al_create_event_queue();
    if (!eventQueue) {
        al_show_native_message_box(NULL, "Error", "Error", "Failed to create event queue!", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return false;
    }

    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());

    // Initialize drawing and UI components
    initialize_drawing();
    initialize_user_interface();

    return true;
}