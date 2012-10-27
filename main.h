//Version 0.1
//Jakob Maier

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

    #define DEBUG 1         //OB DEBUG-BEFEHLE DURCHGEFÜHRT WERDEN SOLLEN
    const char* const VERSION={"Ver. 0.0"};






    #include <GL/glfw.h>
    //#include <windows.h>
    #include <iostream>
    #include <conio.h>
    #include <string.h>
    #include <math.h>        //cos sin M_PI .... sin und cos liefern rad werte zurück!!!
    #include <ctime>         //stoppen der zeit für einen durchlauf der schleife um das Prog computerunaabhängig zu machen
    #include <stdio.h>
    #include <stdlib.h>
    #include "tga.h"         //Laden er Texturen
    #include "display.h"     //Konsolenfarben

    #include "logger.h"             //Errorlog und Infolog
    #include "logger.cpp"           //

    #include "definitions.h"        //Globale Einstellungen und Typ-Definitionen
    #include "definitions.cpp"      //Grundfunktionien

    #include "graphics.h"           //Grafik
    #include "graphics.cpp"




#endif // MAIN_H_INCLUDED
