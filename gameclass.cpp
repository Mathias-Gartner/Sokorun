#include <stdlib.h>
#include "animation.h"
#include "avatar.h"
#include "gameclass.h"
#include "globals.h"
#include "graphics.h"
#include "kugel.h"
#include "lock.h"
#include "logger.h"
#include "transporter.h"



void GAME::setupGameData()
{   if(prepared)
    {   error("GAME::setupGameData()","Das Spiel darf nicht aufbereitet werden da zuvor die bereit existierenden Daten zurückgesetzt und freigegeben werden muessen. Dies wurde an dieser Stelle noch nicht ausprogrammiert");
        //Hier: Leeren/Freigeben aller Daten
        return;
    }
    gamelog=new class GAMELOG();           //Gamelog erzeugen


    avatar=new class AVATAR(this,&origin,&elsize,&size,avatarOrigin);

    ///Verkettete Kugelklasse generieren:
        KUGELorigin *k=kugelOriginStart;    //Alle Kugeln aus den Leveldaten durchgehen
        while(k!=NULL)
        {   kugelStartPointer=new class KUGEL(this,&origin,&elsize,&size,k->origin,k->type,/*next-pointer=startpointer*/kugelStartPointer);   //Am Anfang einfügen
            k=k->next;
        }
    ///Verkettete Schlosslasse generieren:
        LOCKorigin *l=lockOriginStart;    //Alle Schlösser aus den Leveldaten durchgehen
        while(l!=NULL)
        {   lockStartPointer=new class LOCK(this,&origin,&elsize,&size,l->lock,l->key,l->color,/*next-pointer=startpointer*/lockStartPointer);   //Am Anfang einfügen
            l=l->next;
        }
    ///Verkettete Transporterklasse generieren:
        TRANSPORTERorigin *t=transporterOriginStart;
        while(t!=NULL)
        {   transporterStartPointer=new class TRANSPORTER(this,&origin,&elsize,&size,t,/*next-pointer=startpointer*/transporterStartPointer);    //Am Anfang einfügen
            t=t->next;
        }
    prepareLevelfieldAnimations(),      //Animierte Felder erzeugen

    prepared=1;
}

void GAME::clearGameData()              //Löscht alle Spieledaten, damit diese später wieder neu geladen werden können
{
    if(prepared)
    {   delete gamelog;
        delete avatar;

        ///Die verketteten Objekte der Klasse KUGEL löschen:
            KUGEL *k=kugelStartPointer,*lk;
            while(k!=NULL)
            {   lk=k->getNextObject();
                delete k;
                k=lk;
            }
            kugelStartPointer=NULL;
        ///Die verketteten Objekte der Klasse LOCK löschen:
            LOCK *l=lockStartPointer,*ll;
            while(l!=NULL)
            {   ll=l->getNextObject();
                delete l;
                l=ll;
            }
            lockStartPointer=NULL;
        ///Die verketteten Objekte der Klasse TRANSPORTER löschen:
            TRANSPORTER *t=transporterStartPointer,*lt;
            while(t!=NULL)
            {   lt=t->getNextObject();
                delete t;
                t=lt;
            }
            transporterStartPointer=NULL;
        ///Animationen entfernen:
            animationHandler.remove(LEVELFIELD);
            animationHandler.remove(LEVELEFFECT);
            animationHandler.remove(OBJECTBUILDUP);
            animationHandler.remove(MOVEMENTINFO);
            levelfieldAnimationsPrepared=0;                                         //Animationen für Levelfelder (zB. Lava) noch nicht erstellt
        ///Blockierte Lava wiederherstellen:
            for(int y=0;y<size.y;y++)
            {   for(int x=0;x<size.x;x++)
                {   if(spielfeld[y][x]==TILE_BLOCKEDLAVA)
                        spielfeld[y][x]=TILE_LAVA;
                }
            }
        ///Displayliste zurücksetzen:
            resetDisplayList();
        ///Hintergrund zurücksetzen:
             gamebackground.reset();                                                //Setzt den Hintergrund zurueck
        ///Variablen neu initialisieren:
            avatarAnimationCompleted=0;
            kugelAnimationCompleted=0;

        prepared=0;
        logger(1,"\"GAME::clearGameData()\":  Die Game-Daten wurden erfolgreich geloescht. prepared=%d",prepared);
    }else
    {   error("GAME::clearGameData()","Die Spieledaten wurden nicht vorbereitet und koennen daher auch nicht zurueckgesetzt werden");
    }
}

void GAME::prepareLevelfieldAnimations()                 //Fügt die Animationen für animierte Levelfelder (zB. Lava) in den Animationshandler ein
{   if(levelfieldAnimationsPrepared)
    {   error("LEVEL::prepareLevelfieldAnimations()","Die Animationen fuer Levelfelder wurden bereits erstellt");
        return;
    }
    for(int y=0;y<size.y;y++)
    {   for(int x=0;x<size.x;x++)
        {   if(spielfeld[y][x]==TILE_LAVA)
            {   POS pos;
                pos.x=origin.x+x*elsize;
                pos.y=origin.y+(size.y-y-1)*elsize;
                animationHandler.add(LEVELFIELD,2,0,0,6,1,LAVAFLOW_SPEED/*speed*/,&lavaflow,{0,0},{pos,{pos.x+elsize,pos.y+elsize}});
            }
        }
    }
}

void GAME::initBuildupAnimationSpecialElements()    //Animation-Handler aktualisieren um Overlay-Elemente animiert aufzubauen
{
    POS pos;
    ///TRANSPORTER
        char num;
        bool mirrorX,mirrorY;
        AREA output;
        TRANSPORTERorigin *t=transporterOriginStart;
        while(t!=NULL)
        {   //Transporter
            pos.x=origin.x+((t->origin)->position).x* elsize;
            pos.y=origin.y+(size.y-((t->origin)->position).y-1)* elsize;
            if(t->type==0)  num=TILE_TRANSPORTER;
            else            num=TILE_DEATHTRANSPORTER;
            animationHandler.add(OBJECTBUILDUP,3,0,0,100,3,BUILDUP_SPCLELEM_SPEED/*speed*/,&leveltiles,{num%8,num/8},{pos,{pos.x+elsize,pos.y+elsize}},((t->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR));

            //Schienen
            bool startAllowed=1;
            RAIL *r=t->start;
            while(r!=NULL && (r!=t->start||startAllowed))
            {   startAllowed=0;
                pos.x=origin.x+r->position.x* elsize;
                pos.y=origin.y+(size.y-r->position.y-1)* elsize;

                getRailOutputInformation(r->outputType,&num,&mirrorX,&mirrorY);
                     if(!mirrorX && !mirrorY)/*Normal*/  output={{pos.x,pos.y},{pos.x+elsize,pos.y+elsize}};
                else if(!mirrorX &&  mirrorY)/* >Y < */  output={{pos.x,pos.y+elsize},{pos.x+elsize,pos.y}};
                else if( mirrorX && !mirrorY)/* >X < */  output={{pos.x+elsize,pos.y},{pos.x,pos.y+elsize}};
                else                         /* >XY< */  output={{pos.x+elsize,pos.y+elsize},{pos.x,pos.y}};

                animationHandler.add(OBJECTBUILDUP,3,0,0,100,3,BUILDUP_SPCLELEM_SPEED/*speed*/,&leveltiles,{num%8,num/8},output,((t->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR));
                r=r->next;
            }
            t=t->next;
        }
    ///KUGELN
        KUGELorigin *p=kugelOriginStart;
        while(p!=NULL)
        {   pos.x=origin.x+p->origin.x* elsize;
            pos.y=origin.y+(size.y-p->origin.y-1)* elsize;
            if(p->type==1)  animationHandler.add(OBJECTBUILDUP,2,1, 28,31, 3,BUILDUP_SPCLELEM_SPEED/*speed*/,&levelanimations,POS{-1,-1},{pos,{pos.x+elsize,pos.y+elsize}});//Braune Kugel (Typ 1)
            else            animationHandler.add(OBJECTBUILDUP,2,1, 32,35, 3,BUILDUP_SPCLELEM_SPEED/*speed*/,&levelanimations,POS{-1,-1},{pos,{pos.x+elsize,pos.y+elsize}});
            p=p->next;
        }
    ///AVATAR
        pos.x=origin.x+avatarOrigin.x* elsize;
        pos.y=origin.y+(size.y-avatarOrigin.y-1)* elsize;
        animationHandler.add(OBJECTBUILDUP,2,1, 36,39, 3,BUILDUP_SPCLELEM_SPEED/*speed*/,&levelanimations,POS{-1,-1},{pos,{pos.x+elsize,pos.y+elsize}});
    ///SCHLÖSSER
        LOCKorigin *l=lockOriginStart;
        while(l!=NULL)
        {   pos.x=origin.x+(l->lock).x* elsize;
            pos.y=origin.y+(size.y-(l->lock).y-1)* elsize;
            animationHandler.add(OBJECTBUILDUP,3,0,0,100,3,BUILDUP_SPCLELEM_SPEED/*speed*/,&leveltiles,{TILE_LOCK%8,TILE_LOCK/8},{pos,{pos.x+elsize,pos.y+elsize}},l->color);

            pos.x=origin.x+(l->key).x* elsize;
            pos.y=origin.y+(size.y-(l->key).y-1)* elsize;
            animationHandler.add(OBJECTBUILDUP,3,0,0,100,3,BUILDUP_SPCLELEM_SPEED/*speed*/,&leveltiles,{TILE_KEY%8,TILE_KEY/8},{pos,{pos.x+elsize,pos.y+elsize}},l->color);
            l=l->next;
        }

    return;
}


GAME::~GAME()
{   //Der LEVEL-Destruktor wird ebenfalls aufgerufen
    if(prepared)
    {   clearGameData();                            //Befindet sich deshalb in diesem if, damit keine Fehlermeldung ausgegeben wird, wenn die Spieldaten nicht vorbereitet sind
    }
//    {   delete gamelog;
//
//        delete avatar;
//
//        ///Die verketteten Objekte der Klasse KUGEL löschen:
//            KUGEL *k=kugelStartPointer,*lk;
//            while(k!=NULL)
//            {   lk=k->getNextObject();
//                delete k;
//                k=lk;
//            }
//            kugelStartPointer=NULL;
//        ///Die verketteten Objekte der Klasse LOCK löschen:
//            LOCK *l=lockStartPointer,*ll;
//            while(l!=NULL)
//            {   ll=l->getNextObject();
//                delete l;
//                l=ll;
//            }
//            lockStartPointer=NULL;
//        ///Die verketteten Objekte der Klasse TRANSPORTER löschen:
//            TRANSPORTER *t=transporterStartPointer,*lt;
//            while(t!=NULL)
//            {   lt=t->getNextObject();
//                delete t;
//                t=lt;
//            }
//            transporterStartPointer=NULL;
//    }
}

void GAME::EnablePauseButton(bool enable)                       //Ativiert/Deaktiviert den Pause-Button im Gamelog
{   gamelog->EnablePauseButton(enable);                         //An den Gamelog weiterleiten
}

POS GAME::getTargetFieldCoord(POS position,DIRECTION richtung)  //Gibt die Koorinaten des Nachbarfeldes, abhängig von der Richtung, zurück
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

bool GAME::getLevelfieldAnimationsPrepared()               //Abfragen, ob die Animierten Levelfelder bereits erstellt wurden
{   return levelfieldAnimationsPrepared;
}

void GAME::printMovingObject(MOVEMENT *movement,POS position,int spriteNum,POS beamTarget)      //Gibt ein Objekt am Spielfeld aus, dass sich darauf bewegen kann
{
    bool noOutput=0;        //Ob die Ausgabe zum Schluss stattfinden soll
    POS pos;
    pos.x=origin.x+position.x* elsize;
    pos.y=origin.y+(size.y-position.y-1)* elsize;
    //Je nach Bewegung das Objekt von seiner Urpsrünglichen Position wegbewegen:
    if(movement->moving!=0)//in Bewegung
    {   switch(movement->richtung)
        {   case UP:    pos.y+= elsize*movement->progress/100; break;
            case DOWN:  pos.y-= elsize*movement->progress/100; break;
            case LEFT:  pos.x-= elsize*movement->progress/100; break;
            case RIGHT: pos.x+= elsize*movement->progress/100; break;



            case BEAM:  {   //Einschwärzen an der Startposition:    leveltiles.print({pos,{pos.x+ elsize,pos.y+ elsize}},{spriteNum%8,spriteNum/8},COLOR{1-(movement->progress/100.0f),1-(movement->progress/100.0f),1-(movement->progress/100.0f)});
                            /*ausblenden:*/ leveltiles.print({pos,{pos.x+ elsize,pos.y+ elsize}},{spriteNum%8,spriteNum/8},WHITE,(100-movement->progress)/100.0f);

                            if(beamTarget.x<0)  pos=getTargetBeamer();  //Zielfeld herausfinden
                            else                pos=beamTarget;         //Zielfeld wurde übergeben

                            pos.x=origin.x+pos.x*elsize;
                            pos.y=origin.y+(size.y-pos.y-1)*elsize;
                            //Aufhellen an der Zielposition:  leveltiles.print({pos,{pos.x+ elsize,pos.y+ elsize}},{spriteNum%8,spriteNum/8},COLOR{(movement->progress/100.0f),(movement->progress/100.0f),(movement->progress/100.0f)});
                            /*einblenden*/leveltiles.print({pos,{pos.x+ elsize,pos.y+ elsize}},{spriteNum%8,spriteNum/8},WHITE,movement->progress/100.0f);
                            noOutput=1;//Ausgabe bereits geschehen --> nicht mehr ausgeben
                        }break;


            default:    error("AVATAR::print()","Der Avatar ist zwar in Bewegung, die Richtung ist aber ungueltig. Die Bewegung wird abgebrochen. richtung=%d",movement->richtung);
                        movement->moving=0;
        }
    }
    if(!noOutput)
        leveltiles.print({pos,{pos.x+elsize,pos.y+elsize}},{spriteNum%8,spriteNum/8}, WHITE, 1.0);

    if(ImgDebug)
    {
        if(movement->moving==0 || movement->progress<=100-OccupiedLimit)
        {   marker(position,movement->blocking?RED:YELLOW);
        }
        if(movement->moving!=0 && movement->progress>=OccupiedLimit)
        {   if(movement->richtung==BEAM && beamTarget.x>=0)//Zielfeld wurde mitübergeben (zB. bei transportern)
            {   marker(beamTarget,movement->blocking?RED:YELLOW);
            }else
            marker(getTargetFieldCoord(position,movement->richtung),movement->blocking?RED:YELLOW);
        }
    }


}


int GAME::addFieldEffect(POS position,FIELDEFFECT effect,DIRECTION richtung,COLOR color,double progress)//Erzeugt eine Animation
{
    ///Effekte, die durch eigene Unterprogramme realisiert wurden:
    switch(effect)
    {   case COLLISION:     return collision(position,richtung,color);
        default: break;
    }


    AREA fieldCoord;
    if(effect==LAVAFALL)//Doppelt so große Animation (über 4 Felder)
    {   fieldCoord.a={(int)(origin.x+(position.x-0.5f)*elsize),(int)(origin.y+(size.y-position.y-0.5f-1)*elsize)};
        fieldCoord.b ={fieldCoord.a.x  +elsize*2   ,fieldCoord.a.y   +elsize*2   };
    }else
    {   fieldCoord.a={origin.x+(position.x)*elsize,origin.y+(size.y-position.y-1)*elsize};
        switch(richtung)//Wenn das Objekt in Bewegung war, und die Animation daher verschoeben werden muss
        {   case UP:    fieldCoord.a.y+= elsize*progress/100; break;
            case DOWN:  fieldCoord.a.y-= elsize*progress/100; break;
            case LEFT:  fieldCoord.a.x-= elsize*progress/100; break;
            case RIGHT: fieldCoord.a.x+= elsize*progress/100; break;
            default: break;
        }
        fieldCoord.b={fieldCoord.a.x        +elsize     ,fieldCoord.a.y         +elsize };
    }


    switch(effect)
    {   case LAVAFALL:      return animationHandler.add(LEVELEFFECT,2,0,0,9,0,LAVAFALL_ANI_SPEED/*speed*/,&lavafall,{-1,-1},fieldCoord);  break;
        case AVATARLAVA:    return animationHandler.add(LEVELEFFECT,2,0,24,27,0,LAVA_KILLANI_SPEED/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        case KUGELLAVA:     return animationHandler.add(LEVELEFFECT,2,0,20,23,0,LAVA_KILLANI_SPEED/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        case AVATARKILL:    return animationHandler.add(LEVELEFFECT,2,0,36,39,0,KILLANI_SPEED/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        case KUGELKILL:     return animationHandler.add(LEVELEFFECT,2,0,32,35,0,KILLANI_SPEED/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        case KUGELBLOCKKILL:return animationHandler.add(LEVELEFFECT,2,0,28,31,0,KILLANI_SPEED/*speed*/,&levelanimations,{-1,-1},fieldCoord);  break;
        default:    error("GAME::addFieldEffect()","Unbekannter Anitamionseffekt uebergeben. Animation wird nicht durchgefuehrt. effect=%d",effect);
    }
    return -1;
}

int GAME::collision(POS position,DIRECTION richtung,COLOR color)                   //Erzeugt eine Aufprall-Animation
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
        default: return -1;//Keine Animation
    }
    return animationHandler.add(LEVELEFFECT,2,0,0,3,0,COLISSIONANI_SPEED/*speed*/,p,{-1,-1},output,color);
}


void GAME::print()
{   if(!prepared)
    {   error("GAME::print()","Das Spiel darf nicht ausgegeben werden, da die Spieldaten noch nicht aufbereitet wurden");
        return;
    }

    gamebackground.print(gamelog->getxpos());

    gamelog->print();               //Logger ausgeben

    printFloor();
    animationHandler.print(LEVELFIELD);         //Animationen ausgeben
    animationHandler.print(LEVELEFFECT);        //Animationen ausgeben
    animationHandler.print(MOVEMENTINFO);       //Animationen ausgeben
    animationHandler.print(LEVELEFFECT);        //Animationen ausgeben


    if(transporterStartPointer!=NULL)  transporterStartPointer->print();      //Gibt alle Transporter (und deren Schienennetz) aus
    if(kugelStartPointer!=NULL) kugelStartPointer->print();     //Gibt alle Kugeln aus der Liste aus
    avatar->print();                //Gibt die Spielfigur aus
    if(lockStartPointer!=NULL)  lockStartPointer->print();      //Gibt alle Schlösser (und Schlüssel) aus der Liste aus
    //Kanonen
}

void GAME::move(DIRECTION richtung)
{   avatar->move(richtung,1);       //Weiterleiten. 1= User Input
}

int GAME::run()                         //Führt einen weiteren Simulationsschritt durch; Rückgabewert: Spielstatus (-1: Gameover; >=0: Anzahl der Noch freien Zielfelder)
{   int targeted=0;

    if(kugelStartPointer!=NULL)
    {   targeted=kugelStartPointer->run();
        cleanupKugeln();                //Alle Kugeln, die zum löschen markiert wurden jetzt löschen
    }else
    {   targeted=-1;                    //Game Over (keine Kugeln mehr)
    }
    avatar->run();
    if(transporterStartPointer!=NULL)   transporterStartPointer->run(); //Alle Transporter simulieren

    //Hier andere Elemente simulieren

    gamelog->run();                     //Gamelogger

    if(targeted<0 || avatar->isDead())    return -1;  //Gameover
    return targetFieldAnz - targeted;
}

void GAME::cleanupKugeln()                //Alle Kugeln, die zum löschen markiert wurden jetzt löschen
{
    KUGEL *p=kugelStartPointer,*last=NULL;
    while(p!=NULL)
    {   if(p->getType()<0)          //Muss gelöscht werden
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

KUGEL* GAME::KugelOnField(POS position,KUGEL* ignore)   //Überprüft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zurückgegeben
{   //Gibt zurück, ob sich irgendeine Kugel an der geschten Position aufhält
    if(kugelStartPointer==NULL) return 0;

    return kugelStartPointer->KugelOnField(position,ignore);        //Weiterleiten an alle Kugeln
}

LOCK* GAME::KeyOnField(POS position)                    //Überprüft, ob sich ein Schlüssel auf diesem Feld befindet. Wenn ja wird der Pointer auf dieses Lock-Objekt zurückgegeben
{   //Gibt zurück, ob sich irgendein Schlüssel an der gesuchten Position aufhält
    if(lockStartPointer==NULL) return 0;

    return lockStartPointer->KeyOnField(position);      //Weiterleiten an alle Locks
}

bool GAME::AvatarOnField(POS position)                  //Überprüft, ob der Avatar dieses Feld blockiert
{   return avatar->AvatarOnField(position);             //Weiterleiten an den Avatar
}

void GAME::stopMovementsTo(POS pos,int limit)           //Objekte, die sich in dieses Feld bewegen wollen abprallen lassen, weil es ein anderes Objekt auf dieses Feld muss
{   //Die Objekte werden nicht gestoppt, wenn es keine Alternative gibt. (Wenn das Objekt in dieses Feld zurückprallt, weil ein anderes belegt ist)
    //Dies sollte aber nicht vorkommen

    //limit: Wie weit sich die Objekte noch in das Feld vorbewegen könne, bevor sie abprallen

    //Avatar stoppen:
    avatar->stopMovementTo(pos,limit);

    //Alle Kugeln stoppen:
    if(kugelStartPointer!=NULL)     kugelStartPointer->stopMovementsTo(pos,limit);
}

//void GAME::deleteKugel(KUGEL *pointer)                        //Löscht eine Kugel aus der Liste
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

bool GAME::isMoving(OBJEKT object)                            //Gibt zurück, ob sich ein solches Objekt gerade am Bildschirm bewegt
{
    if(object==OBJ_AVATAR)
        return avatar->isMoving();
    if(object==OBJ_KUGEL)
    {   if(kugelStartPointer!=NULL) return kugelStartPointer->isMoving();
        else return 0;
    }
    error("GAME::isMoving()","Das Objekt, dass ueberprueft werden soll ist ungueltig. Es wird 0 (not moving) zurueckgegeben. object=%d",object);
    return 0;
}

void GAME::makeLavaSecure(POS position)                     //Lavafeld mit Block befüllen (Dezimalwert des Spielfeldes ändern. Muss beim Neustart des Levels wieder Rückgängig gemacht werden)
{
    if(spielfeld[position.y][position.x]!=TILE_LAVA)        //Keine Lava
    {   error("GAME::makeLavaSecure()","Das Spielfeld enthaelt an der uebergebenen Position keine Lava. Position: (%dx%d) Wert: %d",position.x,position.y,spielfeld[position.y][position.x]);
    }else
    {   spielfeld[position.y][position.x]=TILE_BLOCKEDLAVA; //Mit Block versperren
        resetDisplayList();
        POS pos;
        pos.x=origin.x+position.x* elsize;
        pos.y=origin.y+(size.y-position.y-1)* elsize;
        animationHandler.remove( animationHandler.findAnimation(LEVELFIELD ,{pos,{pos.x+elsize,pos.y+elsize}}) ); //Die Lavaanimation an dieser Position finden und löschen
        pos.x=origin.x+(position.x-0.5f)* elsize;
        pos.y=origin.y+(size.y-position.y-1.5f)* elsize;
        animationHandler.remove( animationHandler.findAnimation(LEVELEFFECT,{pos,{pos.x+2*elsize,pos.y+2*elsize}}) ); //Die Lavafallanimation, wenn sie existiert, an dieser Position finden und löschen
    }
}

void GAME::addGameLogEvent(GameEventType type,DIRECTION richtung=NONE)              //Event hinzufügen
{   //Überprüfung, ob die Klasse prepared ist wird nicht gemacht
    gamelog->addEvent(type,richtung);
}

bool GAME::isLocked(POS position)                           //Überprüft, ob ein bestimmtes Feld von einem Schloss versperrrt wird
{   if(lockStartPointer!=NULL && lockStartPointer->isLocked(position))                //Das Feld ist von einem Schloss blockiert
    {   return 1;                                           //Versperrt
    }
    return 0;                                               //Nicht versperrt
}

int GAME::isWalkable(OBJEKT object,POS position)            //Ob ein bestimmtes Feld von einem Objekt betreten werden darf (Es wird nur der Feldtyp geprüft, nicht ob sich andere Elemente wie zB. Kugeln darauf befinden)
{
    static int limit;

    limit=walkable[object][(int)spielfeld[position.y][position.x]];
    if(isLocked(position) && limit>Wlock)                   //Das Feld wird von einem Schloss blockiert
    {   return Wlock;
    }
    return limit;
}

void GAME::openLock(LOCK* lockAdr)                          //Öffnet (=löscht) das Schloss
{
    LOCK *p=lockStartPointer,*last=NULL;
    while(p!=NULL)
    {   if(p==lockAdr)                                      //Muss gelöscht/geöffnet werden
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

int GAME::isPrepared()                                      //Gibt zurück, ob das Spiel erfolgreich vorbereitet und gespielt werden kann
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
{   gamebackground.print(gamelog->getxpos());               //Hintergrund auch ausgeben
    LEVEL::printPreview();
}

bool GAME::runBuildupAnimation()
{   gamebackground.print(gamelog->getxpos(),false);         //Hintergrund auch ausgeben
    return LEVEL::runBuildupAnimation();
}

void GAME::printFloor(bool printAnimatedFields)             //Spielfläche ausgeben (inkl. Levelhintergrund)
{   gamebackground.print(gamelog->getxpos());               //Hintergrund auch ausgeben
    LEVEL::printFloor(printAnimatedFields);


    //if(rand()%40==0)
    {   static int x=0,y=0;
        int timeout=0;
        do
        {   x+=rand()%size.x;
            x%=size.x;
            y+=rand()%size.y;
            y%=size.y;
            timeout++;
        }while(timeout<8 && spielfeld[y][x]!=TILE_LAVA);

        if(spielfeld[y][x]==TILE_LAVA && rand()%15==0)
        {   POS pos;
            pos.x=origin.x+x*elsize;
            pos.y=origin.y+(size.y-y-1)*elsize;

            animationHandler.add(LEVELEFFECT,2,0,0,15,0,LAVABLUBB_SPEED/*speed*/,&lavablubb,{0,0},{pos,{pos.x+elsize,pos.y+elsize}});
        }
    }
}

void GAME::setGameBackgroundSplashColor(COLOR splash)       //kurzfristige Farbe setzen
{   gamebackground.setSplashColor(splash);
}


bool GAME::isDriveable(RAIL *r,TRANSPORTER *ignore,bool loaded)                 //Gibt zurück ob sich ein Transporter auf dieses Feld bewegen darf (0: nein; 1:ja)
{   //loaded: Wenn 1: der Transporter ist beladen und darf auch dann nicht ins nächste Feld, wenn dort eine Kugel / der Avatar ist
    if(r==NULL) return 0;                                   //Ungültiges Schienenelement. Nicht befahrbar

    if(isLocked(r->position))  return 0;                    //Das Feld wird von einem Schloss versperrt
    if(loaded)                                              //Transporter = nicht tödlich & beladen
    {
        //CHECK FOR KUGEL & AVATAR
    }
    if(transporterStartPointer->TransporterOnField(r->position,ignore)!=NULL)   //Wenn sich ein Transporter an dieser Stelle befindet: nicht betretbar
        return 0;
    return 1;                                               //Befahrbar
}


void GAME::killObjectsOnField(POS pos)                      //Zerstört alle Kugeln und den Avatar, wenn diese dieses Feld blockieren (wird zB. duch den tödlichen Transporter ausgelöst)
{
    //Avatar töten:
    avatar->killOnField(pos);

    //Kugeln zerstören:
    if(kugelStartPointer!=NULL)     kugelStartPointer->killOnField(pos);
}




bool GAME::isPauseButtonClicked()                       //Gibt zurück, ob der Pause-Button seit dem letzten Aufruf gedrückt worden ist (im Gamelog)
{   return gamelog->isPauseButtonClicked();             //Weiterleiten
}









/// ==================================================================================================================================
/// KLASSE GAMEBACKGROUND ************************************************************************************************************
/// ==================================================================================================================================

void GAMEBACKGROUND::reset()                           //Setzt den Hintergrund zurueck
{
    alpha=0;

    final=WHITE;
    old=final;

    splashProgress=0;
}


GAMEBACKGROUND::GAMEBACKGROUND(AREA _area)
{
    area=_area;
    reset();
}

void GAMEBACKGROUND::print(int bx,bool printshine)
{   gamebackground.print({area.a,{bx,area.b.y}},{{0.0,0.0},{(bx-area.a.x)/128.0f,(area.b.y-area.a.y)/128.0f}},WHITE); //Hintergrundbild ausgeben
    if(printshine)
    {   colorProgress+=GAME_BG_SHINE_COLOR_SPEED;
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
        {   target={final.r+(splash.r-final.r)*splashProgress/100.0f,final.g+(splash.g-final.g)*splashProgress/100.0f,final.b+(splash.b-final.b)*splashProgress/100.0f};
        }


        COLOR current;
        if(colorProgress<100 || splashProgress>0)//Noch nicht die neue Farbe
            current={old.r+(float)((target.r-old.r)*colorProgress/100.0f),old.g+(float)((target.g-old.g)*colorProgress/100.0f),old.b+(float)((target.b-old.b)*colorProgress/100.0f)};
        else current=target;

        alpha+=GAME_BG_SHITE_ALPHA_SPEED;
        if(alpha>1.0)   alpha=1.0;

        shine.print(area,stdTextArea,current,alpha);
    }
}
void GAMEBACKGROUND::setColor(COLOR _final)             //Neue, endgültige Farbe wählen
{   //Zielfarbe bestimmen
    COLOR target=final;
    if(splashProgress>0)//Farben mischen
    {   target={final.r+(splash.r-final.r)*splashProgress/100.0f,final.g+(splash.g-final.g)*splashProgress/100.0f,final.b+(splash.b-final.b)*splashProgress/100.0f};
    }

    old={old.r+(float)((target.r-old.r)*colorProgress/100.0f),old.g+(float)((target.g-old.g)*colorProgress/100.0f),old.b+(float)((target.b-old.b)*colorProgress/100.0f)};

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
