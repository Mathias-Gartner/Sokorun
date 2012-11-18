//Version 0.0
//Jakob Maier
//Beinhaltet die Klasse der Kugeln

//Version 0.0
//Jakob Maier
//Beinhaltet die Klasse der Spielfigur

KUGEL::KUGEL(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS kugelOrigin,int _type,KUGEL *nextKugel)
{
    next=nextKugel;                             //VERKETTETE KLASSE - Pointer auf das n�chstes Objekt

    game=gamePointer;
    origin=originPointer;                       //Position, wo das LEvel ausgegeben wird (links unten)
    elsize=elsizePointer;                       //Gr��e eines Elementes/Feldes
    levelSize=levelSizePointer;                 //Levelgr��e

    if(*elsize<1)
    {   error("KUGEL::KUGEL()","Fehler - Die Elementgroesse ist zu niedrig - der Inhalt des Pointers wird auf 40 gesetzt- elsize=%d",*elsize);
        *elsize=40;
    }

    buildupImage=0;
    buildupCounter=0;

    position=kugelOrigin;
    type=_type;
    movement.moving=0;                          //Bewegt sich gerade nicht
    movement.progress=0;
    movement.lastRichtung=NONE;
    movement.blocking=0;
}
void KUGEL::print()
{
    game->printMovingObject(&movement,position,type==0?TILE_KUGEL:TILE_KUGELBLOCK);
    if(next!=NULL)  next->print();  //Alle Kugeln ausgeben
}

class KUGEL* KUGEL::getNextObject()                  //Liefert die Adresse des n�chsten Objekts zur�ck
{  return next;
}

/*bool KUGEL::setupAnimation()                        //Blendet die Spielfigur langsam ein. Sobald die Animation fertig ist wird 1 �bergeben
{
    buildupCounter++;
    if(buildupCounter>=4)                           //Counter, bis das n�chste Bild angewandt wird
    {   buildupImage++;
        buildupCounter=0;
        if(buildupImage>=(sizeof(kugelBuildupImages)/sizeof(int)))//Ende
        {   buildupImage=0;
            if(next!=NULL)  next->setupAnimation(); //Alle Kugeln animieren
            return 1;//fertig
        }
    }
    POS pos;

    pos.x=origin->x+position.x* (*elsize);
    pos.y=origin->y+(levelSize->y-position.y-1)* (*elsize);
    leveltiles.print({pos,{pos.x+ (*elsize),pos.y+ (*elsize)}},{kugelBuildupImages[buildupImage]%8,kugelBuildupImages[buildupImage]/8});

    if(next!=NULL)  return next->setupAnimation();  //Alle Kugeln animieren
    return 0;
}*/




void KUGEL::move(DIRECTION richtung)
{   if(richtung==NONE)  return;                     //Nicht bewegen (kommt vor, wenn die Kugel am Eis steht und nicht rutscht)

    POS next=game->getTargetFieldCoord(position,richtung);

    if(game->checkPos(next))    return;             //Au�erhalb des Spielfeldes

    int limit=game->isWalkable(OBJ_KUGEL,next);     //Nachbarfeld, auf das sich die Kugel bewegen soll �berpr�fen

    if(limit>0)
    {
        if(movement.moving==1)  return;             //Die Kugel ist bereits in bewegung - Kann nicht geschoben werden (kommt vor, wenn der Avatar gegen eine sich bewegende Kugel f�hrt, weil die Kugel und er selbst auf einem F�rderband/Eis ist.)
                                                    //Beispiel: Ein Avatar schieb eine Kugel �ber mehrere Eis-Felder. Danach kommt ein Kugel-f�rderband in eine andere Richtung. Daher muss die Kugel in eine andere Richtung weitergeschoben werden
                                                    //          und darf nicht vom Avatar, der sich noch immer am Eis befindet und jetzt gegen die Kugel dr�ckt, angesto�en werden. Der Avatar rutscht weiter und kommt zur gleichen zeit am Kugelf�rderband zu stehen,
                                                    //          wie die Kugel das n�chste Feld nach dem F�rderband betritt
        game->setGameBackgroundSplashColor({0,0.6,1});   //Blau werden

        movement.moving=1;
        movement.progress=0;
        movement.richtung=richtung;
        movement.limit=limit;                       //Limit durch Feld selbst (Kugeln und andere Objekte in diesem Feld werden erst dann beachtet, wenn sie ber�hrt werden k�nnen)
    }
}



void KUGEL::run()                                                       //F�hrt einen Simulationsschritt durch
{
    if(type==-1)    return;                                             //Diese Kugel wurde zum l�schen markiert

    movement.blocking=0;                                                //Falls eine Kugel blockiert wird diese immer auf moving=1 und -1 gesetzt --> collision-Animation muss verhindert werden k�nnen

    if(movement.moving!=0)
    {
        if(movement.moving==-1 && propRicCmp(movement.richtung,game->getFieldProperty(OBJ_KUGEL,position))) //Die Kugel prallt (genau jetzt) ab, darf aber nicht zur�ckkehren, weil der Feldtyp die Bewegung sofort wieder starten w�rde. (Ansonsten wackeln die Kugeln immer hin und her)
        {   //Pr�fen, ob das Hinderniss bereits aus dem Weg ist und der Weg fortgesetzt werden darf:
            movement.moving=1;
            movement.blocking=1;
            //Wenn nein, wird automatisch innerhalb dieses if's wieder abgeblockt und im n�chsten Durchgang landet man hier
        }else
        {   movement.progress+=movement.moving*WALKING_SPEED;               //Vor bzw. Zur�ck Bewegen
        }
        POS targetField=game->getTargetFieldCoord(position,movement.richtung);


        ///Auf Hindernisse �berpr�fen: (limit nicht darauf ausgelegt)
        if(movement.moving==1)
        {
            if(movement.progress >= Wkgl && movement.progress < Wkgl+WALKING_SPEED)//Bei genau diesem Schritt k�nnte eine Kugel ber�hrt werden
            {   KUGEL *touchedKugel=game->KugelOnField(targetField,this);

                if(touchedKugel != NULL)                                    //Im n�chsten Feld ist eine Kugel
                {   ///IM N�CHSTEN FELD IST EINE KUGEL --> Abprallen
                    movement.moving=-1;                         //Die Kugel prallt sofort ab (Die Kugel die sich im n�chsten Feld befindet wird sich nicht bewegen)
                    if(!movement.blocking)
                    {   game->addFieldEffect(position,COLLISION,movement.richtung,(type==0)?KUGELCOLLISIONCOLOR:KUGELBLOCKCOLLISIONCOLOR);
                        /*�berpr�fen, ob sich der Avatar oder ein anderes Objekt gerade in dieses Feld bewegen wollen, in die die Kugel jetzt zur�ckkehren muss:*/
                        /*(Ist der Fall, wenn der Avatar die Kugel angesto�en hat oder sich eine andere Kugel in das Feld bewegt)*/
                        game->stopMovementsTo(position,Wkgl); //Alle Objekte nach WK % abprallen lassen (sofern m�glich.)
                    }
                }
            }

            if(movement.blocking||(movement.progress >= Wava && movement.progress < Wava+WALKING_SPEED))//Bei genau diesem Schritt k�nnte der Avatar ber�hrt werden
            {   if(game->AvatarOnField(targetField))                  //Im n�chsten Feld ist der Avatar
                {   ///IM N�CHSTEN FELD IST DER AVATAR --> Abprallen
                    movement.moving=-1;                                     //Die Kugel prallt sofort ab (Der Avatar der sich im n�chsten Feld befindet wird sich nicht bewegen)

                    /*�berpr�fen, ob sich der Avatar oder ein anderes Objekt gerade in dieses Feld bewegen wollen, in die die Kugel jetzt zur�ckkehren muss:*/
                    if(!movement.blocking)
                    {   game->stopMovementsTo(position,Wkgl); //Alle Objekte nach WK % abprallen lassen (sofern m�glich.)
                    }
                }
            }
        }

        ///Auf Abprallen wegen Feldtyp �berpr�fen: (limit bereits bekannt)
        if(movement.moving==1 && movement.limit<100 && movement.progress>=movement.limit)          //Pr�fen, ob die Kugel jetzt vlt. abprallt
        {   movement.moving=-1;                                         //Abprallen
            movement.progress=movement.limit;                           //Genau an die Kante setzen
            if(!movement.blocking)
            {   game->addFieldEffect(position,COLLISION,movement.richtung,(type==0)?KUGELCOLLISIONCOLOR:KUGELBLOCKCOLLISIONCOLOR);
                game->stopMovementsTo(position,Wkgl); //Alle Objekte nach WK % abprallen lassen (sofern m�glich.)
            }
        }


        ///Pr�fen, ob die Bewegung schon abgeschlossen wurde:
        if(movement.moving==-1 && movement.progress<=0)                 //Ins Startfeld zur�ckgekehrt
        {   movement.moving=0;
            movement.lastRichtung=NONE;                                 //Abgeprallt --> letzte Richtung war nicht m�glich
        }
        if(movement.moving==1 && movement.progress>=100)                //Im Zielfeld angekommen
        {   position=targetField;
            game->addGameLogEvent(type==0?KUGELMOVE:KUGELBLOCKMOVE,movement.richtung);         //Ereignis berichten
            movement.moving=0;
            movement.lastRichtung=movement.richtung;
            int feld=game->getField(position);
            if(feld==TILE_TYPECHANGE || feld==TILE_TYPECHANGEICE)                             //Typumwandlung
            {   if(type==0) type=1;
                else        type=0;
                game->addGameLogEvent(type==0?KUGELTYPECHANGED0:KUGELTYPECHANGED1);           //Ereignis berichten
            }

            if(type==0 && (feld==TILE_TARGET || feld==TILE_FIXEDTARGET))                       //Zielfeld
            {   game->addGameLogEvent(KUGELTARGET);                     //Ereignis berichten
                game->setGameBackgroundSplashColor(GREEN);        //Rot werden
            }
        }
    }
    if(movement.moving==0)                                              //Steht still --> pr�fen, ob sich der Avatar auf einem Spezialfeld befindet
    {   switch(game->getFieldProperty(OBJ_KUGEL,position))              //Dieses Feld hat vlt. eine spezielle Eigenschaft
        {   case up: move(UP);      break;
            case dn: move(DOWN);    break;
            case lt: move(LEFT);    break;
            case rt: move(RIGHT);   break;
            case ic: move(movement.lastRichtung);    break;             //Weiter rutschen
            case bm: move(BEAM);    break;                              //Zum Ziel-Beamer versetzen
            case tr: move(turnRight(movement.lastRichtung)); break;     //Nach rechts (90� im Uhrzeigersinn) weiterruthschen
            case tl: move(turnLeft (movement.lastRichtung)); break;     //Nach links (90� gegen Uhrzeigersinn) weiterruthschen
            case dt:{   /*T�dlich*/
                        int feld=game->getField(position);              //Feldtyp herausfinden
                        switch(feld)
                        {   case TILE_LAVA: if(type==0) game->addFieldEffect(position,KUGELLAVA);                /*lava*/
                                            else {      game->makeLavaSecure(position);                         //Lavafeld mit Block bef�llen (Dezimalwert des Spielfeldes �ndern. Muss beim Neustart des Levels wieder R�ckg�ngig gemacht werden)
                                                        game->addFieldEffect(position,LAVAFALL);
                                                        game->addGameLogEvent(KUGELBLOCKEDLAVA);                //Ereignis berichten
                                                } break;
                            default:        error("KUGEL::run()","Ubekanntes, toedliches Feld. Es wird keine Animations ausgegeben. feld: %d",feld);
                        }
                        game->addGameLogEvent(KUGELDESTROYED);          //Ereignis berichten
                        game->setGameBackgroundSplashColor(RED);        //Rot werden
                        position={-1,-1};
                        type=-1;                                        //Diese Kugel zum l�schen markieren
                    }break;
            case fx: /*do nothing*/ break;
            case nm: /*do nothing*/ break;
            default: error("KUGEL::run()","Unbekannte Feldeigenschaft gefunden. Verarbeite wert als \"nm\" (Normal). fieldproperty=%d",game->getFieldProperty(OBJ_KUGEL,position));
        }
    }
    if(next != NULL)  next->run();                                      //N�chste Kugel auch simulieren
}


KUGEL* KUGEL::KugelOnField(POS pos,KUGEL* ignore)                       //�berpr�ft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zur�ckgegeben
{
    if(ignore!=this)
    {   if(movement.moving==0 || movement.progress<=100-OccupiedLimit)  //Kugel befindet sich am position-Feld
        {   if(poscmp(pos,position))                                    //=gesuchte Position
                return this;                                            //Kugeladresse zur�ckgeben
        }
        if(movement.moving!=0 && movement.progress>=OccupiedLimit && !movement.blocking)      //Kugel befindet sich bereits auf n�chstem Feld
        {   if(poscmp(pos,game->getTargetFieldCoord(position,movement.richtung)))       //n�chstes Feld=gesuchte Position
                return this;                                            //Kugeladresse zur�ckgeben
        }
    }
    if(next!=NULL)  return next->KugelOnField(pos,ignore);              //n�chste Kugel �berpr�fen
    else            return NULL;                                        //letzte Kugel --> keine �bereinstimmung gefunden
}

void KUGEL::stopMovementsTo(POS pos,int limit)                          //Wenn sich eine Kugel gerade auf dieses Feld zubewegt: abprallen lassen
{
    if(movement.moving==1)
    {   if(poscmp(pos,game->getTargetFieldCoord(position,movement.richtung)))       //Der Avatar bewegt sich auf dieses Feld zu
        {   if(movement.progress>=limit)
            {   movement.moving=-1;                                     //sofort abprallen
                game->addFieldEffect(position,COLLISION,movement.richtung,(type==0)?KUGELCOLLISIONCOLOR:KUGELBLOCKCOLLISIONCOLOR);
            }else
            if(movement.limit>limit)
            {   movement.limit=limit;                                   //Nach dem neuen Limit abprallen
            }
        }
    }
    if(next!=NULL) next->stopMovementsTo(pos,limit);                    //N�chste Kugel
}

void KUGEL::setNextObject(KUGEL* nextPointer)                           //�ndert den Zeiger des n�chsten Elements
{   next=nextPointer;
}

bool KUGEL::isMoving()                                                  //gibt zur�ck, ob sich der Avatar gerade bewegt
{
    if(movement.moving==0 || (movement.moving==-1 && propRicCmp(movement.richtung,game->getFieldProperty(OBJ_KUGEL,position))))
    {   if(next!=NULL)  return next->isMoving();
        else            return 0;
    }
    return 1;
}

int KUGEL::getType()                                                    //Gibt den Kugeltyp zur�ck
{   return type;
}
