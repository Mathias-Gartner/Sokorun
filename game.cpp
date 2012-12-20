//Version 0.0
//
//Spielschleife

#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <math.h>
#define M_PI		3.14159265358979323846
#include <windows.h>
#include "animation.h"
#include "globals.h"
#include "graphics.h"
#include "logger.h"
#include "pausemenue.h"

//Rückgabewerte:
//  >=0     Anzahl der Kugeln die noch gefehlt haben, um das Level zu gewinnen
//  -1:     Game Over
//  -2:     Level abgebrochen. -> Level neu starten
//  -3:     Level abgebrochen. -> nächstes Level starten
//  -4:     Level abgebrochen. -> Zurück zur Levelauswahl
int gameMain(GAME *game)
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
            game->setupGameData();                      //Spieldaten aus den Leveldaten laden
            if(!game->isPrepared())                     //Fehler - Spiel kann nicht gespielt werden
            {   return -1;
            }

            //if(!game->getLevelfieldAnimationsPrepared())
            //    game->prepareLevelfieldAnimations();    //Animierte Levelfelder hinzufügen
        }

    ///Variablen initialisieren:
        bool exit=0;

    ///Builup-Animation durchführen:
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   prepare_graphics();                 //Grafiken vorbereiten
            game->printGameLogBackground();     //Gamelog-Bereich markieren
            if(game->runBuildupAnimation())
                exit=1;
            if(glfwGetKey(GLFW_KEY_SPACE) || glfwGetKey(GLFW_KEY_ESC))
                exit=1;
        }while(complete_graphics() && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;

    ///Spezial-Elemente einblenden:
        game->initBuildupAnimationSpecialElements();        //Für jedes Objekt eine Einblendeanimation generieren und zum Animationshanlder hinzufügen
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   prepare_graphics();                 //Grafiken vorbereiten
            game->printGameLogBackground();     //Gamelog-Bereich markieren
            game->printFloor(true);             //Spielfläche ausgeben

            animationHandler.run(OBJECTBUILDUP);
            animationHandler.print(OBJECTBUILDUP);
            if(animationHandler.getActiveAnimationAnz(OBJECTBUILDUP)<=0)  //Alle Animationen abgeschlossen
                exit=1;
            if(glfwGetKey(GLFW_KEY_SPACE) || glfwGetKey(GLFW_KEY_ESC))
                exit=1;
        }while(complete_graphics() && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;
        animationHandler.remove(OBJECTBUILDUP); //Animationen löschen

    ///Preview:
    {   BUTTON start({{GAMELOG_X+GAMELOGPADDING,windY/2-20},{windX-GAMELOGPADDING,windY/2+20}},0,3,10,CYAN,"Spiel starten",20,YELLOW);
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   prepare_graphics();                 //Grafiken vorbereiten
            game->printGameLogBackground();     //Gamelog-Bereich markieren

            game->printPreview();
            if(start.clicked()) exit=1;


            if(glfwGetKey(GLFW_KEY_ESC)||glfwGetKey(GLFW_KEY_ENTER)||glfwGetKey(GLFW_KEY_SPACE)||glfwGetKey(GLFW_KEY_RIGHT)||glfwGetKey(GLFW_KEY_LEFT)||glfwGetKey(GLFW_KEY_UP)||glfwGetKey(GLFW_KEY_DOWN))
                exit=1;

            start.print();

        }while(complete_graphics() && !exit);           //Abschlussarbeiten und Abbruch-Überprüfung
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
        double endingProgress=0;            //Nach dem Ende des Spiels wird dieser Counter erhöht. Beim erreichen eines Limits wird die Spielschleife beendet
        int lastEndingProgress=0;           //Hiflsvariable für die End-Animation
        POS hlp;                            //Hilfsvariable für die End-Animation


        movementInfoAniID=animationHandler.add(MOVEMENTINFO,1/*drehen*/,1,0,100,2,MOVEMENTINFO_SPEED,&levelanimations,{0,11},{{windX-movementInfoSize/2,windY-movementInfoSize/2},{movementInfoSize,-1}});
        prepare_GameLoop();                 //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   prepare_graphics();             //Grafiken vorbereiten

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
            if(status<=0)                           //Gewonnen bzw. verloren --> Spielschleife abbrechen
            {   endingProgress+=GAMEEND_SPEED;
                if(endingProgress>=100)
                {   endingProgress=100;
                    exit=1;
                }
            }



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

            if(status==-1)                          //GAME OVER
            {   double alpha=endingProgress*0.006f;

                for(int i=lastEndingProgress;i<(int)endingProgress;i++)
                {   hlp.x=rand()%MaxXsize;
                    hlp.y=rand()%MaxYsize;
                    if(!game->isFieldNull(hlp))
                        game->addFieldEffect(hlp,LAVAFALL);
                }
                lastEndingProgress=endingProgress;



                switchGraphicMode(DRAWING);
                glBegin(GL_QUADS);          //Beginn des zeichenvorganges
                        glColor4f(1.0f, 0.0f, 0.0f,alpha);   glVertex2f(0,0);
                        glColor4f(1.0f, 0.0f, 0.0f, 0.0f);   glVertex2f(0,windY/2);
                        glColor4f(1.0f, 0.0f, 0.0f, 0.0f);   glVertex2f(windX,windY/2);
                        glColor4f(1.0f, 0.0f, 0.0f,alpha);   glVertex2f(windX,0);

                        glColor4f(1.0f, 0.0f, 0.0f,alpha);   glVertex2f(0,windY);
                        glColor4f(1.0f, 0.0f, 0.0f, 0.0f);   glVertex2f(0,windY/2);
                        glColor4f(1.0f, 0.0f, 0.0f, 0.0f);   glVertex2f(windX,windY/2);
                        glColor4f(1.0f, 0.0f, 0.0f,alpha);   glVertex2f(windX,windY);
                glEnd();                    //Ende des Zeichnens

                normalFont.setFontColor(RED);
                normalFont.setFontSize(56);
                int dist=sin(((int)endingProgress%17)/17.0f*M_PI)*60.0f;
                normalFont.printf({( GAMELOG_X)/2,windY/2-28 +dist},taCENTER,"GAME");
                normalFont.printf({(GAMELOG_X)/2,windY/2-28 -dist},taCENTER,"OVER");

            }else if(status==0)                     //GEWONNEN
            {   double fontSize=sin(endingProgress/100.0f*(M_PI*0.5))*150;
                double alpha=endingProgress*0.004f;

                switchGraphicMode(DRAWING);
                glBegin(GL_QUADS);          //Beginn des zeichenvorganges
                        glColor4f(0.0f, 1.0f, 0.0f,alpha);   glVertex2f(0,0);
                        glColor4f(0.0f, 1.0f, 0.0f, 0.0f);   glVertex2f(0,windY/2);
                        glColor4f(0.0f, 1.0f, 0.0f, 0.0f);   glVertex2f(windX,windY/2);
                        glColor4f(0.0f, 1.0f, 0.0f,alpha);   glVertex2f(windX,0);

                        glColor4f(0.0f, 1.0f, 0.0f,alpha);   glVertex2f(0,windY);
                        glColor4f(0.0f, 1.0f, 0.0f, 0.0f);   glVertex2f(0,windY/2);
                        glColor4f(0.0f, 1.0f, 0.0f, 0.0f);   glVertex2f(windX,windY/2);
                        glColor4f(0.0f, 1.0f, 0.0f,alpha);   glVertex2f(windX,windY);
                glEnd();                    //Ende des Zeichnens

                if(endingProgress<20)       alpha=endingProgress/20.0f;
                else if(endingProgress>60)  alpha=1.0f-((endingProgress-60)/40.0f);
                else                        alpha=1.0;

                normalFont.setFontColor({0,0.8,0},alpha);
                normalFont.setFontSize(fontSize);
                normalFont.printf({( GAMELOG_X)/2,windY/2-(int)(fontSize/2)},taCENTER,"GEWONNEN!");
            }else
            if(game->isPauseButtonClicked())    //Pausemenü öffnen
            {   help=pauseMenue(game);
                switch(help)
                {   case 0: break;                    //fortsetzen
                    case 1: status=-2;  exit=1; break;//Level neu starten
                    case 2: status=-3;  exit=1; break;//Nächstes Level
                    case 3: status=-4;  exit=1; break;//Levelselect
                    default: error("gameMain()","Das Pausemenue hat einen ungueltigen Rueckgabeparameter zurueckgegeben. Wert: %d",help);
                }
            }

            firstLoopRun=0;
        }while(complete_graphics() && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung

    logger(1,"Spiel mit Status %d beendet\n",status);


    /*if(DEBUG)
    {   //für debugging-zwecke: warten, bis ESC gedrückt wurde
        do
        {   prepare_graphics();             //Grafiken vorbereiten
            game->print();
        }while(complete_graphics());        //Abschlussarbeiten und Abbruch-Überprüfung
    }*/
    return status;
}







