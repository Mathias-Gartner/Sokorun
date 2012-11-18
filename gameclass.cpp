extern TEXTURE levelanimations,crashAnimationX,crashAnimationY,lavafall,gamebackground,shine;
extern ANIMATIONGROUP animationHandler;


void GAME::setupGameData()
{   if(prepared)
    {   error("GAME::setupGameData()","Das Spiel darf nicht aufbereitet werden da zuvor die bereit existierenden Daten zur�ckgesetzt und freigegeben werden muessen. Dies wurde an dieser Stelle noch nicht ausprogrammiert");
        //Hier: Leeren/Freigeben aller Daten
        return;
    }
    gamelog=new class GAMELOG();           //Gamelog erzeugen


    avatar=new class AVATAR(this,&origin,&elsize,&size,avatarOrigin);

    ///Verkettete Kugelklasse generieren:
        KUGELorigin *k=kugelOriginStart;    //Alle Kugeln aus den Leveldaten durchgehen
        while(k!=NULL)
        {   kugelStartPointer=new class KUGEL(this,&origin,&elsize,&size,k->origin,k->type,/*next-pointer=startpointer*/kugelStartPointer);   //Am Anfang einf�gen
            k=k->next;
        }
    ///Verkettete Schlosslasse generieren:
        LOCKorigin *l=lockOriginStart;    //Alle Schl�sser aus den Leveldaten durchgehen
        while(l!=NULL)
        {   lockStartPointer=new class LOCK(this,&origin,&elsize,&size,l->lock,l->key,l->color,/*next-pointer=startpointer*/lockStartPointer);   //Am Anfang einf�gen
            l=l->next;
        }

    prepared=1;
}


void GAME::initBuildupAnimationSpecialElements()    //Animation-Handler aktualisieren um Overlay-Elemente animiert aufzubauen
{
    POS pos;
    pos.x=origin.x+avatarOrigin.x* elsize;
    pos.y=origin.y+(size.y-avatarOrigin.y-1)* elsize;
    animationHandler.add(OBJECTBUILDUP,2,1, 36,39, 3,6/*speed*/,&levelanimations,POS{-1,-1},{pos,{pos.x+elsize,pos.y+elsize}});

    KUGELorigin *p=kugelOriginStart;
    while(p!=NULL)
    {   pos.x=origin.x+p->origin.x* elsize;
        pos.y=origin.y+(size.y-p->origin.y-1)* elsize;
        if(p->type==1)  animationHandler.add(OBJECTBUILDUP,2,1, 28,31, 3,6/*speed*/,&levelanimations,POS{-1,-1},{pos,{pos.x+elsize,pos.y+elsize}});//Braune Kugel (Typ 1)
        else            animationHandler.add(OBJECTBUILDUP,2,1, 32,35, 3,6/*speed*/,&levelanimations,POS{-1,-1},{pos,{pos.x+elsize,pos.y+elsize}});
        p=p->next;
    }
    return;
}


GAME::~GAME()
{   //Der LEVEL-Destruktor wird ebenfalls aufgerufen
    if(prepared)
    {   delete gamelog;

        delete avatar;

        ///Die verketteten Objekte der Klasse KUGEL l�schen:
            KUGEL *k=kugelStartPointer,*lk;
            while(k!=NULL)
            {   lk=k->getNextObject();
                delete k;
                k=lk;
            }
            kugelStartPointer=NULL;
        ///Die verketteten Objekte der Klasse LOCK l�schen:
            LOCK *l=lockStartPointer,*ll;
            while(l!=NULL)
            {   ll=l->getNextObject();
                delete l;
                l=ll;
            }
            lockStartPointer=NULL;
    }
}

POS GAME::getTargetFieldCoord(POS position,DIRECTION richtung)   //Gibt die Koorinaten des Nachbarfeldes, abh�ngig von der Richtung, zur�ck
{
    switch(richtung)
    {   case UP:    return POS{position.x,position.y-1};
        case DOWN:  return POS{position.x,position.y+1};
        case LEFT:  return POS{position.x-1,position.y};
        case RIGHT: return POS{position.x+1,position.y};
        case BEAM:  return getTargetBeamer();
        default: error("getFieldCoord()","Die uebergebene Richtung ist ungueltig. Es wird die Position (-1x-1) zurueckgegeben. richtung=%d",richtung);
    }
    return POS{-1,-1};//Fehler
}

void GAME::printMovingObject(MOVEMENT *movement,POS position,int spriteNum)      //Gibt ein Objekt am Spielfeld aus, dass sich darauf bewegen kann
{
    static POS pos;
    pos.x=origin.x+position.x* elsize;
    pos.y=origin.y+(size.y-position.y-1)* elsize;
    //Je nach Bewegung das Objekt von seiner Urpsr�nglichen Position wegbewegen:
    if(movement->moving!=0)//in Bewegung
    {   switch(movement->richtung)
        {   case UP:    pos.y+= elsize*movement->progress/100; break;
            case DOWN:  pos.y-= elsize*movement->progress/100; break;
            case LEFT:  pos.x-= elsize*movement->progress/100; break;
            case RIGHT: pos.x+= elsize*movement->progress/100; break;



            case BEAM:  {   //Einschw�rtzen an der Startposition:
                            leveltiles.print({pos,{pos.x+ elsize,pos.y+ elsize}},{spriteNum%8,spriteNum/8},COLOR{1-(movement->progress/100.0f),1-(movement->progress/100.0f),1-(movement->progress/100.0f)});
                            //Aufhellen an der Zielposition:
                            pos=getTargetBeamer();//Zielfeld herausfinden
                            pos.x=origin.x+pos.x*elsize;
                            pos.y=origin.y+(size.y-pos.y-1)*elsize;
                            leveltiles.print({pos,{pos.x+ elsize,pos.y+ elsize}},{spriteNum%8,spriteNum/8},COLOR{(movement->progress/100.0f),(movement->progress/100.0f),(movement->progress/100.0f)});
                            return;
                        }break;


            default:    error("AVATAR::print()","Der Avatar ist zwar in Bewegung, die Richtung ist aber ungueltig. Die Bewegung wird abgebrochen. richtung=%d",movement->richtung);
                        movement->moving=0;
        }
    }
    leveltiles.print({pos,{pos.x+elsize,pos.y+elsize}},{spriteNum%8,spriteNum/8});

    if(ImgDebug)
    {
        if(movement->moving==0 || movement->progress<=100-OccupiedLimit)
        {   marker(position,YELLOW);
        }
        if(movement->moving!=0 && movement->progress>=OccupiedLimit)
        {   marker(getTargetFieldCoord(position,movement->richtung),YELLOW);
        }
    }


}


void GAME::addFieldEffect(POS position,FIELDEFFECT effect,DIRECTION richtung=NONE,COLOR color=WHITE)//Ezreugt eine Animation
{
    ///Effekte, die durch eigene Unterprogramme realisiert wurden:
    switch(effect)
    {   case COLLISION:     collision(position,richtung,color); return;
    }

    AREA fieldCoord={origin.x+(position.x)*elsize,origin.y+(size.y-position.y-1)*elsize,{0,0}};
    AREA fieldCoordDouble={{origin.x+(position.x-0.5)*elsize,origin.y+(size.y-position.y-0.5-1)*elsize},{0,0}};  //F�r eine doppelt so gro�e Ausgabe
    fieldCoord.b        ={fieldCoord.a.x        +elsize     ,fieldCoord.a.y         +elsize     };
    fieldCoordDouble.b  ={fieldCoordDouble.a.x  +elsize*2   ,fieldCoordDouble.a.y   +elsize*2   };



    switch(effect)
    {
        case LAVAFALL:      animationHandler.add(LEVELEFFECT,2,0,0,9,0,6/*speed*/,&lavafall,{-1,-1},fieldCoordDouble);  break;
        case AVATARLAVA:    animationHandler.add(LEVELEFFECT,2,0,24,27,0,2/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        case KUGELLAVA:     animationHandler.add(LEVELEFFECT,2,0,20,23,0,2/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        default:    error("GAME::addFieldEffect()","Unbekannter Anitamionseffekt uebergeben. Animation wird nicht durchgefuehrt. effect=%d",effect);
    }
}

void GAME::collision(POS position,DIRECTION richtung,COLOR color)                   //Erzeugt eine Aufprall-Animation
{
    TEXTURE *p=&crashAnimationX;

    position=getTargetFieldCoord(position,richtung);
    position.x=origin.x+position.x* elsize;
    position.y=origin.y+(size.y-position.y-1)* elsize;

    AREA output;//={position,{position.x+elsize,position.y+elsize}};
    switch(richtung)
    {   case UP:    output={{position.x         ,position.y},       {position.x+elsize,position.y+elsize/2}}; p=&crashAnimationY;   break;
        case DOWN:  output={{position.x         ,position.y+elsize},{position.x+elsize,position.y+elsize/2}}; p=&crashAnimationY;   break;
        case LEFT:  output={{position.x+elsize/2,position.y},       {position.x+elsize,position.y+elsize  }}; break;
        case RIGHT: output={{position.x+elsize/2,position.y},       {position.x       ,position.y+elsize  }}; break;
        default: return;//Keine Animation
    }
    animationHandler.add(LEVELEFFECT,2,0,0,3,0,5/*speed*/,p,{-1,-1},output,color);
}

//void GAME::lavafallAnimation(POS position)                                                //Ezrueg eine Animation f�r ein in die Lava gefallenes Objekt
//{
//    position.x=origin.x+(position.x-0.5)*elsize;
//    position.y=origin.y+(size.y-position.y-0.5-1)*elsize;
//
//    animationHandler.add(LEVELEFFECT,2,0,0,9,0,6/*speed*/,&lavafall,{-1,-1},{position,{position.x+elsize*2,position.y+elsize*2}});
//}



void GAME::print()
{   if(!prepared)
    {   error("GAME::print()","Das Spiel darf nicht ausgegeben werden, da die Spieldaten noch nicht aufbereitet wurden");
        return;
    }

    gamebackground.print();

    gamelog->print();               //Logger ausgeben

    printFloor();
    //Transportert
    if(kugelStartPointer!=NULL) kugelStartPointer->print();     //Gibt alle Kugeln aus der Liste aus
    avatar->print();                //Gibt die Spielfigur aus
    if(lockStartPointer!=NULL)  lockStartPointer->print();      //Gibt alle Schl�sser (und Schl�ssel) aus der Liste aus
    //Kanonen
}

void GAME::move(DIRECTION richtung)
{   avatar->move(richtung,1);       //Weiterleiten. 1= User Input
}

void GAME::run()                    //F�hrt einen weiteren Simulationsschritt durch
{
    if(kugelStartPointer!=NULL)     kugelStartPointer->run();
    cleanupKugeln();                //Alle Kugeln, die zum l�schen markiert wurden jetzt l�schen
    avatar->run();

    //Hier andere Elemente simulieren

    gamelog->run();                 //Gamelogger
}

void GAME::cleanupKugeln()                //Alle Kugeln, die zum l�schen markiert wurden jetzt l�schen
{
    KUGEL *p=kugelStartPointer,*last=NULL;
    while(p!=NULL)
    {   if(p->getType()<0)          //Muss gel�scht werden
        {   if(last==NULL)//erstes Objekt
            {   kugelStartPointer=p->getNextObject();
                delete p;
                p=kugelStartPointer;
                continue;
            }else
            {   KUGEL *q=p->getNextObject();
                last->setNextObject(q);
                delete p;
                p=q;
                continue;
            }
        }
        last=p;
        p=p->getNextObject();
    }
}

KUGEL* GAME::KugelOnField(POS position,KUGEL* ignore)   //�berpr�ft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zur�ckgegeben
{   //Gibt zur�ck, ob sich irgendeine Kugel an der geschten Position aufh�lt
    if(kugelStartPointer==NULL) return 0;

    return kugelStartPointer->KugelOnField(position,ignore);        //Weiterleiten an alle Kugeln
}

LOCK* GAME::KeyOnField(POS position)                    //�berpr�ft, ob sich ein Schl�ssel auf diesem Feld befindet. Wenn ja wird der Pointer auf dieses Lock-Objekt zur�ckgegeben
{   //Gibt zur�ck, ob sich irgendein Schl�ssel an der gesuchten Position aufh�lt
    if(lockStartPointer==NULL) return 0;

    return lockStartPointer->KeyOnField(position);      //Weiterleiten an alle Locks
}

bool GAME::AvatarOnField(POS position)                  //�berpr�ft, ob der Avatar dieses Feld blockiert
{   return avatar->AvatarOnField(position);             //Weiterleiten an den Avatar
}

void GAME::stopMovementsTo(POS pos,int limit)           //Objekte, die sich in dieses Feld bewegen wollen abprallen lassen, weil es ein anderes Objekt auf dieses Feld muss
{   //Die Objekte werden nicht gestoppt, wenn es keine Alternative gibt. (Wenn das Objekt in dieses Feld zur�ckprallt, weil ein anderes belegt ist)
    //Dies sollte aber nicht vorkommen

    //limit: Wie weit sich die Objekte noch in das Feld vorbewegen k�nne, bevor sie abprallen

    //Avatar stoppen:
    avatar->stopMovementTo(pos,limit);

    //Alle Kugeln stoppen:
    if(kugelStartPointer!=NULL)     kugelStartPointer->stopMovementsTo(pos,limit);
}

//void GAME::deleteKugel(KUGEL *pointer)                        //L�scht eine Kugel aus der Liste
//{
//     KUGEL *p=kugelStartPointer,*last=NULL;
//     while(p!=NULL)
//     {
//        if(p==pointer)
//        {   if(last==NULL)//erstes Objekt
//            {   kugelStartPointer=p->getNextObject();
//                delete p;
//                return;
//            }else
//            {   last->setNextObject(p->getNextObject());
//                delete p;
//                return;
//            }
//        }
//        last=p;
//        p=p->getNextObject();
//     }
//}

bool GAME::isMoving(OBJEKT object)                            //Gibt zur�ck, ob sich ein solches Objekt gerade am Bildschirm bewegt
{
    if(object==OBJ_AVATAR)
        return avatar->isMoving();
    if(object==OBJ_KUGEL)
        if(kugelStartPointer!=NULL) return kugelStartPointer->isMoving();
        else return 0;
    error("GAME::isMoving()","Das Objekt, dass ueberprueft werden soll ist ungueltig. Es wird 0 (not moving) zurueckgegeben. object=%d",object);
    return 0;
}

void GAME::makeLavaSecure(POS position)                     //Lavafeld mit Block bef�llen (Dezimalwert des Spielfeldes �ndern. Muss beim Neustart des Levels wieder R�ckg�ngig gemacht werden)
{
    if(spielfeld[position.y][position.x]!=34)               //Keine Lava
    {   error("GAME::makeLavaSecure()","Das Spielfeld enthaelt an der uebergebenen Position keine Lava. Position: (%dx%d) Wert: %d",position.x,position.y,spielfeld[position.y][position.x]);
    }else
        spielfeld[position.y][position.x]=54;               //Mit Block versperren
}

void GAME::addGameLogEvent(GameEventType type,DIRECTION richtung=NONE)              //Event hinzuf�gen
{   //�berpr�fung, ob die Klasse prepared ist wird nicht gemacht
    gamelog->addEvent(type,richtung);
}


int GAME::isWalkable(OBJEKT object,POS position)            //Ob ein bestimmtes Feld von einem Objekt betreten werden darf (Es wird nur der Feldtyp gepr�ft, nicht ob sich andere Elemente wie zB. Kugeln darauf befinden)
{
    static int limit;

    limit=walkable[object][spielfeld[position.y][position.x]];
    if(lockStartPointer!=NULL && lockStartPointer->isLocked(position))                //Das Feld ist von einem Schloss blockiert
    {   if(limit>Wlock)
            return Wlock;
    }
    return limit;
}

void GAME::openLock(LOCK* lockAdr)                          //�ffnet (=l�scht) das Schloss
{
    LOCK *p=lockStartPointer,*last=NULL;
    while(p!=NULL)
    {   if(p==lockAdr)                                      //Muss gel�scht/ge�ffnet werden
        {   if(last==NULL)//erstes Objekt
            {   lockStartPointer=p->getNextObject();
                delete p;
                return;
            }else
            {   last->setNextObject(p->getNextObject());
                delete p;
                return;
            }
        }
        last=p;
        p=p->getNextObject();
    }
}

int GAME::isPrepared()                                      //Gibt zur�ck, ob das Spiel erfolgreich vorbereitet und gespielt werden kann
{   return prepared;
}

void GAME::printGameLogBackground()                         //Gibt nur den linken Rand und den Hintergrund des GameLog-Bereiches aus (wird von GAMELOG::print() auch erledigt)
{   gamelog->printBackground();                             //weiterleiten
    return;
}

void GAME::setGameBackgroundColor(COLOR target)             //Setzt die Farbe des Leuchtens im Spielhintergrund
{   gamebackground.setColor(target);                        //Weiterleiten
}

void GAME::printPreview()
{   gamebackground.print();                                 //Hintergrund auch ausgeben
    LEVEL::printPreview();
}

bool GAME::runBuildupAnimation()
{   gamebackground.print(false);                            //Hintergrund auch ausgeben
    return LEVEL::runBuildupAnimation();
}

void GAME::printFloor()                                     //Spielfl�che ausgeben (inkl. Levelhintergrund)
{   gamebackground.print();                                 //Hintergrund auch ausgeben
    LEVEL::printFloor();
}

void GAME::setGameBackgroundSplashColor(COLOR splash)       //kurzfristige Farbe setzen
{   gamebackground.setSplashColor(splash);
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
            {   splashProgress-=1;
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
void GAMEBACKGROUND::setColor(COLOR _final)             //Neue, endg�ltige Farbe w�hlen
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
    if(splashProgress<=0 || !colorcmp(splash,_splash))
    {   splash=_splash;
        splashProgress=1;
    }
    splashDirection=0;
}
