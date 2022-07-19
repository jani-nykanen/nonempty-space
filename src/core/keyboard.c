#include "keyboard.h"


Keyboard create_keyboard_manager() {

    Keyboard keyb;
    i32 i;

    for (i = 0; i < KEY_LAST; ++ i) {

        keyb.keyStates[i] = STATE_UP;
    }

    return keyb;
}


void keyboard_key_pressed_event(Keyboard* keyb, KeyCode key) {

    if (key < 0 || key >= KEY_LAST || 
        keyb->keyStates[key] == STATE_DOWN)
        return;

    keyb->keyStates[key] = STATE_PRESSED;
}


void keyboard_key_released_event(Keyboard* keyb, KeyCode key) {

    if (key < 0 || key >= KEY_LAST || 
        keyb->keyStates[key] == STATE_UP)
        return;

    keyb->keyStates[key] = STATE_RELEASED;
}


void keyboard_update(Keyboard* keyb) {

    i32 i;

    for (i = 0; i < KEY_LAST; ++ i) {

        if (keyb->keyStates[i] == STATE_PRESSED)
            keyb->keyStates[i] = STATE_DOWN;
        
        else if (keyb->keyStates[i] == STATE_RELEASED)
            keyb->keyStates[i] = STATE_UP;
    }
}


InputState keyboard_get_key_state(Keyboard* keyb, KeyCode key) {

    if (key < 0 || key >= KEY_LAST)
        return STATE_UP;

    return keyb->keyStates[key]; 
}
