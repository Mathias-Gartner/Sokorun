//Beinhaltet das Pausenmenü
//Jakob Maier

#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include "pausemenue.h"


//Rückgabewerte:
// 0 - Spiel fortsetzen
// 1 - Level neu starten
// 2 - Nächstes Level
// 3 - Levelselect aufrufen
int pauseMenue(GAME *game)              //Pausemenü
{
    game->EnablePauseButton(0);         //Pause-Button deaktivieren

    int exitStatus=-1;                  //=Rückgabewert




//    //Darstellungs-Einstellungen, bovor dise verändert werden:
//        POS OriginOutputPos;
//        int OriginElsize;
//        game->getDisplayOptions(&OriginOutputPos,&OriginElsize);
//
//    //Temporäre Position der Levelausgabe:
//        POS outputPos=OriginOutputPos;
//        int elsize=OriginElsize;
//
//        int progress=0;
//
//    ///LEVEL verkleinern
//        do
//        {   prepare_graphics();             //Grafiken vorbereiten
//
//            progress+=13;
//            if(progress>100)    progress=100;
//            elsize      = 18 + (OriginElsize-18)*(100-progress)/100;
//
//            outputPos.x = (100-progress)*OriginOutputPos.x/100;
//            outputPos.y = (100-progress)*OriginOutputPos.y/100;
//
//            game->setDisplayOptions(outputPos,elsize);
//
//            game->print();
//        }while(complete_graphics() && progress<100);    //Abschlussarbeiten und Abbruch-Überprüfung


        #define PAUSEMENUEarea AREA{{150,175},{GAMELOG_X-150,windY-175}}                //Größe des Pausemenüs

        #define PAUSEMENUE_CENTERX (PAUSEMENUEarea.a.x+(PAUSEMENUEarea.b.x-PAUSEMENUEarea.a.x)/2)
        #define PAUSEMENUE_BUTTONHEIGHT 40
        #define PAUSEMENUE_BUTTONWIDTH  280
        #define PAUSEMNEUE_BUTTONBGCOLOR COLOR{0.5,0.5,0.5}
        #define PAUSEMNEUE_BUTTONFTCOLOR COLOR{0.8,0.8,0.8}

    ///PAUSEMENÜ

        int y=PAUSEMENUEarea.b.y-110;

        BUTTON restart({{(PAUSEMENUE_CENTERX-PAUSEMENUE_BUTTONWIDTH),y},{(PAUSEMENUE_CENTERX+PAUSEMENUE_BUTTONWIDTH),(y+PAUSEMENUE_BUTTONHEIGHT)}},0,4,20,PAUSEMNEUE_BUTTONBGCOLOR,"Neu starten",20,PAUSEMNEUE_BUTTONFTCOLOR);
        y-=(PAUSEMENUE_BUTTONHEIGHT*1.3);
        BUTTON nextlevel({{(PAUSEMENUE_CENTERX-PAUSEMENUE_BUTTONWIDTH),y},{(PAUSEMENUE_CENTERX+PAUSEMENUE_BUTTONWIDTH),(y+PAUSEMENUE_BUTTONHEIGHT)}},0,4,20,PAUSEMNEUE_BUTTONBGCOLOR,"Nächstes Level",20,PAUSEMNEUE_BUTTONFTCOLOR);
        y-=(PAUSEMENUE_BUTTONHEIGHT*1.3);
        BUTTON levelselect({{(PAUSEMENUE_CENTERX-PAUSEMENUE_BUTTONWIDTH),y},{(PAUSEMENUE_CENTERX+PAUSEMENUE_BUTTONWIDTH),(y+PAUSEMENUE_BUTTONHEIGHT)}},0,4,20,PAUSEMNEUE_BUTTONBGCOLOR,"Zur Levelauswahl",20,PAUSEMNEUE_BUTTONFTCOLOR);
        y-=(PAUSEMENUE_BUTTONHEIGHT*1.8);
        BUTTON cancel({{(PAUSEMENUE_CENTERX-PAUSEMENUE_BUTTONWIDTH),y},{(PAUSEMENUE_CENTERX+PAUSEMENUE_BUTTONWIDTH),(y+PAUSEMENUE_BUTTONHEIGHT)}},0,4,20,PAUSEMNEUE_BUTTONBGCOLOR,"Zurück zum Spiel",20,PAUSEMNEUE_BUTTONFTCOLOR);

        cancel.assignKeyboardButton(0,GLFW_KEY_SPACE,GLFW_KEY_ESC);

        do
        {   prepare_graphics();             //Grafiken vorbereiten

            game->print();



            drawBox(PAUSEMENUEarea,40,3,WHITE);
            normalFont.setFontColor(RED);
            normalFont.setFontSize(30);
            normalFont.printf({PAUSEMENUE_CENTERX,PAUSEMENUEarea.b.y-48},taCENTER,"Pause");

            if(restart.clicked())       exitStatus=1;
            restart.print();
            if(nextlevel.clicked())     exitStatus=2;
            nextlevel.print();
            if(levelselect.clicked())   exitStatus=3;
            levelselect.print();
            if(cancel.clicked())        exitStatus=0;
            cancel.print();



        }while(complete_graphics() && exitStatus<0);    //Abschlussarbeiten und Abbruch-Überprüfung



//    ///LEVEL vergrößern
//        do
//        {    prepare_graphics();             //Grafiken vorbereiten
//
//            progress-=13;
//            if(progress<0)    progress=0;
//            elsize      = 18 + (OriginElsize-18)*(100-progress)/100;
//
//            outputPos.x = (100-progress)*OriginOutputPos.x/100;
//            outputPos.y = (100-progress)*OriginOutputPos.y/100;
//
//            game->setDisplayOptions(outputPos,elsize);
//
//            game->print();
//        }while(complete_graphics() && progress>0);    //Abschlussarbeiten und Abbruch-Überprüfung
//
//
//    game->setDisplayOptions(OriginOutputPos,OriginElsize);

    game->EnablePauseButton(1);         //Pause-Button aktivieren
    return exitStatus;
}
