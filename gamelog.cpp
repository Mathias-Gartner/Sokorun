//Für das GAME selbst. Es werden alle Ereignisse abgespeichert unt mit diesem "gamelog" ausgegeben
//Version 1.0
//Jakob Maier

extern TEXTURE gamelogIcons;
extern FONT normalFont;


#define GAMELOG_X (windX-GameLogWidth+GAMELOGBORDERWITH/4+GAMELOGPADDING)    //Position, ab der Ausgaben erlaubt sind


#define GAMELOGBUTTONS_Y windY-movementInfoSize/2-(movementInfoSize-5)/2    //Y-Position für die Gamelog-Buttons
#define GAMELOGshowBUTTONarea AREA{{windX-movementInfoSize,GAMELOGBUTTONS_Y-50},{windX-movementInfoSize+25,GAMELOGBUTTONS_Y-25}}       //Show-Button AREA
#define GAMELOGhideBUTTONarea AREA{{GAMELOG_X,GAMELOGBUTTONS_Y-30},{GAMELOG_X+25,GAMELOGBUTTONS_Y-5}}       //Hide-Button AREA
#define GAMELOGsaveBUTTONarea AREA{{GAMELOG_X+30,GAMELOGBUTTONS_Y-30},{GAMELOG_X+55,GAMELOGBUTTONS_Y-5}}    //Save-Button AREA


GAMELOG::GAMELOG() :    show(GAMELOGshowBUTTONarea,0,&gamelogIcons,POS{0,3}),
                        hide(GAMELOGhideBUTTONarea,0,&gamelogIcons,POS{0,3}),
                        save(GAMELOGsaveBUTTONarea,0,&gamelogIcons,POS{2,3})
{
    playtime=0;
    events=0;

    displayGameLog=1;

    start=NULL;

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

void GAMELOG::run()
{   playtime++;                                             //Weiterer Spielschleifendurchlauf
}

void GAMELOG::addEvent(GameEventType type,DIRECTION richtung=NONE)                 //Event hinzufügen
{
    GameEvent *neu=(GameEvent*)malloc(sizeof(GameEvent));

    neu->type=type;
    neu->time=playtime;
    neu->richtung=richtung;

    events++;
    neu->next=start;                                        //Am Anfang einfügen
    start=neu;
}



void GAMELOG::setGameEventInformation()                                                                 //Gibt Titel und Farbe für ein GameEvent zurück
{
    switch(type)
    {   case GAMESTART:             farbe=WHITE;            strcpy(title,"Spielstart"); break;          //Das Spiel wurde gestartet
        case USERAVATARMOVE:        farbe=RED;              strcpy(title,"Spielerbewegung"); break;     //Der Benutzer hat den Avatar bewegt
        case AVATARMOVE:            farbe={0.5,0.0,0.0};    strcpy(title,"Avatar"); break;              //der Avatar wird bewegt
        case AVATARDEATH:           farbe=BLACK;            strcpy(title,"Avatar ist Tot"); break;      //Der Avatar ist gestorben

        case KUGELMOVE:             farbe=BLUE;             strcpy(title,"Kugel"); break;               //Eine Kugel (typ 0) wird bewegt
        case KUGELBLOCKMOVE:        farbe={0.7,0.2,0.0};    strcpy(title,"Kugel"); break;               //Eine Kugel (typ 1) wird bewegt
        case KUGELTYPECHANGED0:     farbe=BLUE;             strcpy(title,"Kugelumwandlung"); break;     //Eine Kugel ist jezt Typ 0
        case KUGELTYPECHANGED1:     farbe={0.7,0.2,0.0};    strcpy(title,"Kugelumwandlung"); break;     //Eine Kugel ist jezt Typ 0
        case KUGELBLOCKEDLAVA:      farbe={1.0,0.5,0.0};    strcpy(title,"Lava belegt"); break;         //Eine Kugel (typ 0) hat ein Lavafeld geblockt
        case KUGELTARGET:           farbe=GREEN;            strcpy(title,"Zielfeld belegt"); break;     //Eine Kugel (typ 0) befindet sich jetzt auf einem Zielfeld
        case KUGELDESTROYED:        farbe={1.0,0.0,0.3};    strcpy(title,"Kugel kaputt"); break;        //Eine Kugel (beide Typen) ist zerstört worden

        case LOCKOPENED:            farbe={0.0,0.6,0.0};    strcpy(title,"Schloss geöffnet"); break;    //Ein Schloss wurde mit einem Schlüssel geöffnet

        case GAMEOVER:              farbe=BLACK;            strcpy(title,"GAME OVER"); break;           //Game Over



        default:    error("GAMELOG::print()","Unbekannter Event-Type. type=%d",type);
                    farbe={0.3,0.2,0.2}; strcpy(title,"Ereignis");
    }
    //#define GAMELOGDARKNING 0.11
    //farbe={farbe.r-farbe.r*(GAMELOGDARKNING*dispNum),*farbe.g-*farbe.g*(GAMELOGDARKNING*dispNum),*farbe.b-*farbe.b*(GAMELOGDARKNING*dispNum)};
    return;
}

void GAMELOG::printGameEventInformation()                                                               //Gibt eine Informationsbox aus
{
    #define GAMELOGBOXHEIGHT 58
    if(colorcmp(farbe,BLACK))
        farbe=WHITE;   //Schriftfarbe
    else
        drawBox({{GAMELOG_X,y-GAMELOGBOXHEIGHT},{windX-GAMELOGPADDING,y}},GAMELOGBORDERWITH,GAMELOGBOXTYPE,farbe);

    ///Titel:
            normalFont.setFontColor(farbe);
            normalFont.setFontSize(GameLogTitleFontSize);
        normalFont.printf({((windX-GAMELOGPADDING)-GAMELOG_X)/2+GAMELOG_X,y-(GAMELOGPADDING*1.5)-GameLogTitleFontSize},taCENTER,"%s",title);

                normalFont.setFontSize(GameLogInfoFontSize);

    ///Daten:
        #define GameLogInfoColor COLOR{0.7,0.7,0.7}
            normalFont.setFontColor(GameLogInfoColor);
            //Mit abdunkeln:
            //normalFont.setFontColor({GameLogInfoColor.r-GameLogInfoColor.r*(GAMELOGDARKNING*dispNum),GameLogInfoColor.g-GameLogInfoColor.g*(GAMELOGDARKNING*dispNum),GameLogInfoColor.b-GameLogInfoColor.b*(GAMELOGDARKNING*dispNum)});

        if(coordInside(mouse,{{GAMELOG_X,y-GAMELOGBOXHEIGHT},{windX-GAMELOGPADDING,y}}))    //Maus in der Box
        {   y=y-GAMELOGBOXHEIGHT+GAMELOGPADDING*1.5;
            normalFont.printf({windX-GAMELOGPADDING-GAMELOGPADDING*2,y},taRIGHT,"%d",time);
        }
        else
        {   y=y-GAMELOGBOXHEIGHT+GAMELOGPADDING*1.5;
            normalFont.printf({windX-GAMELOGPADDING-GAMELOGPADDING*2,y},taRIGHT,"%.1f s",(time*GAMESPEED)/1000.0);
        }

    ///EVENT-ICONS:
        x=GAMELOG_X+GAMELOGPADDING*2;
        for(int i=icons-1;i>=0;i--)
        {   gamelogIcons.print({{x,y},{x+20,y+20}},iconSet[i]);
            x+=21;
        }
}



void GAMELOG::print()                                       //Kümmert sich um die Ausgabe
{

    if(!displayGameLog)
    {
        boxTextures.print({{windX-GAMELOGBORDERWITH-movementInfoSize,0},{windX-movementInfoSize,windY}},{2,2},GAMELOGMAINCOLOR);
        boxTextures.print({{windX-movementInfoSize,0},{windX,windY}},{3,GAMELOGBOXTYPE},GAMELOGMAINCOLOR);
        if(show.clicked())  displayGameLog=1;               //Wieder einblenden
        show.print();                                       //Show-Button ausgeben
        return;
    }



    //Abgrenzen des Loggers vom Spiel:
        boxTextures.print({{windX-GameLogWidth,0},{windX-GameLogWidth+GAMELOGBORDERWITH,windY}},{2,2},GAMELOGMAINCOLOR);    //Linker Rand
        boxTextures.print({{windX-GameLogWidth+GAMELOGBORDERWITH,0},{windX,windY}},{3,GAMELOGBOXTYPE},GAMELOGMAINCOLOR);    //Hindergrund
    //Akt. Spielzeit ausgeben:
        normalFont.setFontSize(movementInfoSize-5);             //Schriftgröße ändern
        normalFont.setFontColor(YELLOW);                        //Schriftfarbe ändern

        y=windY-movementInfoSize/2-(movementInfoSize-5)/2;  //aktuelle y-Position
        if(coordInside(mouse,{{GAMELOG_X,y},{windX-movementInfoSize,y+movementInfoSize-5}}))
            normalFont.printf({GAMELOG_X,y},taLEFT,"%d",playtime);
        else
            normalFont.printf({GAMELOG_X,y},taLEFT,"%.1fs",(playtime*GAMESPEED)/1000.0);


   //Anzahl der Ereignisse ausgeben:
        normalFont.setFontSize(GameLogTitleFontSize);
            normalFont.printf({GAMELOG_X,GAMELOGPADDING},taLEFT,"%d Ereignisse",events);

    //Buttons:
        if(hide.clicked())  displayGameLog=0;
        hide.print();   //Hide
        if(save.clicked())  MessageBox(NULL,"Die Speicherfunktion kann derzeit leider noch nicht verwendet werden","Funktion noch nicht implementiert",MB_OK|MB_ICONWARNING);
        save.print();   //Save



    //Ausgabe vorbereiten:
        y-=movementInfoSize;    //Y-Position der ersten Box:
        dispNum=0;  //Nummeriert die Boxen durch

        type=Gevt_NONE;//Zum zusammenfassen von Events
        icons=0;
        time=0;


    //Für alle Events durchgehen:
    GameEvent *p=start;
    type=p->type;
    while(p!=NULL && y>0 && dispNum<8)
    {
        ///Ausgeben wenn notwendig:
        if(p->type!=type || icons>=7)//Kann nicht mit alter Box zusamengefasst werden
        {   //BOX-DATEN herausfinden:
                dispNum++;      //Neue Box
                setGameEventInformation();
            //AUSGABE
                printGameEventInformation();
                y-=GAMELOGNOTICESPACING;
            //Neue Event-Daten erstellen:
                type=p->type;
                icons=0;

        }

        ///aktuelles Event anhängen
            //kann entweder das alte sein, oder ein neues das eben erst erstellt wurde
            //ICONS:
            switch(type)
            {   case USERAVATARMOVE:
                case AVATARMOVE:
                case KUGELMOVE:
                case KUGELBLOCKMOVE:    if(p->richtung==BEAM)
                                        {   iconSet[icons]={0,1};
                                        }else
                                        if(p->richtung!=NONE)
                                        {   iconSet[icons]={p->richtung-1,0};
                                        }else icons--; //Es wird kein Icons ausgegeben, aber unten inkrementiert
                                        break;
                case KUGELTYPECHANGED0:
                case KUGELTYPECHANGED1: iconSet[icons]={0,2};
                                        break;
                case KUGELTARGET:       iconSet[icons]={1,2};
                                        break;
                case GAMEOVER:          iconSet[icons]={2,2};
                                        break;
                case AVATARDEATH:       iconSet[icons]={1,1};
                                        break;

                case KUGELDESTROYED:    iconSet[icons]={2,1};
                                        break;
                case LOCKOPENED:        iconSet[icons]={3,2};
                                        break;
                default: icons--;       //Es wird kein Icons ausgegeben, aber unten inkrementiert
            }
            icons++;
            time=p->time,
        p=p->next;
    }

    ///Für die letzte Box:
            //BOX-DATEN herausfinden:
                dispNum++;      //Neue Box
                setGameEventInformation();
            //AUSGABE
                printGameEventInformation();
                y-=GAMELOGNOTICESPACING;

}





