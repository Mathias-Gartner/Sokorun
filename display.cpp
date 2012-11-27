#include <conio.h>
#include <stdlib.h>
#include "definitions.h"
#include "display.h"
#include "globals.h"

void textcolor(COLORS color)
{
    __COLOR = (__COLOR & 0xf0) | (color & 0x0f);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), __COLOR);
}

void textbackground(COLORS color)
{
    __COLOR = (__COLOR & 0x0f) | ((color & 0x0f) << 4);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), __COLOR);
}

void txtcolor (int farbe)
{
    switch(farbe)
    {
        case 0: textcolor(ccBLACK);       textbackground(ccWHITE);          break;
        case 1: textcolor(ccYELLOW);      textbackground(ccBLUE);           break;
        case 2: textcolor(ccLIGHTMAGENTA);textbackground(ccGREEN);          break;
        case 3: textcolor(ccLIGHTRED);    textbackground(ccCYAN);           break;
        case 4: textcolor(ccLIGHTCYAN);   textbackground(ccRED);            break;
        case 5: textcolor(ccLIGHTGREEN);  textbackground(ccMAGENTA);        break;
        case 6: textcolor(ccLIGHTBLUE);   textbackground(ccBROWN);          break;
        case 7: textcolor(ccDARKGRAY);    textbackground(ccLIGHTGRAY);      break;
        case 8: textcolor(ccLIGHTGRAY);   textbackground(ccDARKGRAY);       break;
        case 9: textcolor(ccBROWN);       textbackground(ccLIGHTBLUE);      break;
        case 10:textcolor(ccMAGENTA);     textbackground(ccLIGHTGREEN);     break;
        case 11:textcolor(ccRED);         textbackground(ccLIGHTCYAN);      break;
        case 12:textcolor(ccCYAN);        textbackground(ccLIGHTRED);       break;
        case 13:textcolor(ccGREEN);       textbackground(ccLIGHTMAGENTA);   break;
        case 14:textcolor(ccBLUE);        textbackground(ccYELLOW);         break;
        case 15:textcolor(ccWHITE);       textbackground(ccBLACK);          break;
    }
}

// Ändert die Anzeige des Cursors am Bildschirm
void setcursortype(int type)
{
    CONSOLE_CURSOR_INFO info;

    info.bVisible = TRUE;
    if(type == NOCURSOR)
        info.bVisible = FALSE;
    else if(type == NORMALCURSOR)
        info.dwSize = 1;
    else if(type == SOLIDCURSOR)
        info.dwSize = 100;
    else
        return;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
