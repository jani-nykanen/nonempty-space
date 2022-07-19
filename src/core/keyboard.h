#ifndef NONEMPTY_SPACE_CORE_INTERNAL_KEYBOARD_H
#define NONEMPTY_SPACE_CORE_INTERNAL_KEYBOARD_H


#include "common/types.h"

#include "core/inputstate.h"
#include "core/keycode.h"


typedef struct {

    InputState keyStates [KEY_LAST];

} Keyboard;


Keyboard create_keyboard_manager();

void keyboard_key_pressed_event(Keyboard* keyb, KeyCode key);
void keyboard_key_released_event(Keyboard* keyb, KeyCode key);
void keyboard_update(Keyboard* keyb);

InputState keyboard_get_key_state(Keyboard* keyb, KeyCode key);


#endif // NONEMPTY_SPACE_CORE_INTERNAL_KEYBOARD_H
