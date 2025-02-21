#include "Screen.h"

void Sc_changeScreen(Screen *screen, Sc_Type type) {
    switch (type) {
    case SC_MENU:
        screen->update = Sc_Menu_update;
        break;
    default:
        break;
    }
}

Sc_Type Sc_Menu_update(Event *event, Input_t *input_buf) {
    char options[3][12] = {
        "NOVO EVENTO", "VER EVENTOS ", "Teste      "
    };
    static int option = 0;
    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_ANALOG_RIGHT) {
            if (++option > 2)
                option = 0;
        } else if (input_buf[i] == IP_ANALOG_LEFT) {
            if (--option < 0)
                option = 2;
        } else if (input_buf[i] == IP_NONE)
            break;
    }

    Dp_drawString(options[option], 25, DISPLAY_HEIGHT/2-4);
    Dp_update();
    return SC_MENU;
}