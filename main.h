//Version 0.2
//Jakob Maier

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
    #define DEBUG 1                 //OB DEBUG-BEFEHLE DURCHGEFÜHRT WERDEN SOLLEN
    #define ImgDebug 0              //Ob Debug-Funktionen, die im Grafik-Fenster ausgegeben werden angewandt werden sollen
    #define TIMEDEBUGOUTPUT 0       //Ob die Zeit/Spielschleifenruchlauf ausgegeben werden soll
    const char* const VERSION={"Ver. 0.0"};

    //#include "GLee/GLee.h"
    //#include "GLee/GLee.c"

    #include <GL/glfw.h>
    //#include <windows.h>
    #include <iostream>
    #include <conio.h>
    #include <string.h>
    #include <math.h>           //cos, sin,...
    #include <ctime>         //stoppen der zeit für einen durchlauf der schleife um das Prog computerunaabhängig zu machen
    #include <stdio.h>
    #include <stdlib.h>
    #include "tga.h"                //Laden er Texturen
    #include "display.h"            //Konsolenfarben

    #include "logger.h"             //Errorlog und Infolog
    #include "definitions.h"        //Globale Einstellungen und Typ-Definitionen
    #include "button.h"             //Buttons
    #include "graphics.h"           //Grafik

    #include "logger.cpp"           //Errorlog und Infolog
    #include "definitions.cpp"      //Grundfunktionien
    #include "button.cpp"           //Buttons
    #include "graphics.cpp"         //Grafik


    ///-----------------------------

/*
    class LEVEL;
    class GAME;
    class AVATAR;
    class KUGEL;
*/
    #include "gamelog.h"            //Speichern und Anzeigen von Spieleereignissen (=Lösungsweg)

    #include "animation.h"          //Animations-Klasse
    #include "levelclass.h"         //Level-Klasse
    #include "avatar.h"             //Spielfiguren-Klasse
    #include "kugel.h"              //Kugel-Klasse
    #include "lock.h"               //Schloss-Klasse
    #include "gameclass.h"          //Spiel-Klasse

    ///-----------------------------

    #include "mainmenue.h"          //Hauptmenü
    #include "levelselect.h"        //Levelauswahl
    #include "game.h"               //Spiel
    #include "highscore.h"          //Highscore nach dem Spiel
    #include "editor.h"             //Leveleditor
    #include "score.h"              //Score zwischen Spiel und Editor
    #include "gameguide.h"          //Spielanleitung

    ///-----------------------------

    #include "gamelog.cpp"          //Speichern und Anzeigen von Spieleereignissen (=Lösungsweg)

    #include "animation.cpp"        //Animations-Klasse
    #include "levelclass.cpp"       //Level-Klasse
    #include "gameclass.cpp"        //Spiel-Klasse
    #include "avatar.cpp"           //Spielfiguren-Klasse
    #include "lock.cpp"             //Schloss-Klasse
    #include "kugel.cpp"            //Kugel-Klasse

    ///-----------------------------

    #include "mainmenue.cpp"        //Hauptmenü
    #include "levelselect.cpp"      //Levelauswahl
    #include "game.cpp"             //Spiel
    #include "highscore.cpp"        //Highscore nach dem Spiel
    #include "editor.cpp"           //Leveleditor
    #include "score.cpp"            //Score zwischen Spiel und Editor
    #include "gameguide.cpp"        //Spielanleitung


#endif // MAIN_H_INCLUDED
