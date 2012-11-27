/*
  Name:     display.h
  Author:   Prof. Dipl.-Ing. Andreas JEDLICKA, HTBL-Hollabrunn
  Date:     21.02.2008
  Description: Bildschirmfunktionen

  Version:  1.0 (f�r gcc 4.2)
*/

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

// Cursorarten
#define NOCURSOR        0
#define NORMALCURSOR    1
#define SOLIDCURSOR     2

// Farben f�r Text und Hintergrund
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

// Cursorposition am Bildschirm von 1,1 bis 80,25 (links oben bis rechts unten) [oder entsprechend der ge�nderten Gr��e]
void textcolor(COLORS color);       // setzen der Textfarbe
void textbackground(COLORS color);  // setzen der Hintergrundfarbe bei der Textausgabe
void setcursortype(int type);       // setzen, wie der Cursors ausschaut
int  gotoXY(int x, int y);          // Cursor am Bildschirm setzen (return 1 bei OK, 0 bei Fehler)
int  whereX(void);                  // auf welcher Position ist der Cursor ?
int  whereY(void);                  // in welcher Zeile ist der Cursor ?
int  sizeX(void);                   // wie Breit ist das Fenster ?
int  sizeY(void);                   // wie Hoch ist das Fenster ?
void clrscr(void);                  // L�scht den gesamten Bildschirm
void clreol(void);                  // L�scht vom Cursor bis zum Zeilenende
void delline(void);                 // L�scht die Zeile, in der der Cursor steht
int  getstr(int left, int top, int right, int bottom, char *str);// Liefert die angezeigten Zeichen aus dem angegebenen Fensterbereich
int  putstr(int left, int top, int right, int bottom, char *str);// Schreibt den angegebenen STRING in den angebenen Fensterbereich und setzt die Hintergrundfarbe
void txtcolor (int farbe);

// ******************** INTERN ******************** //
#include <windows.h>



// Setzt Farben f�r Ausgabe am Bildschirm
void textcolor(COLORS color);

void textbackground(COLORS color);

void txtcolor (int farbe);

// �ndert die Anzeige des Cursors am Bildschirm
void setcursortype(int type);

//
//// Cursor am Bildschirm setzen (return 1 bei OK und 0 bei Fehler)
//int gotoXY(int x, int y)
//{
//    return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){x-1, y-1});
//}
//
//void gotoxy(int x, int y)
//{
//    gotoXY(x+3,y+3);
//}
//
//
//// Liefert die aktuelle Cursorposition zur�ck
//int whereX(void)
//{
//    CONSOLE_SCREEN_BUFFER_INFO info;
//
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//    return info.dwCursorPosition.X+1;
//}
//
//int whereY(void)
//{
//    CONSOLE_SCREEN_BUFFER_INFO info;
//
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//    return info.dwCursorPosition.Y+1;
//}
//
//
//// Liefert die gr��e des Fensters zur�ck
//int sizeX(void)
//{
//    CONSOLE_SCREEN_BUFFER_INFO info;
//
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//    return info.srWindow.Right-info.srWindow.Left+1;
//}
//
//int sizeY(void)
//{
//    CONSOLE_SCREEN_BUFFER_INFO info;
//
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//    return info.srWindow.Bottom-info.srWindow.Top+1;
//}
//
//
//// Bildschirml�schfunktionen
//void clrscr(void)
//{
//    DWORD  dummy;
//    int    size;
//    CONSOLE_SCREEN_BUFFER_INFO info;
//    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//    GetConsoleScreenBufferInfo(hConsoleOut, &info);
//    size = (info.srWindow.Right-info.srWindow.Left+1) * (info.srWindow.Bottom-info.srWindow.Top+1);
//
//    FillConsoleOutputAttribute(hConsoleOut, 0 /*__COLOR*/, size, (COORD){0, 0}, &dummy);
//    FillConsoleOutputCharacter(hConsoleOut, '\0', size, (COORD){0, 0}, &dummy);
//    SetConsoleCursorPosition(hConsoleOut, (COORD){0, 0});
//}
//
//void clreol(void)
//{
//    DWORD  dummy;
//    int    size;
//    CONSOLE_SCREEN_BUFFER_INFO info;
//    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//    GetConsoleScreenBufferInfo(hConsoleOut, &info);
//    size = (info.srWindow.Right-info.srWindow.Left+1) - info.dwCursorPosition.X;
//
//    FillConsoleOutputAttribute(hConsoleOut, 0 /*__COLOR*/, size, info.dwCursorPosition, &dummy);
//    FillConsoleOutputCharacter(hConsoleOut, '\0', size, info.dwCursorPosition, &dummy);
//    SetConsoleCursorPosition(hConsoleOut, info.dwCursorPosition);
//}
//
//void delline(void)
//{
//    DWORD  dummy;
//    int    size;
//    CONSOLE_SCREEN_BUFFER_INFO info;
//    HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
//
//    GetConsoleScreenBufferInfo(hConsoleOut, &info);
//    size = (info.srWindow.Right-info.srWindow.Left+1);
//
//    FillConsoleOutputAttribute(hConsoleOut, 0 /*__COLOR*/, size, (COORD){0,info.dwCursorPosition.Y}, &dummy);
//    FillConsoleOutputCharacter(hConsoleOut, '\0', size, (COORD){0, info.dwCursorPosition.Y}, &dummy);
//    SetConsoleCursorPosition(hConsoleOut, (COORD){0, info.dwCursorPosition.Y});
//}
//
//
//// Liest oder Schreibt Text in einen Bildschirmbereich
//int getstr(int left, int top, int right, int bottom, char *str)
//{
//    CONSOLE_SCREEN_BUFFER_INFO info;
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//
//    int i, j, n=0;
//    SMALL_RECT r = (SMALL_RECT){left-1, top-1, right-1, bottom-1};
//    CHAR_INFO buffer[info.dwSize.Y][info.dwSize.X];
//
//    if(ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), (PCHAR_INFO)buffer, info.dwSize, (COORD){0, 0}, &r))
//    {
//        for(i=0; i<=bottom-top; i++)
//            for(j=0; j<=right-left; j++)
//                str[n++] = buffer[i][j].Char.AsciiChar;
//    }
//    str[n] = '\0';
//    return n;
//}
//
//int putstr(int left, int top, int right, int bottom, char *str)
//{
//    CONSOLE_SCREEN_BUFFER_INFO info;
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//
//    int i, j, n=0;
//    SMALL_RECT r = (SMALL_RECT){left-1, top-1, right-1, bottom-1};
//    CHAR_INFO buffer[info.dwSize.Y][info.dwSize.X];
//
//    for(i=0; i<=bottom-top; i++)
//        for(j=0; j<=right-left;  j++)
//        {
//            buffer[i][j].Attributes     = /*str[n]=='\0' ? 0 :*/ __COLOR;
//            buffer[i][j].Char.AsciiChar = str[n]=='\0' ? '\0' : str[n++];
//        }
//    return WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), (PCHAR_INFO)buffer, info.dwSize, (COORD){0, 0}, &r);
//}

#endif //DISPLAY_H_INCLUDED
