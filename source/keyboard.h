#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include <string>
#include <iostream>

std::string from_keyboard(const std::string& hint_text, bool numeric=false) {
    static SwkbdState swkbd;
    char buffer[300] = "\0";
    swkbdInit(&swkbd, numeric ? SWKBD_TYPE_NUMPAD : SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetInitialText(&swkbd, buffer);
    swkbdSetHintText(&swkbd, hint_text.c_str());
    swkbdSetButton(&swkbd, SWKBD_BUTTON_LEFT, "Cancel", false);
    swkbdSetButton(&swkbd, SWKBD_BUTTON_RIGHT, "Done", true);
    swkbdInputText(&swkbd, buffer, sizeof(buffer));

    return std::string(buffer);
}

#endif