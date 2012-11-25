//Version 0.0
//Jakob Maier
//Beinhaltet die Klasse der Spielfigur

extern ANIMATIONGROUP animationHandler;

AVATAR::AVATAR(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS avatarOrigin)
{
    game=gamePointer;
    origin=originPointer;                       //Position, wo das LEvel ausgegeben wird (links unten)
    elsize=elsizePointer;                       //Größe eines Elementes/Feldes
    levelSize=levelSizePointer;                 //Levelgröße

    deathProgress=0;                            //Avatar=nicht tot

    if(*elsize<1)
    {   error("AVATAR::AVATAR()","Fehler - Die Elementgroesse ist zu niedrig - der Inhalt des Pointers wird auf 40 gesetzt- elsize=%d",*elsize);
        *elsize=40;
    }

    buildupImage=0;
    buildupCounter=0;

    position=avatarOrigin;
    movement.moving=0;//Bewegt sich gerade nicht
    movement.progress=0;
    movement.lastRichtung=NONE;
    movement.blocking=0;
    userinput=0;        //Nicht unbedingt notwendig, wird bei move() automatisch gesetzt

}


void AVATAR::print()
{   if(deathProgress!=0)                                                //Avatar=Tot
    {   return;
    }
    game->printMovingObject(&movement,position,TILE_AVATAR);
}




/*bool AVATAR::setupAnimation()//Blendet die Spielfigur langsam ein. Sobald die Animation fertig ist wird 1 übergeben
{
    buildupCounter++;
    if(buildupCounter>=4)       //Counter, bis das nächste Bild angewandt wird
    {   buildupImage++;
        buildupCounter=0;
        if(buildupImage>=(sizeof(avatarBuildupImages)/sizeof(int)))//Ende
        {   buildupImage=0;
            return 1;
        }
    }
    POS pos;

    pos.x=origin->x+position.x* (*elsize);
    pos.y=origin->y+(levelSize->y-position.y-1)* (*elsize);
    levelSprites->print({pos,{pos.x+ (*elsize),pos.y+ (*elsize)}},{avatarBuildupImages[buildupImage]%10,avatarBuildupImages[buildupImage]/10});

    return 0;
}*/


void AVATAR::move(DIRECTION richtung,bool _userinput=0)
{   if(richtung==NONE)  return;                                         //Nicht bewegen (kommt vor, wenn der Avatar am Eis steht und nicht rutscht)
    if(deathProgress>0) return;                                         //Avatar bereits tot

    if(isMoving())
    {   /*Avatar kann sich nicht bewegen weil er bereits in bewegung wird*/
        return;
    }
    if(game->getFieldProperty(OBJ_AVATAR,position)==fx)                 //Avatar fixiert
        return;
    if(_userinput && game->isMoving(OBJ_KUGEL))
    {   /*Avatar kann sich nicht bewegen weil Kugeln in bewegung sind*/
        return;
    }

    POS next=game->getTargetFieldCoord(position,richtung);

    if(game->checkPos(next))    return;                                 //Außerhalb des Spielfeldes

    int limit=game->isWalkable(OBJ_AVATAR,next);                        //Nachbarfeld, auf das sich der Avatar bewegen soll überprüfen

    if(limit>0)
    {   //game->setGameBackgroundSplashColor(RED);                        //Rot werden
        movement.moving=1;
        movement.progress=0;
        movement.richtung=richtung;
        movement.limit=limit;                                           //Limit durch Feld selbst (Kugeln und andere Objekte in diesem Feld werden erst dann beachtet, wenn sie berührt werden können)

        userinput=_userinput;                                           //Damit der Gamelog dann weiß wer dafür verantwortlich war
    }
}


void AVATAR::run()                                                      //Führt einen Simulationsschritt durch
{
    if(deathProgress>0)                                                 //Avatar=Tot
    {   deathProgress+=DIEINGSPEED;
        if(deathProgress>=100)                                          //Jetzt das Spiel beenden
            deathProgress=100;
        return;
    }


    movement.blocking=0;                                                   //Falls der Avatar blockiert wird dieser immer auf moving=1 und -1 gesetzt --> collision-Animation muss verhindert werden können

    if(movement.moving!=0)
    {   if(movement.moving==-1 && propRicCmp(movement.richtung,game->getFieldProperty(OBJ_AVATAR,position))) //Der Avatar prallt (genau jetzt) ab, darf aber nicht zurückkehren, weil der Feldtyp die Bewegung sofort wieder starten würde. (Ansonsten wackelt der Avatar immer hin und her)
        {   //Prüfen, ob das Hinderniss bereits aus dem Weg ist und der Weg fortgesetzt werden darf:
            movement.moving=1;
            //movement.progress=OccupiedLimit-1;//Damit das Zielfeld nicht von diesem Objekt blockiert wird (kommt vor, wenn mehrere Kugeln im Start-Beamer sind und der Avatar über den Zielbeamer fährt)
            movement.blocking=1;
            //Wenn nein, wird automatisch innerhalb dieses if's wieder abgeblockt und im nächsten Durchgang landet man hier
        }else
            movement.progress+=movement.moving*WALKING_SPEED;               //Vor bzw. Zurück Bewegen


        POS next=game->getTargetFieldCoord(position,movement.richtung);



        if(movement.moving==1 && movement.richtung!=BEAM && movement.progress >= Wkey && movement.progress < Wkey+WALKING_SPEED)//Bei genau diesem Schritt könnte ein Schlüssel berührt und das Schloss geöffnert werden
        {   LOCK *touchedLock=game->KeyOnField(next);
            if(touchedLock != NULL)
            {   game->openLock(touchedLock);    //Öffnet (=löscht) das Schloss
                game->addGameLogEvent(LOCKOPENED);                //Ereignis berichten
            }
        }

        if(movement.moving==1 && movement.richtung!=BEAM && movement.progress >= Wkgl && movement.progress < Wkgl+WALKING_SPEED)//Bei genau diesem Schritt könnte angestoßen werden
        {   KUGEL *touchedKugel=game->KugelOnField(next,NULL);

            if(touchedKugel != NULL)                                    //Im nächsten Feld ist eine Kugel
            {   ///IM NÄCHSTEN FELD IST EINE KUGEL --> Es muss etwas passieren
                    POS next2=game->getTargetFieldCoord(next,movement.richtung);    //Feld nach der Kugel

                    if(movement.blocking || /*vorheriger Bedingungsteil: siehe unten*/game->getFieldProperty(OBJ_KUGEL,next)==fx || game->checkPos(next2))         //Kugel fixiert ODER Übernächste Position außerhalb des Spielfeldes
                    {   movement.moving=-1;                             //Der Avatar prallt sofort ab (Die Kugel wird sich nicht bewegen)
                        if(!movement.blocking)
                        {   game->addFieldEffect(position,COLLISION,movement.richtung,AVATARCOLLITIONCOLOR);
                            /*Überprüfen, ob sich ein anderes Objekt gerade in dieses Feld bewegen will, in die der Avatar jetzt zurückkehren muss:*/
                            game->stopMovementsTo(position,Wava); //Alle Objekte nach Wava % abprallen lassen (sofern möglich.)
                        }

                        //Grund, warum oben movement.blocking in der Bedingung steht:
                        //  Wenn der Avatar gegen ein Feld gedrückt wird das nicht betreten werden kann, und sich in diesem Feld eine Kugel befindet,
                        //  Dann würde diese automatisch weggeschoben werden. Das passiert bei jeder Kugel die in ein solches Feld kommt.
                        //  Damit dass verhindert wird kann die Kugel nicht angestoßen werden, wenn der Avatar blockiert.
                        //  Sie kann nur dann angestoßen werden, wenn der Benutzer den Avatar in die entsprechende Richtung steuert. Auch wenn dies die Richtung ist, in die er gedrückt und blockiert wird. So kann und darf die Kugel dann angestoßen werden
                        //
                        //Wann kann es vorkommen:
                        //          07  <--- Hier ist die Kugel
                        //          07
                        //       12 11 01  <--- In dieses Freie Feld soll die Kugel nur dann geschoben werden, wenn der Spieler es will, nicht automatisch
                        //        ^ 25
                        //        |
                        //      Hier ist der Avatar, der gegen das Feld drückt in das die Kugel kommen wird

                    }else
                    {   //Übernächstes Feld überprüfen:

                        int limit2=game->isWalkable(OBJ_KUGEL,next2);   //Wie weit sich die Kugel weiterbewegen kann

                        if(limit2>0)    //Kugel kann sich bewegen -> Anstoßen
                        {   touchedKugel->move(movement.richtung);      //Kugel anstoßen
                        }

                        if(limit2<100)                                  //Die Kugel kann sich nicht ewig weiterbewegen --> Der Avatar prallt irgendwann zurück (darf nicht ins nächste Feld)
                        {   if(movement.limit > Wkgl + limit2)
                            {   movement.limit = Wkgl + limit2;         //Limit setzen
                            }else if(movement.limit >= 100)             //ZB. Die Kugel kann sich 80% bewegen und prallt dann ab. Der Avatar ist aber schon bei 22% --> 102%, obwohl der Avatar zurückprallen muss
                            {   movement.limit=99;
                            }
                        }
                    }
            }
        }


        if(movement.moving==1 && movement.limit<100 && movement.progress>=movement.limit)          //Prüfen, ob er jetzt vlt. abprallt
        {   movement.moving=-1;                                         //Abprallen
            movement.progress=movement.limit;                           //Genau an die Kante setzen
            if(!movement.blocking)
            {   game->addFieldEffect(position,COLLISION,movement.richtung,AVATARCOLLITIONCOLOR);
                /*Überprüfen, ob sich ein anderes Objekt gerade in dieses Feld bewegen will, in die der Avatar jetzt zurückkehren muss:*/
                game->stopMovementsTo(position,Wava); //Alle Objekte nach Wava % abprallen lassen (sofern möglich.)
            }
        }

        ///Prüfen, ob die Bewegung schon abgeschlossen wurde:
        if(movement.moving==-1 && movement.progress<=0)                 //Ins Startfeld zurückgekehrt
        {   movement.moving=0;
            movement.lastRichtung=NONE;
        }
        if(movement.moving==1 && movement.progress>=100)                //Im Zielfeld angekommen
        {   position=next;
            movement.moving=0;
            movement.lastRichtung=movement.richtung;
            game->addGameLogEvent(userinput?USERAVATARMOVE:AVATARMOVE,movement.richtung);     //Ereignis berichten
            userinput=0;
        }

        if(movement.blocking && movement.moving!=-1)    movement.blocking=0;    //Der Avatar hat blockiert, wurde jetzt aber wieder freigegeben
    }
    if(movement.moving==0)                                              //Steht still --> prüfen, ob sich der Avatar auf einem Spezialfeld befindet
    {   switch(game->getFieldProperty(OBJ_AVATAR,position))             //Dieses Feld hat vlt. eine spezielle Eigenschaft
        {   case up: move(UP);      break;
            case dn: move(DOWN);    break;
            case lt: move(LEFT);    break;
            case rt: move(RIGHT);   break;
            case ic: move(movement.lastRichtung);    break;             //Weiter rutschen
            case bm: move(BEAM);    break;                              //Zum Ziel-Beamer versetzen
            case tr: move(turnRight(movement.lastRichtung)); break;     //Nach rechts (90° im Uhrzeigersinn) weiterruthschen
            case tl: move(turnLeft (movement.lastRichtung)); break;     //Nach links (90° gegen Uhrzeigersinn) weiterruthschen
            case dt:{   /*Tödlich*/
                        int feld=game->getField(position);              //Feldtyp herausfinden
                        switch(feld)
                        {   case TILE_LAVA:     game->addFieldEffect(position,AVATARLAVA); break;                /*lava*/
                            default:            error("AVATAR::run()","Ubekanntes, toedliches Feld. Es wird keine Animations ausgegeben. feld: %d",feld);
                        }
                        game->addGameLogEvent(AVATARDEATH);             //Ereignis berichten
                        game->addGameLogEvent(GAMEOVER);                //Ereignis berichten
                        deathProgress=1;                                //Den Avatar zu Tode verurteilen
                    }break;
            case fx: /*do nothing*/ break;
            case nm: /*do nothing*/ break;
            default: error("AVATAR::run()","Unbekannte Feldeigenschaft gefunden. Verarbeite wert als \"nm\" (Normal). fieldproperty=%d",game->getFieldProperty(OBJ_KUGEL,position));
        }
    }
}


void AVATAR::stopMovementTo(POS pos,int limit)                         //Wenn sich der Avatar gerade auf dieses Feld zubewegt: abprallen lassen
{
    if(movement.moving==1)
    {   if(poscmp(pos,game->getTargetFieldCoord(position,movement.richtung)))       //Der Avatar bewegt sich auf dieses Feld zu
        {   if(movement.progress>=limit)
            {   movement.moving=-1;                                     //sofort abprallen
                game->addFieldEffect(position,COLLISION,movement.richtung,AVATARCOLLITIONCOLOR);
            }else
            if(movement.limit>limit)
            {   movement.limit=limit;                                   //Nach dem neuen Limit abprallen
            }
        }
    }
}

bool AVATAR::AvatarOnField(POS pos)                                     //Überprüft, ob der Avatar dieses Feld blockiert
{
    if(movement.moving==0 || movement.progress<=100-OccupiedLimit)      //Avatar befindet sich am eigenen position-Feld
    {   if(poscmp(pos,position))                                        //=gesuchte Position
            return 1;                                                   //ja
    }
    if(movement.moving!=0 && movement.progress>=OccupiedLimit && !movement.blocking/* siehe unten */)   //Avatar befindet sich bereits auf nächstem Feld
    {   if(poscmp(pos,game->getTargetFieldCoord(position,movement.richtung)))       //nächstes Feld=gesuchte Position
            return this;                                                //ja
    }

    //Der Grund, warum oben im if movement.blocking steht:
    //   Wenn der Avatar gegen ein nicht betretbares Feld gedrückt wird und dadurch blockiert, ist er auf genau 22%.(=WC=Wkgl=OccupiedLimit)
    //   Wenn dieses Feld dadurch blockiert werden würde, könnte es von niemand anderen mehr betreten werden. Um das zu vermeiden wird der blocking-Wert eingebaut.
    //
    //Wann kann es vorkommen:
    //          07  <--- Hier ist die Kugel
    //          07
    //       12 11  <--- Hier würde das Feld vom Avatar blockiert werden und die Kugel würde daher im Feld darüber stehenbleiben (und ebenfalls blockieren)
    //        ^ 01  <--- In dieses Freie Feld sollte die Kugel kommen
    //        |
    //      Hier ist der Avatar, der gegen das Feld drückt in das die Kugel kommen wird

    return 0;                                                           //nein
}

bool AVATAR::isMoving()                                                 //gibt zurück, ob sich der Avatar gerade bewegt
{
    if(movement.moving==0 || (movement.moving==-1 && propRicCmp(movement.richtung,game->getFieldProperty(OBJ_AVATAR,position))))
        return 0;
    return 1;
}

