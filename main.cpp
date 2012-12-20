//HAUPRPROGRAMM
//KEINE VERSIONSNUMMER
//jeder hat sein eigenes main() damit die Funktionen/Blöcke, die gerade programmiert werden, aufgerufen werden können
//Versionsnummer gibts erst ab dem zusamenstellen von mehreren Blöcken

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



///OBJEKTE, DIE NUR EINMAL EXISTIEREN UND ÜBERALL VERWENDET WERDEN:


                                                                       //Zur Verwendung mit der Maus

int main(int argc,char* argv[])
{

    init_window();  //Fenster initialisieren





//    LEVELSELECT *levelselect = new LEVELSELECT();
//    for(;;)
//    {
//
//        int success = levelselect->Select();
//
//        if (success)
//        {

    char path[256];
    int lvlnummer=4;

    int gamestatus;
    GAME *game=new GAME({50,50},40,"");//"/*levelselect->GetLevelPath()*/"daten/level/test.lvl");
    do
    {   lvlnummer++;
        sprintf(path,"daten/level/%d.lvl",lvlnummer);
        game->loadLevel(path);
        if(game->getStatus() != 0)
        {
            char *dateP,*timeP;
            game->getMetaData(&dateP,&timeP);
            logger(1,"Das Level wurde erfolgreich geladen. Levelpfad: %s, Erstelldatum: %s, Erstellzeitpunkt: %s",game->getLevelPath(),dateP,timeP);
        }
        do
        {   gamestatus=gameMain(game);
            if(gamestatus==-1)        logger(1,"Spiel verloren");
            else if(gamestatus==0)  logger(1,"Spiel gewonnen");
            else                logger(1,"Spiel abgebrochen");
            game->clearGameData();  //Alle Daten wieder löschen
        }while(gamestatus==-2);//Level neu starten
    }while(1);//gamestatus==-3);//Nächstes Level

    cleanup();  //Abschlussarbeiten vor dem Programmende
    return 0;

}
