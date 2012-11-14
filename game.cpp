//Version 0.0
//
//Spielschleife


extern TEXTURE levelanimations;
extern ANIMATIONGROUP animationHandler;


int gameMain(GAME *game)
{

    ///GAME-Klasse vorbereiten:
        if(game->getStatus() != 1)
        {   error("gameMain()","Das Spiel kann nicht gestartet werden, da keine gueltigen Daten uebergeben wurden. game->status=%d",game->getStatus());
            return -1;
        }
        game->setDisplayOptions({50,50},40);    //Spielfeld platzieren und Größe einstellen
        game->setupGameData();                  //Spieldaten aus den Leveldaten laden


    ///Variablen initialisieren:
        bool exit=0;
        long loopStart;




//Preview:
//        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
//        do
//        {   loopStart=clock();
//            prepare_graphics();                 //Grafiken vorbereiten
//            game->printPreview();
//        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
//        exit=0;









    ///Builup-Animation durchführen:
//        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
//        do
//        {   loopStart=clock();
//            prepare_graphics();                 //Grafiken vorbereiten
//            if(game->runBuildupAnimation())
//                exit=1;
//        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
//        exit=0;
//
//    ///Spezial-Elemnte einblenden:
//
//        game->initBuildupAnimationSpecialElements();        //Für jedes Objekt eine Einblendeanimation generieren und zum Animationshanlder hinzufügen
//        prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
//        do
//        {   loopStart=clock();
//            prepare_graphics();                 //Grafiken vorbereiten
//            game->printFloor();
//
//            animationHandler.run(OBJECTBUILDUP);
//            animationHandler.print(OBJECTBUILDUP);
//            if(animationHandler.getActiveAnimationAnz(OBJECTBUILDUP)<=0)  //Alle Animationen abgeschlossen
//                exit=1;
//        }while(complete_graphics(loopStart,GAMESPEED) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
//        exit=0;
//        animationHandler.remove(OBJECTBUILDUP); //Animationen löschen

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
                game->move(RIGHT);
            if(glfwGetKey(GLFW_KEY_LEFT))
                game->move(LEFT);
            if(glfwGetKey(GLFW_KEY_UP))
                game->move(UP);
            if(glfwGetKey(GLFW_KEY_DOWN))
                game->move(DOWN);


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
