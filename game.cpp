//Version 0.0
//
//Spielschleife


extern TEXTURE levelanimations;
extern ANIMATIONGROUP animationHandler;


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
            game->setupGameData();                  //Spieldaten aus den Leveldaten laden
            if(!game->isPrepared())                 //Fehler - Spiel kann nicht gespielt werden
            {   return -1;
            }
        }

    ///Variablen initialisieren:
        bool exit=0;
        long loopStart;
        GAMEBACKGROUND gamebg({{0,0},{GAMELOG_X,windY}});   //Hintergrund initialisieren


    ///Builup-Animation durchführen:
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();                 //Grafiken vorbereiten
            gamebg.print(false);                //Hintergrund ausgeben, ohne Shine
            game->printGameLogBackground();     //Gamelog-Bereich markieren
            if(game->runBuildupAnimation())
                exit=1;
        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;

    ///Spezial-Elemnte einblenden:
        game->initBuildupAnimationSpecialElements();        //Für jedes Objekt eine Einblendeanimation generieren und zum Animationshanlder hinzufügen
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();                 //Grafiken vorbereiten
            gamebg.print();                     //Hintergrund ausgeben
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
    {   BUTTON start({{GAMELOG_X+GAMELOGPADDING,windY/2-20},{windX-GAMELOGPADDING,windY/2+20}},0,3,10,CYAN,"Spiel starten",20,YELLOW);
        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();                 //Grafiken vorbereiten
            gamebg.print();                     //Hintergrund ausgeben
            game->printGameLogBackground();     //Gamelog-Bereich markieren

            game->printPreview();
            if(start.clicked()) exit=1;
            start.print();

        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
        exit=0;
    }

gamebg.setColor(YELLOW);


        logger(1,"Game starting...");


    ///
        int movementInfoAniID=0;            //ID mit der die Animation wieder identifiziert werden kann
        int movementInfoType=0;             //Welcher Typ grade im Animationshandler eingestellt ist
        int help;

        movementInfoAniID=animationHandler.add(MOVEMENTINFO,1/*drehen*/,1,0,100,2,1,&levelanimations,{0,11},{{windX-movementInfoSize/2,windY-movementInfoSize/2},{movementInfoSize,-1}});
        prepare_GameLoop();                 //Für die Spiel-/Anzeigeschleife vorbereiten
        do
        {   loopStart=clock();
            prepare_graphics();             //Grafiken vorbereiten


            ///POSITION MIT TASTATUR VERÄNDERN:
            if(glfwGetKey(GLFW_KEY_RIGHT))
            {   game->move(RIGHT);
                gamebg.setSplashColor(RED);
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

            gamebg.print();                     //Hintergrund ausgeben


            game->run();                    //Einen weiteren Simulationsschritt durchführen
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
                animationHandler.run(LEVELEFFECT);  //beamer,
                animationHandler.run(LEVELFIELD);   //lava,


            game->print();                          //Level ausgeben
            animationHandler.print(0);              //Animationen ausgeben


        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung

    logger(1,"Game ended\n");

    if(DEBUG)
    {   //für debugging-zwecke: warten, bis ESC gedrückt wurde
        do
        {   loopStart=clock();
            prepare_graphics();             //Grafiken vorbereiten
            game->print();
        }while(complete_graphics(loopStart,GAMESPEED));    //Abschlussarbeiten und Abbruch-Überprüfung
    }
    return 0;
}



















/// ==================================================================================================================================
/// KLASSE GAMEBACKGROUND ************************************************************************************************************
/// ==================================================================================================================================






GAMEBACKGROUND::GAMEBACKGROUND(AREA _area)
{   area=_area;

    alpha=0;

    final=WHITE;
    old=final;

    splashProgress=0;   //Keine Splash-Farbe
}

void GAMEBACKGROUND::print(bool printshine)
{   gamebackground.print(area,{{0.0,0.0},{(area.b.x-area.a.x)/128,(area.b.y-area.a.y)/128}},WHITE); //Hintergrundbild ausgeben

    if(printshine)
    {   colorProgress+=5;
        if(colorProgress>100)
        {   colorProgress=100;
        }

        if(splashProgress>0)
        {   if(splashDirection==0)
            {   splashProgress+=2;
                if(splashProgress>=100)
                {   splashProgress=100;
                    splashDirection=1;
                }
            }else
            {   splashProgress-=2;
                if(splashProgress<0)    //Fertig
                    splashProgress=0;
            }
        }


        //Zielfarbe bestimmen:
        COLOR target=final;
        if(splashProgress>0)//Farben mischen
        {   target={final.r+(splash.r-final.r)*splashProgress/100.0,final.g+(splash.g-final.g)*splashProgress/100.0,final.b+(splash.b-final.b)*splashProgress/100.0};
        }


        COLOR current;
        if(colorProgress<100 || splashProgress>0)//Noch nicht die neue Farbe
            current={old.r+(target.r-old.r)*colorProgress/100.0,old.g+(target.g-old.g)*colorProgress/100.0,old.b+(target.b-old.b)*colorProgress/100.0};
        else current=target;

        alpha+=0.005;
        if(alpha>1.0)   alpha=1.0;

        shine.print(area,stdTextArea,current,alpha);
    }
}
void GAMEBACKGROUND::setColor(COLOR _final)             //Neue, endgültige Farbe wählen
{   //Zielfarbe bestimmen
    COLOR target=final;
    if(splashProgress>0)//Farben mischen
    {   target={final.r+(splash.r-final.r)*splashProgress/100.0,final.g+(splash.g-final.g)*splashProgress/100.0,final.b+(splash.b-final.b)*splashProgress/100.0};
    }

    old={old.r+(target.r-old.r)*colorProgress/100.0,old.g+(target.g-old.g)*colorProgress/100.0,old.b+(target.b-old.b)*colorProgress/100.0};

    final=_final;
    colorProgress=0;
}

void GAMEBACKGROUND::setSplashColor(COLOR _splash)      //Farbe einstellen, die kurzfristig verwendet werden soll
{
    splash=_splash;
    splashProgress=1;
    splashDirection=0;
}
