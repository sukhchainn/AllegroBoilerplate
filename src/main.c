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

typedef struct ViewNode {
    struct View *view;
    struct ViewNode *next;
} ViewNode;

typedef struct View {
    int x, y, width, height;
    const char *tag;
    ViewNode *children;
} View;

typedef enum {
    TAG_DIV,
    TAG_P,

    TAG_IMG
    // Add more supported tags here
} HtmlTag;

// Parse HTML tags and return corresponding enum
HtmlTag parse_html_tag(const char *tag) {
    if (strcmp(tag, "div") == 0) {
        return TAG_DIV;
    } else if (strcmp(tag, "p") == 0) {
        return TAG_P;
    } else if (strcmp(tag, "img") == 0) {
        return TAG_IMG;
    } else {
        return -1; // Invalid tag
    }
}

// Parse HTML attributes and return corresponding values
// You can add more attributes as needed
void parse_html_attributes(const char *attributes, int *width, int *height) {
    // Example: width="300" height="200"
    sscanf(attributes, "width=\"%d\" height=\"%d\"", width, height);
}

// Parse HTML content and generate view hierarchy
View *parse_html(const char *html) {
    // Simulate parsing and create a root view
    View *root = malloc(sizeof(View));
    root->x = 10;
    root->y = 10;
    root->width = 640;
    root->height = 480;
    root->tag = "div";

    // Simulate child views (sub-elements)
    View *child1 = malloc(sizeof(View));
    child1->x = 20;
    child1->y = 20;
    child1->width = 200;
    child1->height = 100;
    child1->tag = "div";

    View *child2 = malloc(sizeof(View));
    child2->x = 30;
    child2->y = 150;
    child2->width = 300;
    child2->height = 200;
    child2->tag = "p";

    ViewNode *childNode1 = malloc(sizeof(ViewNode));
    childNode1->view = child1;
    childNode1->next = NULL;

    ViewNode *childNode2 = malloc(sizeof(ViewNode));
    childNode2->view = child2;
    childNode2->next = NULL;

    root->children = childNode1;
    childNode1->next = childNode2;

    return root;
}

void render_view(View *view) {
    if (view->tag) {
        if (strcmp(view->tag, "div") == 0) {
            al_draw_filled_rectangle(view->x, view->y, view->x + view->width, view->y + view->height, al_map_rgb(100, 100, 100));
        } else if (strcmp(view->tag, "p") == 0) {
            // Render <p> tag
        } else if (strcmp(view->tag, "img") == 0) {
            // Render <img> tag
        }
    }

    ViewNode *current = view->children;
    while (current != NULL) {
        render_view(current->view);
        current = current->next;
    }
}

void free_view_hierarchy(View *view) {
    if (view == NULL) {
        return;
    }

    ViewNode *current = view->children;
    while (current != NULL) {
        ViewNode *next = current->next;
        free_view_hierarchy(current->view);
        free(current);
        current = next;
    }

    free(view);
}

View* parse_html_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open HTML file: %s\n", filename);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory and read HTML content
    char *html_content = (char *)malloc(file_size + 1);
    if (!html_content) {
        fclose(file);
        return NULL;
    }
    fread(html_content, 1, file_size, file);
    html_content[file_size] = '\0';

    fclose(file);

    // Parse the HTML content and create a view hierarchy
    View *root = parse_html(html_content);

    free(html_content);

    return root;
}


bool initialize();

View *rootView;
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <html_file>\n", argv[0]);
        return -1;
    }

    if (!initialize()) {
        return -1;
    }

    // Construct the full path to the HTML file using the working directory
    char htmlFilePath[256]; // Adjust the size as needed
    snprintf(htmlFilePath, sizeof(htmlFilePath), "%s", argv[1]);
    printf("%s\n%s\n%s\n", argv[0], argv[1], argv[2]);

    rootView = parse_html_file(htmlFilePath);
    if (!rootView) {
        fprintf(stderr, "Failed to parse HTML file.\n");
        al_destroy_display(display);
        return -1;
    }

    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_KEY_DOWN: 
                // Handle F11 key press
                if (event.keyboard.keycode == ALLEGRO_KEY_F11) {
                    al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW);
                    al_resize_display(display, al_get_display_width(display), al_get_display_height(display)); // Refresh the display
                }
                // Handle Esc key press
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    running = false; // Exit the loop
                }
                break;
            default:
                handle_input(event);
                break;
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));
        draw_content(); // Draw your scrollable content;
        // Render the view hierarchy
        render_view(rootView);
        al_flip_display();
    }

    
    // Free memory
    free_view_hierarchy(rootView);
    cleanup();
    return 0;
}

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