//HAUPRPROGRAMM
//KEINE VERSIONSNUMMER
//jeder hat sein eigenes main() damit die Funktionen/Bl�cke, die gerade programmiert werden, aufgerufen werden k�nnen
//Versionsnummer gibts erst ab dem zusamenstellen von mehreren Bl�cken

#include <stdlib.h>
#include <stdio.h>
#include "animation.h"
#include "game.h"
#include "gameclass.h"
#include "globals.h"
#include "graphics.h"
#include "levelselect.h"
#include "logger.h"


//Wenn Grafiken und andere Datein nicht gefunden werden: "Execution Working Dir" in den Project-Options editieren









//TEXTURE coldAnimation       ("daten/texturen/animation/coldAnimation.tga"       ,{900,272},{5,2});          //Animationsdatei
//TEXTURE lavaAnimation       ("daten/texturen/animation/lavaAnimation.tga"       ,{256,256},{4,4});          //Animationsdatei
//TEXTURE lava                ("daten/texturen/animation/lava.tga"                ,{16,64},{1,4});            //Animationsdatei



///OBJEKTE, DIE NUR EINMAL EXISTIEREN UND �BERALL VERWENDET WERDEN:


//Akt. Problem: Level 0, blocked wird nicht zur�ckgesetzt
int main(int argc,char* argv[])
{

    init_window();  //Fenster initialisieren



    bool continueNextLevel = false;
    LEVELSELECT *levelselect = new LEVELSELECT();
    for(;;)
    {

        bool success = true;
        if (continueNextLevel && levelselect->NextLevelAvailable())
            success = levelselect->SwitchLevel(1);
        else
            success = levelselect->Select();

        continueNextLevel = false;

        if (success)
        {
            GAME *game=new GAME({50,50},40,levelselect->GetLevel()->path/*"daten/level/test.lvl"*/);
            if(game->getStatus() != 0)
            {
                char *dateP,*timeP;
                game->getMetaData(&dateP,&timeP);
                logger(1,"Das Level wurde erfolgreich geladen. Levelpfad: %s, Erstelldatum: %s, Erstellzeitpunkt: %s",game->getLevelPath(),dateP,timeP);
            }
            int status=-5;
            do
            {
                status=gameMain(game);
                if(status<0)        logger(1,"Spiel verloren");
                else if(status==0)
                {
                    logger(1,"Spiel gewonnen");
                    continueNextLevel = true;
                    levelselect->GetLevel()->score.setScoreFromGameLog(game->getGameLog());
                }
                else                logger(1,"Spiel abgebrochen");

                if(status==-3)
                    continueNextLevel = true;

                game->clearGameData();  //Alle Daten wieder l�schen
            }while(status==-2);


        }
        else
        {
            logger (true, "LevelSelect canceled, show main menu");
            return 0;
        }
    }

    delete levelselect;
    cleanup();  //Abschlussarbeiten vor dem Programmende
    return 0;

}
