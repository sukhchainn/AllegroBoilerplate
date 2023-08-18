// user_interface.h
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <allegro5/allegro.h>

void initialize_user_interface();
void draw_user_interface();
void handle_user_interface_events(ALLEGRO_EVENT event);
void update_user_interface();
void destroy_user_interface();

#endif // USER_INTERFACE_H