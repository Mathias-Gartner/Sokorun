//Version 0.0
//
//Spielschleife

#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <windows.h>
#include "animation.h"
#include "globals.h"
#include "graphics.h"
#include "logger.h"
#include "pausemenue.h"


int gameMain(GAME *game)    //Rückgabewert: Anzahl der Kugeln die noch gefehlt haben, um das Level zu gewinnen. Wenn -1: Game Over
{

    ///GAME-Klasse vorbereiten:
        {   if(game->getStatus() != 1)
            {   error("gameMain()","Das Spiel kann nicht gestartet werden, da keine gueltigen Daten uebergeben wurden. game->status=%d",game->getStatus());
                return -1;
            }

            POS size=game->getSize();               //Anzahl der Felder in diesem Level

            //Mindestabstand zu allen Rändern: 20
                /*int fieldSize=(GAMELOG_X-40)/size.x; //Größe pro Feld
                  if((windY-40)/size.y < fieldSize)    fieldSize=(windY-40)/size.y;
                */
                int fieldSize=40;

            //resultierenden Abstand zum Außenrand berechnen:
            POS padd={(GAMELOG_X - size.x*fieldSize)/2 , (windY - size.y*fieldSize)/2};



            game->setDisplayOptions(padd,fieldSize);    //Spielfeld platzieren und Größe einstellen
            game->setupGameData();                  //Spieldaten aus den Leveldaten laden
            if(!game->isPrepared())                 //Fehler - Spiel kann nicht gespielt werden
            {   return -1;
            }
        }

    ///Variablen initialisieren:
        bool exit=0;
        long loopStart;

    ///Builup-Animation durchführen:
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();                 //Grafiken vorbereiten
            game->printGameLogBackground();     //Gamelog-Bereich markieren
            if(game->runBuildupAnimation())
                exit=1;
            if(glfwGetKey(GLFW_KEY_SPACE))
                exit=1;
        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;

    ///Spezial-Elemnte einblenden:
        game->initBuildupAnimationSpecialElements();        //Für jedes Objekt eine Einblendeanimation generieren und zum Animationshanlder hinzufügen
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();                 //Grafiken vorbereiten
            game->printGameLogBackground();     //Gamelog-Bereich markieren
            game->printFloor();

            animationHandler.run(OBJECTBUILDUP);
            animationHandler.print(OBJECTBUILDUP);
            if(animationHandler.getActiveAnimationAnz(OBJECTBUILDUP)<=0)  //Alle Animationen abgeschlossen
                exit=1;
        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;
        animationHandler.remove(OBJECTBUILDUP); //Animationen löschen

    ///Preview:
    {   //char pressed=glfwGetKey(GLFW_KEY_RIGHT)||glfwGetKey(GLFW_KEY_LEFT)||glfwGetKey(GLFW_KEY_UP)||glfwGetKey(GLFW_KEY_DOWN);

        BUTTON start({{GAMELOG_X+GAMELOGPADDING,windY/2-20},{windX-GAMELOGPADDING,windY/2+20}},0,3,10,CYAN,"Spiel starten",20,YELLOW);
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();                 //Grafiken vorbereiten
            game->printGameLogBackground();     //Gamelog-Bereich markieren

            game->printPreview();
            if(start.clicked()) exit=1;


            if(glfwGetKey(GLFW_KEY_ESC)||glfwGetKey(GLFW_KEY_ENTER)||glfwGetKey(GLFW_KEY_SPACE)||glfwGetKey(GLFW_KEY_RIGHT)||glfwGetKey(GLFW_KEY_LEFT)||glfwGetKey(GLFW_KEY_UP)||glfwGetKey(GLFW_KEY_DOWN))
                exit=1;

            start.print();

        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;
    }
    game->setGameBackgroundColor({1.0,1.0,0.5});       //Gelbes Hintergrundleuchten


    logger(1,"Game starting...");


    ///
        int movementInfoAniID=0;            //ID mit der die Animation wieder identifiziert werden kann
        int movementInfoType=0;             //Welcher Typ grade im Animationshandler eingestellt ist
        int help;
        bool firstLoopRun=1;                //Wenn die Game-Loop das erste mal durchlaufen wird: keine Usereingabe erlaubt. (Falls sich Kugeln am Anfang auf Spezialfeldern befinden darf sich die Spielfirgur nicht bewegen)
        int status;                         //Zur statusabfrage von GAMECLASS::run() --> zeigt an, ob WIN oder GAME OVER


        movementInfoAniID=animationHandler.add(MOVEMENTINFO,1/*drehen*/,1,0,100,2,1,&levelanimations,{0,11},{{windX-movementInfoSize/2,windY-movementInfoSize/2},{movementInfoSize,-1}});
        prepare_GameLoop();                 //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();             //Grafiken vorbereiten

            if(game->isPauseButtonClicked())    //Pausemenü öffnen
            {   //MessageBox(NULL,"Pause gibts noch nicht","Funktion noch nicht implementiert",MB_OK|MB_ICONWARNING);
                pauseMenue(game);
            }


            if(firstLoopRun!=1)
            {   ///POSITION MIT TASTATUR VERÄNDERN:
                if(glfwGetKey(GLFW_KEY_RIGHT))
                {   game->move(RIGHT);
                }
                if(glfwGetKey(GLFW_KEY_LEFT))
                {   game->move(LEFT);

                }
                if(glfwGetKey(GLFW_KEY_UP))
                {   game->move(UP);

                }
                if(glfwGetKey(GLFW_KEY_DOWN))
                {   game->move(DOWN);
                }
            }


            status=game->run();                     //Einen weiteren Simulationsschritt durchführen
            if(status<=0)   exit=1;                 //Gewonnen bzw. verloren --> Spielschleife abbrechen



            ///Animationen:
            //Movement-Info (drehende Animation) ausgeben:
                help=0;
                if(game->isMoving(OBJ_KUGEL))
                    help=1;
                if(game->isMoving(OBJ_AVATAR))
                    help+=2;
                if(movementInfoType!=help)
                {   animationHandler.setSpritePos(movementInfoAniID,{help,11});
                    movementInfoType=help;
                }
                animationHandler.run(movementInfoAniID);
            //Level-Effekte:
                animationHandler.run(LEVELEFFECT);  //kollission, lavafall,...
                animationHandler.run(LEVELFIELD);   //beamer,lava

            game->print();                          //Level ausgeben
            animationHandler.print(0);              //Animationen ausgeben

            firstLoopRun=0;
        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung

    logger(1,"Game ended\n");


    /*if(DEBUG)
    {   //für debugging-zwecke: warten, bis ESC gedrückt wurde
        do
        {   loopStart=clock();
            prepare_graphics();             //Grafiken vorbereiten
            game->print();
        }while(complete_graphics(loopStart,GAMESPEED));    //Abschlussarbeiten und Abbruch-Überprüfung
    }*/
    return status;
}







