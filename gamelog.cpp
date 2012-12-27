//Für das GAME selbst. Es werden alle Ereignisse abgespeichert unt mit diesem "gamelog" ausgegeben
//Version 1.0
//Jakob Maier

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctime>
#include "definitions.h"
#include "gamelog.h"
#include "graphics.h"
#include "globals.h"
#include "logger.h"


GAMELOG::GAMELOG() :    show(GAMELOGshowBUTTONarea,0,&gamelogIcons,POS{0,3}),
                        hide(GAMELOGhideBUTTONarea,0,&gamelogIcons,POS{0,3}),
                        save(GAMELOGsaveBUTTONarea,0,&gamelogIcons,POS{2,3}),
                        pause(GAMELOGpauseBUTTONarea,0,&gamelogIcons,POS{1,3})
{
    show.assignKeyboardButton(0,'H',0);
    hide.assignKeyboardButton(0,'H',0);
    save.assignKeyboardButton(0,GLFW_KEY_RSHIFT ,0);
    pause.assignKeyboardButton(0,'P',GLFW_KEY_ESC);
    pauseButtonClicked=0;
    pauseButtonEnable=1;


    //playtime=0;
    startTime=clock();
    events=0;
    useravatarmoves=0;

    displayGameLog=1;

    start=NULL;

    if(progress>=100)                                       //Falls viele Boxen auf einmal kommen: mit diesem if wird die Darstellung schöner
        progress=0;

    addEvent(GAMESTART,NONE);
}

GAMELOG::~GAMELOG()                                         //Destruktor
{   GameEvent *p=start,*q;
    while(p!=NULL)
    {   q=p->next;
        free(p);
        p=q;
    }
}

unsigned long GAMELOG::getPlayTime()
{
    return clock()-startTime;/*playtime*(1000.0f/FPS);*/
}

void GAMELOG::EnablePauseButton(bool enable)                //Ativiert/Deaktiviert den Pause-Button
{   pauseButtonEnable=enable;
}


void GAMELOG::run()
{
    //playtime+=(1/FPS);
}

void GAMELOG::addEvent(GameEventType type,DIRECTION richtung=NONE)                 //Event hinzufügen
{
    events++;
    if (type == USERAVATARMOVE)
    {
        useravatarmoves++;
    }

    if(start!=NULL)
    {   if(start->type==type && start->size<GAMELOG_ICONS_PER_BOX)//selber Typ + noch Platz
        {   start->time[(int)(start->size)]=clock()-startTime;
            start->richtung[(int)(start->size)]=richtung;
            start->size++;
            return;
        }
    }

    //Neues Event erstellen:

    GameEvent *neu=(GameEvent*)malloc(sizeof(GameEvent));

    neu->type=type;
    neu->time[0]=clock()-startTime;
    neu->richtung[0]=richtung;
    neu->size=1;


    neu->next=start;                                        //Am Anfang einfügen
    start=neu;

    progress=0;
}



void GAMELOG::getGameEventInformation(int type,char *title,COLOR *farbe)                                 //Gibt Titel und Farbe für ein GameEvent zurück
{
    switch(type)
    {   case GAMESTART:             *farbe=WHITE;            strcpy(title,"Spielstart"); break;          //Das Spiel wurde gestartet
        case USERAVATARMOVE:        *farbe=RED;              strcpy(title,"Spielerbewegung"); break;     //Der Benutzer hat den Avatar bewegt
        case AVATARMOVE:            *farbe={0.5,0.0,0.0};    strcpy(title,"Avatar"); break;              //der Avatar wird bewegt
        case AVATARDEATH:           *farbe=BLACK;            strcpy(title,"Avatar ist Tot"); break;      //Der Avatar ist gestorben

        case KUGELMOVE:             *farbe=BLUE;             strcpy(title,"Kugel"); break;               //Eine Kugel (typ 0) wird bewegt
        case KUGELBLOCKMOVE:        *farbe={0.7,0.2,0.0};    strcpy(title,"Kugel"); break;               //Eine Kugel (typ 1) wird bewegt
        case KUGELTYPECHANGED0:     *farbe=BLUE;             strcpy(title,"Kugelumwandlung"); break;     //Eine Kugel ist jezt Typ 0
        case KUGELTYPECHANGED1:     *farbe={0.7,0.2,0.0};    strcpy(title,"Kugelumwandlung"); break;     //Eine Kugel ist jezt Typ 0
        case KUGELBLOCKEDLAVA:      *farbe={1.0,0.5,0.0};    strcpy(title,"Lava belegt"); break;         //Eine Kugel (typ 0) hat ein Lavafeld geblockt
        case KUGELTARGET:           *farbe=GREEN;            strcpy(title,"Zielfeld belegt"); break;     //Eine Kugel (typ 0) befindet sich jetzt auf einem Zielfeld
        case KUGELDESTROYED:        *farbe={1.0,0.0,0.3};    strcpy(title,"Kugel kaputt"); break;        //Eine Kugel (beide Typen) ist zerstört worden

        case LOCKOPENED:            *farbe={0.0,0.6,0.0};    strcpy(title,"Schloss geöffnet"); break;    //Ein Schloss wurde mit einem Schlüssel geöffnet

        case GAMEOVER:              *farbe=BLACK;            strcpy(title,"GAME OVER"); break;           //Game Over



        default:    error("GAMELOG::print()","Unbekannter Event-Type. type=%d",type);
                    *farbe={0.3,0.2,0.2}; strcpy(title,"Ereignis");
    }
    //#define GAMELOGDARKNING 0.11
    //farbe={farbe.r-farbe.r*(GAMELOGDARKNING*dispNum),*farbe.g-*farbe.g*(GAMELOGDARKNING*dispNum),*farbe.b-*farbe.b*(GAMELOGDARKNING*dispNum)};
    return;
}


void GAMELOG::printBackground()                             //Gibt nur den Linken Rand und den Hintergrund aus
{
    if(!displayGameLog)
    {
        boxTextures.print({{windX-GAMELOGBORDERWITH-movementInfoSize,0},{windX-movementInfoSize,windY}},{2,2},GAMELOGMAINCOLOR);
        boxTextures.print({{windX-movementInfoSize,0},{windX,windY}},{3,GAMELOGBOXTYPE},GAMELOGMAINCOLOR);
    }else
    {   boxTextures.print({{windX-GameLogWidth,0},{windX-GameLogWidth+GAMELOGBORDERWITH,windY}},{2,2},GAMELOGMAINCOLOR);    //Linker Rand
        boxTextures.print({{windX-GameLogWidth+GAMELOGBORDERWITH,0},{windX,windY}},{3,GAMELOGBOXTYPE},GAMELOGMAINCOLOR);    //Hindergrund
    }
}


void getTimeString(char *string,int time)                   //Wandelt eine Zeitangabe (ms) in eine String um
{   if(time<61000)      sprintf(string,"%.1fs",time/1000.0);
    else
    {   time/=1000;
        if(time<3600)     sprintf(string,"%dm %ds",time/60,time%60);
        else
        {   time/=60;
            sprintf(string,"%dh %dm",time/60,time%60);
        }
    }
}


void GAMELOG::print()                                       //Kümmert sich um die Ausgabe
{
    //Abgrenzen des Loggers vom Spiel:
        printBackground();


    if(!displayGameLog)
    {
        if(show.clicked())  displayGameLog=1;               //Wieder einblenden
        show.print();                                       //Show-Button ausgeben
        if(pauseButtonEnable)
        {   if(pause.clicked()) pauseButtonClicked=1;
            pause.print(1);                                     //Pause-Button mit Tastatur anklickbar, wird aber nicht ausgegeben und ist nicht mit Maus aklickbar
        }else
            pauseButtonClicked=0;
        return;
    }

    //Akt. Spielzeit ausgeben:
        normalFont.setFontSize(movementInfoSize-5);             //Schriftgröße ändern
        normalFont.setFontColor(YELLOW);                        //Schriftfarbe ändern

        y=windY-movementInfoSize/2-(movementInfoSize-5)/2;  //aktuelle y-Position
        if(coordInside(mouse,{{GAMELOG_X,y},{windX-movementInfoSize,y+movementInfoSize-5}}))
            normalFont.printf({GAMELOG_X,y},taLEFT,"%dms",clock()-startTime);
        else
        {   char txt[20];
            getTimeString(txt,clock()-startTime);
            normalFont.printf({GAMELOG_X,y},taLEFT,txt);
        }


   //Anzahl der Ereignisse ausgeben:
        normalFont.setFontSize(GameLogTitleFontSize);
            normalFont.printf({GAMELOG_X,GAMELOGPADDING},taLEFT,"%d Ereignisse",events);

    //Buttons:
        if(hide.clicked())  displayGameLog=0;
        hide.print();   //Hide
        if(save.clicked())  MessageBox(NULL,"Die Speicherfunktion kann derzeit leider noch nicht verwendet werden","Funktion noch nicht implementiert",MB_OK|MB_ICONWARNING);
        save.print();   //Save
        if(pauseButtonEnable)
        {   if(pause.clicked()) pauseButtonClicked=1;
            pause.print();  //Pause
        }else
            pauseButtonClicked=0;

    //Ausgabe vorbereiten:
        y-=movementInfoSize;    //Y-Position der ersten Box
        dispNum=0;              //Nummeriert die Boxen durch

        //Zwischenspeicher:
        char title[128];
        COLOR farbe;
        POS spritePos;
        int ypos=y;                         //Beginn der akt. Box
        int xpos=GAMELOG_X;                 //Beginn der Box
        int endXpos=windX-GAMELOGPADDING;   //Ende der Box


    progress+=GAMELOG_SCROLL_SPEED;        //Scroll-Geschindigkeit
    if(progress>100)    //Overflow vermeiden, wenn länger kein scrollen vorkommt
        progress=100;

    //Für alle Events durchgehen:
    GameEvent *p=start;
    while(p!=NULL && y>0 && dispNum<((progress>=100)?9:10))//Sonlange Elemente existieren und diese auch sichtbar wären. Max. 7 Boxen
    {   dispNum++;
        y=ypos;

        getGameEventInformation(p->type,title,&farbe);                                 //Gibt Titel und Farbe für ein GameEvent zurück

        if(dispNum>9)
        {   xpos=GAMELOG_X + GameLogWidth*progress/100;
            endXpos=windX-GAMELOGPADDING + GameLogWidth*progress/100;
            y+=progress*(GAMELOGBOXHEIGHT+GAMELOGPADDING)/100;
        }


        if(p==start && progress<100)
        {   ypos-=progress*(GAMELOGBOXHEIGHT+GAMELOGPADDING)/100;
        }else
            ypos-=GAMELOGBOXHEIGHT + GAMELOGPADDING;

        ///BOX:
            if(colorcmp(farbe,BLACK))
                farbe=WHITE;   //Schriftfarbe
            else //schwarze Box nicht ausgeben
                drawBox({{xpos,y-GAMELOGBOXHEIGHT},{endXpos,y}},GAMELOGBORDERWITH,GAMELOGBOXTYPE,farbe);

        ///Titel:
            normalFont.setFontColor(farbe);
            normalFont.setFontSize(GameLogTitleFontSize);
                normalFont.printf({(int)(((endXpos)-xpos)/2+xpos),(int)(y-(GAMELOGPADDING*1.5f)-GameLogTitleFontSize)},taCENTER,"%s",title);
            normalFont.setFontSize(GameLogInfoFontSize);

        ///Zeitpunkt:
            normalFont.setFontColor(GameLogInfoColor);
            //Mit abdunkeln:   normalFont.setFontColor({GameLogInfoColor.r-GameLogInfoColor.r*(GAMELOGDARKNING*dispNum),GameLogInfoColor.g-GameLogInfoColor.g*(GAMELOGDARKNING*dispNum),GameLogInfoColor.b-GameLogInfoColor.b*(GAMELOGDARKNING*dispNum)});

            if(coordInside(mouse,{{xpos,y-GAMELOGBOXHEIGHT},{endXpos,y}}))    //Maus in der Box
            {   y=y-GAMELOGBOXHEIGHT+GAMELOGPADDING*1.5;
                normalFont.printf({endXpos-GAMELOGPADDING*2,y},taRIGHT,"%dms",p->time[(p->size)-1]);
            }
            else
            {   y=y-GAMELOGBOXHEIGHT+GAMELOGPADDING*1.5;

                char txt[20];
                getTimeString(txt,p->time[(p->size)-1]/* *(1000.0f/FPS)*/);
                normalFont.printf({endXpos-GAMELOGPADDING*2,y},taRIGHT,txt);
            }

        ///Icons:
            x=xpos+GAMELOGPADDING*2;
            //for(int i=(p->size)-1;i>=0;i--)
            for(int i=0;i<p->size;i++)
            {
                spritePos.x=-1;//Kein Icon
                switch(p->type)
                {   case USERAVATARMOVE:
                    case AVATARMOVE:
                    case KUGELMOVE:
                    case KUGELBLOCKMOVE:    if(p->richtung[i]==BEAM)
                                            {   spritePos={0,1};
                                            }else
                                            if(p->richtung[i]!=NONE)
                                            {   spritePos={p->richtung[i]-1,0};
                                            }
                                            break;
                    case KUGELTYPECHANGED0:
                    case KUGELTYPECHANGED1: spritePos={0,2};
                                            break;
                    case KUGELTARGET:       spritePos={1,2};
                                            break;
                    case GAMEOVER:          spritePos={2,2};
                                            break;
                    case AVATARDEATH:       spritePos={1,1};
                                            break;

                    case KUGELDESTROYED:    spritePos={2,1};
                                            break;
                    case LOCKOPENED:        spritePos={3,2};
                                            break;
                    default:                break;
                }
                if(spritePos.x>=0)//Icon vorhanden
                    gamelogIcons.print({{x,y},{x+20,y+20}},spritePos, WHITE, 1.0);
                x+=21;
            }

        p=p->next;
    }

}

int GAMELOG::getxpos()                                 //Gibt die X-Position zurück, ab der sich der Gamelog befindet
{
    if(!displayGameLog)
        return windX-GAMELOGBORDERWITH-movementInfoSize;
    else
        return windX-GameLogWidth;
}


bool GAMELOG::isPauseButtonClicked()                   //Gibt zurück, ob der Pause-Button seit dem letzten Aufruf gedrückt worden ist
{
    if(pauseButtonClicked)
    {   pauseButtonClicked=0;
        return 1;
    }
    return 0;
}
