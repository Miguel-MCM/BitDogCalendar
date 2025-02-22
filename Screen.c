#include "Screen.h"

void Sc_changeScreen(Screen *screen, Sc_Type type) {
    Dp_clear();
    switch (type) {
    case SC_MENU:
        screen->update = Sc_Menu_update;
        break;
    case SC_NEW_EVENT:
        screen->update = Sc_New_Event_update;
        break;
    default:
        break;
    }
}



Sc_Type Sc_Menu_update(Event *event, Input_t *input_buf) {
    char options[2][12] = {
       "           ", "NOVO EVENTO"
    };
    static int option = 0;
    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_BUTTOM_A) {
            if (option == 0)
                return SC_NEW_EVENT;
        }
        else if (input_buf[i] == IP_ANALOG_RIGHT) {
            Dp_clear();
            if (++option > 1)
                option = 0;
        } else if (input_buf[i] == IP_ANALOG_LEFT) {
            Dp_clear();
            if (--option < 1)
                option = 0;
        } else if (input_buf[i] == IP_NONE)
            break;
    }

    Dp_drawString(options[option], 25, DISPLAY_HEIGHT/2-4);
    if (option == 0) {
        Dp_drawString("  /  /    ", 25, DISPLAY_HEIGHT/4);
        Dp_drawUInt(event->begin.tm_mday, 25 + 8, DISPLAY_HEIGHT/4, 2);
        Dp_drawUInt(event->begin.tm_mon, 25 + 32, DISPLAY_HEIGHT/4, 2);
        Dp_drawUInt(event->begin.tm_year, 25 + 72, DISPLAY_HEIGHT/4, 4);

        Dp_drawString("  :  ", 25+24, (DISPLAY_HEIGHT*3)/4);
        Dp_drawUInt(event->begin.tm_hour, 25 + 32, (DISPLAY_HEIGHT*3)/4, 2);
        Dp_drawUInt(event->begin.tm_min, 25 + 58, (DISPLAY_HEIGHT*3)/4, 2);
    }
    Dp_update();
    return SC_MENU;
}

int getInputInt(Input_t *input_buf, int lower_limit, int upper_limit, bool restart, int restart_value) {
    static int selected = 1;
    if (restart)
        selected = restart_value;

    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_ANALOG_UP) {
            if (++selected > upper_limit)
                selected = lower_limit;
        } else if (input_buf[i] == IP_ANALOG_DOWN) {
            if (--selected < lower_limit)
                selected = upper_limit;
        } else if (input_buf[i] == IP_NONE)
            break;
    }
    return selected;
}

int8_t Sc_New_Event_select_time(struct tm *time, Input_t *input_buf) {
    static bool first = true;
    static bool blink = false;
    static int selected = 0;
    const int upper_limit[5] = {31, 12, 9999, 23, 59};
    const int lower_limit[5] = {1, 1, 0, 0, 0};
    int* ptrs[5] = {&(time->tm_mday), &(time->tm_mon), &(time->tm_year), &(time->tm_hour), &(time->tm_min)};


    *(ptrs[selected]) = getInputInt(input_buf, lower_limit[selected], upper_limit[selected], first, (*(ptrs[selected]) >= lower_limit[selected] && *(ptrs[selected]) <= upper_limit[selected])? *ptrs[selected] : lower_limit[selected]);
    //first = false;

    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_ANALOG_RIGHT) {
            first = true;
            if (++selected > 4)
                selected = 0;
        } else if (input_buf[i] == IP_ANALOG_LEFT) {
            first = true;
            if (--selected < 0)
                selected = 4;
        } else if (input_buf[i] == IP_NONE)
            break;
    }

    Dp_drawString("  /  /    ", 25, DISPLAY_HEIGHT/4);
    if (selected != 0 || (selected == 0 && blink)) Dp_drawUInt(time->tm_mday, 25 + 8, DISPLAY_HEIGHT/4, 2);
    if (selected != 1 || (selected == 1 && blink)) Dp_drawUInt(time->tm_mon, 25 + 32, DISPLAY_HEIGHT/4, 2);
    if (selected != 2 || (selected == 2 && blink)) Dp_drawUInt(time->tm_year, 25 + 72, DISPLAY_HEIGHT/4, 4);

    Dp_drawString("  :  ", 25+32, (DISPLAY_HEIGHT*3)/4);
    if (selected != 3 || (selected == 3 && blink)) Dp_drawUInt(time->tm_hour, 25 + 40, (DISPLAY_HEIGHT*3)/4, 2);
    if (selected != 4 || (selected == 4 && blink)) Dp_drawUInt(time->tm_min, 25 + 64, (DISPLAY_HEIGHT*3)/4, 2);
    blink = !blink;

    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_BUTTOM_A) {
            return 1;
        }if (input_buf[i] == IP_BUTTOM_B) {
            return -1;
        } else if (input_buf[i] == IP_NONE)
            break;
    }
    return 0;
}

int8_t Sc_New_Event_insertName(Event *event, Input_t *input_buf) {
    static bool first = true;
    static bool blink = false;
    static int selected = 0;
    char* name = event->name;


    name[selected] = getInputInt(input_buf, 'A'-1, 'Z', first, (name[selected] >= 'A' && name[selected] <= 'Z')? name[selected] : 'A'-1);
    first = false;

    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_ANALOG_RIGHT) {
            first = true;
            if (++selected > MAX_EVENT_NAME_LEN)
                selected = MAX_EVENT_NAME_LEN;
        } else if (input_buf[i] == IP_ANALOG_LEFT) {
            first = true;
            if (--selected < 0)
                selected = 0;
        } else if (input_buf[i] == IP_NONE)
            break;
    }

    for (int i=0; i<MAX_EVENT_NAME_LEN;++i) {
        if (selected != i || (selected == i && blink)) Dp_drawChar(name[i], DISPLAY_CHAR_SIZE*i, DISPLAY_HEIGHT/2 + DISPLAY_CHAR_SIZE/2);
        else Dp_drawChar(0xff, DISPLAY_CHAR_SIZE*i, DISPLAY_HEIGHT/2 + DISPLAY_CHAR_SIZE/2);
    }

    blink = !blink;

    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_BUTTOM_A) {
            return 1;
        }if (input_buf[i] == IP_BUTTOM_B) {
            return -1;
        } else if (input_buf[i] == IP_NONE)
            break;
    }
    return 0;
}

int8_t Sc_New_Event_alarm(Event *event, Input_t *input_buf) {
    Dp_drawString("Alarme", 32, DISPLAY_HEIGHT/4);
    Dp_drawString("A: Sim", 0, DISPLAY_HEIGHT/2);
    Dp_drawString("B: Nao", DISPLAY_WIDTH-6*DISPLAY_CHAR_SIZE, DISPLAY_HEIGHT/2);



    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_BUTTOM_A) {
            event->alert = true;
            return 1;
        }if (input_buf[i] == IP_BUTTOM_B) {
            event->alert = false;
            return 1;
        } else if (input_buf[i] == IP_NONE)
            break;
    }
    return 0;
}

int8_t Sc_New_Event_setColor(Event *event, Input_t *input_buf) {
    static bool first = true;
    static bool blink = false;
    static int selected = 0;
    uint8_t* ptrs[3] = {&(event->color.r), &(event->color.g), &(event->color.b)};

    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_ANALOG_RIGHT) {
            first = true;
            if (++selected > 2)
            selected = 0;
        } else if (input_buf[i] == IP_ANALOG_LEFT) {
            first = true;
            if (--selected < 0)
            selected = 2;
        } else if (input_buf[i] == IP_NONE)
        break;
    }
    
    Dp_drawString("R: ", 0, 0);
    Dp_drawString("G: ", 0, DISPLAY_HEIGHT/2 - DISPLAY_CHAR_SIZE/2);
    Dp_drawString("B: ", 0, DISPLAY_HEIGHT- DISPLAY_CHAR_SIZE);
    *(ptrs[selected]) = getInputInt(input_buf, 0, 255, first, *ptrs[selected]);

    if (selected != 0 || (selected == 0 && blink)) Dp_drawUInt(*ptrs[0], 4*DISPLAY_CHAR_SIZE, 0, 3);
    if (selected != 1 || (selected == 1 && blink)) Dp_drawUInt(*ptrs[1], 4*DISPLAY_CHAR_SIZE, DISPLAY_HEIGHT/2 - DISPLAY_CHAR_SIZE/2, 3);
    if (selected != 2 || (selected == 2 && blink)) Dp_drawUInt(*ptrs[2], 4*DISPLAY_CHAR_SIZE, DISPLAY_HEIGHT- DISPLAY_CHAR_SIZE, 3);

    blink = !blink;
    for (int i=0; i<INPUT_BUFF_SIZE; ++i) {
        if (input_buf[i] == IP_BUTTOM_A) {
            return 1;
        }if (input_buf[i] == IP_BUTTOM_B) {
            return -1;
        } else if (input_buf[i] == IP_NONE)
            break;
    }
    return 0;
}

Sc_Type Sc_New_Event_update(Event *event, Input_t *input_buf) {
    static int8_t stage = 0;
    Dp_clear();
    switch (stage) {
    case -1:
        stage = 0;
        return SC_MENU;
    case 0:
        Dp_drawString("Inicio", 32, 0);
        stage += Sc_New_Event_select_time(&(event->begin), input_buf);
        event->end = event->begin;
        break;
    case 1:
        Dp_drawString("Fim", 32, 0);
        stage += Sc_New_Event_select_time(&(event->end), input_buf);
        break;
    case 2:
        stage += Sc_New_Event_insertName(event, input_buf);
        break;
    case 3:
        stage += Sc_New_Event_setColor(event, input_buf);
        break;
    case 4:
        stage += Sc_New_Event_alarm(event, input_buf);
        break;
    case 5:
        stage = 0;
        return SC_NEW_EVENT_F;
    }
    Dp_update();
    return SC_NEW_EVENT;
}