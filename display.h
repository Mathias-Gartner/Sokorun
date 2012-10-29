/*
  Name:     display.h
  Author:   Prof. Dipl.-Ing. Andreas JEDLICKA, HTBL-Hollabrunn
  Date:     21.02.2008
  Description: Bildschirmfunktionen

  Version:  1.0 (für gcc 4.2)
*/

// Cursorarten
#define NOCURSOR        0
#define NORMALCURSOR    1
#define SOLIDCURSOR     2

// Farben für Text und Hintergrund
typedef enum
{
    ccBLACK,              // 0000
    ccBLUE,               // 0001
    ccGREEN,              // 0010
    ccCYAN,               // 0011
    ccRED,                // 0100
    ccMAGENTA,            // 0101
    ccBROWN,              // 0110
    ccLIGHTGRAY,          // 0111
    ccDARKGRAY,           // 1000
    ccLIGHTBLUE,          // 1001
    ccLIGHTGREEN,         // 1010
    ccLIGHTCYAN,          // 1011
    ccLIGHTRED,           // 1100
    ccLIGHTMAGENTA,       // 1101
    ccYELLOW,             // 1110
    ccWHITE               // 1111
} COLORS;

// Cursorposition am Bildschirm von 1,1 bis 80,25 (links oben bis rechts unten) [oder entsprechend der geänderten Größe]
void textcolor(COLORS color);       // setzen der Textfarbe
void textbackground(COLORS color);  // setzen der Hintergrundfarbe bei der Textausgabe
void setcursortype(int type);       // setzen, wie der Cursors ausschaut
int  gotoXY(int x, int y);          // Cursor am Bildschirm setzen (return 1 bei OK, 0 bei Fehler)
int  whereX(void);                  // auf welcher Position ist der Cursor ?
int  whereY(void);                  // in welcher Zeile ist der Cursor ?
int  sizeX(void);                   // wie Breit ist das Fenster ?
int  sizeY(void);                   // wie Hoch ist das Fenster ?
void clrscr(void);                  // Löscht den gesamten Bildschirm
void clreol(void);                  // Löscht vom Cursor bis zum Zeilenende
void delline(void);                 // Löscht die Zeile, in der der Cursor steht
int  getstr(int left, int top, int right, int bottom, char *str);// Liefert die angezeigten Zeichen aus dem angegebenen Fensterbereich
int  putstr(int left, int top, int right, int bottom, char *str);// Schreibt den angegebenen STRING in den angebenen Fensterbereich und setzt die Hintergrundfarbe
void txtcolor (int farbe);

// ******************** INTERN ******************** //
#include <windows.h>

static int __COLOR = ccLIGHTGRAY | (ccBLACK<<4);

// Setzt Farben für Ausgabe am Bildschirm
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


// Cursor am Bildschirm setzen (return 1 bei OK und 0 bei Fehler)
int gotoXY(int x, int y)
{
    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){x-1, y-1});
}

void gotoxy(int x, int y)
{
    gotoXY(x+3,y+3);
}


// Liefert die aktuelle Cursorposition zurück
int whereX(void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.X+1;
}

int whereY(void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.dwCursorPosition.Y+1;
}


// Liefert die größe des Fensters zurück
int sizeX(void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.srWindow.Right-info.srWindow.Left+1;
}

int sizeY(void)
{
    CONSOLE_SCREEN_BUFFER_INFO info;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
    return info.srWindow.Bottom-info.srWindow.Top+1;
}


// Bildschirmlöschfunktionen
void clrscr(void)
{
    DWORD  dummy;
    int    size;
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsoleOut, &info);
    size = (info.srWindow.Right-info.srWindow.Left+1) * (info.srWindow.Bottom-info.srWindow.Top+1);

    FillConsoleOutputAttribute(hConsoleOut, 0 /*__COLOR*/, size, (COORD){0, 0}, &dummy);
    FillConsoleOutputCharacter(hConsoleOut, '\0', size, (COORD){0, 0}, &dummy);
    SetConsoleCursorPosition(hConsoleOut, (COORD){0, 0});
}

void clreol(void)
{
    DWORD  dummy;
    int    size;
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsoleOut, &info);
    size = (info.srWindow.Right-info.srWindow.Left+1) - info.dwCursorPosition.X;

    FillConsoleOutputAttribute(hConsoleOut, 0 /*__COLOR*/, size, info.dwCursorPosition, &dummy);
    FillConsoleOutputCharacter(hConsoleOut, '\0', size, info.dwCursorPosition, &dummy);
    SetConsoleCursorPosition(hConsoleOut, info.dwCursorPosition);
}

void delline(void)
{
    DWORD  dummy;
    int    size;
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hConsoleOut, &info);
    size = (info.srWindow.Right-info.srWindow.Left+1);

    FillConsoleOutputAttribute(hConsoleOut, 0 /*__COLOR*/, size, (COORD){0,info.dwCursorPosition.Y}, &dummy);
    FillConsoleOutputCharacter(hConsoleOut, '\0', size, (COORD){0, info.dwCursorPosition.Y}, &dummy);
    SetConsoleCursorPosition(hConsoleOut, (COORD){0, info.dwCursorPosition.Y});
}


// Liest oder Schreibt Text in einen Bildschirmbereich
int getstr(int left, int top, int right, int bottom, char *str)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    int i, j, n=0;
    SMALL_RECT r = (SMALL_RECT){left-1, top-1, right-1, bottom-1};
    CHAR_INFO buffer[info.dwSize.Y][info.dwSize.X];

    if(ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), (PCHAR_INFO)buffer, info.dwSize, (COORD){0, 0}, &r))
    {
        for(i=0; i<=bottom-top; i++)
            for(j=0; j<=right-left; j++)
                str[n++] = buffer[i][j].Char.AsciiChar;
    }
    str[n] = '\0';
    return n;
}

int putstr(int left, int top, int right, int bottom, char *str)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    int i, j, n=0;
    SMALL_RECT r = (SMALL_RECT){left-1, top-1, right-1, bottom-1};
    CHAR_INFO buffer[info.dwSize.Y][info.dwSize.X];

    for(i=0; i<=bottom-top; i++)
        for(j=0; j<=right-left;  j++)
        {
            buffer[i][j].Attributes     = /*str[n]=='\0' ? 0 :*/ __COLOR;
            buffer[i][j].Char.AsciiChar = str[n]=='\0' ? '\0' : str[n++];
        }
    return WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), (PCHAR_INFO)buffer, info.dwSize, (COORD){0, 0}, &r);
}
