//Daten für die Transporter, die sich durchs Spielfeld bewegen
//Version 1.0
//Jakob Maier

#include <stdlib.h>
#include <windows.h>
#include "gameclass.h"
#include "globals.h"
#include "logger.h"
#include "transporter.h"

TRANSPORTER::TRANSPORTER(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,TRANSPORTERorigin *_data,TRANSPORTER *_next)       //Initialisiert den Transporter
{
    next=_next;

    game=gamePointer;
    origin=originPointer;                       //Position, wo das LEvel ausgegeben wird (links unten)
    elsize=elsizePointer;                       //Größe eines Elementes/Feldes
    levelSize=levelSizePointer;                 //Levelgröße

    data=_data;
    position=data->origin;
    movement.moving=0;                          //Bewegt sich nicht
    movement.progress=0;
    movement.blocking=0;                        //Für die imgDebug-Ausgabe: damit der Rahmen nicht rot gezeichnet wird
    //lastRichtung, blocking und limit wird nicht verwendet. moving kann nicht -1 sein (kein Abprallen möglich)

    direction=data->startDirection;             //Richtung, in die sich der Transporter los bewegt
    minVisibility=1.0f;
}

TRANSPORTER* TRANSPORTER::getNextObject()                                   //Liefert die Adresse des nächsten Objekts zurück
{  return next;
}

void getRailOutputInformation(int outputType,char *tile,bool *mirrorX,bool *mirrorY)    //Gibt Ausgabeinformationen für ein Schienenelement zurück
{   switch(outputType)
    {   case  0:    *tile=TILE_RAIL_VEND;       *mirrorX=0; *mirrorY=1; break;
        case  1:    *tile=TILE_RAIL_VEND;       *mirrorX=0; *mirrorY=0; break;
        case  2:    *tile=TILE_RAIL_HEND;       *mirrorX=1; *mirrorY=0; break;
        case  3:    *tile=TILE_RAIL_HEND;       *mirrorX=0; *mirrorY=0; break;
        case  4:    *tile=TILE_RAIL_VERT;       *mirrorX=0; *mirrorY=0; break;
        case  5:    *tile=TILE_RAIL_HORI;       *mirrorX=0; *mirrorY=0; break;
        case  6:    *tile=TILE_RAIL_EDGE;       *mirrorX=0; *mirrorY=0; break;
        case  7:    *tile=TILE_RAIL_EDGE;       *mirrorX=1; *mirrorY=0; break;
        case  8:    *tile=TILE_RAIL_EDGE;       *mirrorX=1; *mirrorY=1; break;
        case  9:    *tile=TILE_RAIL_EDGE;       *mirrorX=0; *mirrorY=1; break;
        default:    error("TRANSPORTER::getRailOutputInformation()","Unbekannter Schienentyp entdeckt. Es wird 0 (Beginn nach oben) verwendet. outputType: %d",outputType);
                    *tile=TILE_RAIL_VEND;       *mirrorX=0; *mirrorY=1; break;
    }
    return;
}

void TRANSPORTER::printRailTrack(RAIL *r,DIRECTION in,float railtracksVisibility,float *alo,float *aro,float *alu,float *aru,float fkt)    //Gibt ein einzelnes Schienenstück aus
{   //fkt: Wenn der Transporter zu 90% im nächsten Feld ist, dann wird bei genau diesem Schienenstück 0.1 übergeben
    char num;
    bool mirrorX, mirrorY;
    POS pos;

    switch(in)
    {   case RIGHT: *alo=*aro; *alu=*aru; *aro=*alo-railtracksVisibility*fkt; *aru=*alu-railtracksVisibility*fkt;   break;
        case LEFT:  *aro=*alo; *aru=*alu; *alo=*aro-railtracksVisibility*fkt; *alu=*aru-railtracksVisibility*fkt;   break;
        case DOWN:  *alu=*alo; *aru=*aro; *alo=*alu-railtracksVisibility*fkt; *aro=*aru-railtracksVisibility*fkt;   break;
        case UP:    *alo=*alu; *aro=*aru; *alu=*alo-railtracksVisibility*fkt; *aru=*aro-railtracksVisibility*fkt;   break;
        default:    *alo=*alo-railtracksVisibility;
                    *aro=*aro-railtracksVisibility;
                    *alu=*alu-railtracksVisibility;
                    *aru=*aru-railtracksVisibility;break;
    }
    if(*alo<0)   *alo=0;
    if(*aro<0)   *aro=0;
    if(*alu<0)   *alu=0;
    if(*aru<0)   *aru=0;

    //Fürs langsame ein- und ausblenden:
        if(*alo<minVisibility)   *alo=minVisibility;
        if(*aro<minVisibility)   *aro=minVisibility;
        if(*alu<minVisibility)   *alu=minVisibility;
        if(*aru<minVisibility)   *aru=minVisibility;

    getRailOutputInformation(r->outputType,&num,&mirrorX,&mirrorY);//Informationen berechnen

    pos.x=origin->x+r->position.x*(*elsize);
    pos.y=origin->y+(levelSize->y-r->position.y-1)*(*elsize);

             if(!mirrorX && !mirrorY)/*Normal*/  leveltiles.print({{pos.x,pos.y},{pos.x+(*elsize),pos.y+(*elsize)}},{num%8,num/8},((data->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR),*alo,*aro,*alu,*aru);
        else if(!mirrorX &&  mirrorY)/* >Y < */  leveltiles.print({{pos.x,pos.y+(*elsize)},{pos.x+(*elsize),pos.y}},{num%8,num/8},((data->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR),*alu,*aru,*alo,*aro);
        else if( mirrorX && !mirrorY)/* >X < */  leveltiles.print({{pos.x+(*elsize),pos.y},{pos.x,pos.y+(*elsize)}},{num%8,num/8},((data->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR),*aro,*alo,*aru,*alu);
        else                         /* >XY< */  leveltiles.print({{pos.x+(*elsize),pos.y+(*elsize)},{pos.x,pos.y}},{num%8,num/8},((data->type==0)?TRANSPORTER_COLOR:DEATHTRANSPORTER_COLOR),*aru,*alu,*aro,*alo);

}

void TRANSPORTER::print()
{
    float alo,alu,aro,aru;
    POS lastPos;
    RAIL *r;
    float fkt;
    bool smooth;

    ///DIE KOMMENDEN SCHIENEN ANZEIGEN:
        alo=1;alu=1;aro=1;aru=1;
        if(direction==0)    r=position->next;
        else                r=position->prev;
        lastPos=position->position;
        smooth=1;

        if(r!=NULL)
        {   do
            {   if(movement.moving!=0 && smooth)
                {   fkt=(100-movement.progress)/100.0f;
                    smooth=0;
                }/*else if(smooth)//Bei der ersten stelle nichts abziehen
                {   fkt=0;
                    smooth=0;
                }*/else   fkt=1;
                printRailTrack(r,game->getRailDirection(lastPos,r->position),RAILTRACKS_VISIBILITY_NXT,&alo,&aro,&alu,&aru,fkt);
                lastPos=r->position;

                if(direction==0)    r=r->next;
                else                r=r->prev;
            }while(r!=NULL && r!=position && (alo>0||aro>0||alu>0||aru>0));
        }

    ///DIE VERGANGENEN SCHIENEN ANZEIGEN:
        alo=1;alu=1;aro=1;aru=1;
        r=position;
        if(direction==0 && position->next!=NULL)    lastPos=(position->next)->position;
        else if(position->prev!=NULL)               lastPos=(position->prev)->position;
        else lastPos=position->position;

        smooth=1;
        if(r!=NULL)
        {   do
            {   if(movement.moving!=0 && smooth)
                {   fkt=movement.progress/100.0f;
                    smooth=0;
                }else if(smooth)//Bei der ersten stelle nichts abziehen
                {   fkt=0;
                    smooth=0;
                }else   fkt=1;
                printRailTrack(r,game->getRailDirection(lastPos,r->position),RAILTRACKS_VISIBILITY_PRV,&alo,&aro,&alu,&aru,fkt);
                lastPos=r->position;
                if(direction==0)    r=r->prev;
                else                r=r->next;
            }while(r!=NULL && r!=position && (alo>0||aro>0||alu>0||aru>0));
        }
    POS nxt;
    if(direction==0 && position->next!=NULL)//Nach vorne
    {   nxt=position->next->position;
    }else if(position->prev!=NULL)
    {   nxt=position->prev->position;
    }else
    {   error("TRANSPORTER::print()","Ein Schienenelement hat keine Nachbarn, die Schienenstrecke besteht nur aus einem Feld. -die Leveldaten sind ungueltig, das Spiel wird abgebrochen.");
        MessageBox(NULL,"Es kam zu einem Fehler. Die Leveldaten sind ungültig. Für mehr Details sehen Sie im Erroglog nach.","Schwerwiegender Fehler",MB_OK|MB_ICONERROR);
        exit(-1);   for(;;);
    }
    if(next!=NULL)  next->print();  //Alle Transporter ausgeben (immer zuerst die Schienen, dann den Transporter selbst)
    ///Jetzt, da alle Schienen ausgegeben wurde, den Transporter ausgeben:
    game->printMovingObject(&movement,position->position,data->type==0?TILE_TRANSPORTER:TILE_DEATHTRANSPORTER,nxt);
}

TRANSPORTER* TRANSPORTER::TransporterOnField(POS pos,TRANSPORTER *ignore)                       //Überprüft, ob sich auf dieser Position ein Transporter befindet
{   if(this != ignore)
    {   //if(movement.moving==0 || (movement.moving!=0 && movement.progress<100-OccupiedLimit))  //Transporter ist (noch) im positions-Feld
        {   if(poscmp(pos,position->position))                                                  //=gesuchte Position
                return this;                                                                    //Transporteradresse zurückgeben

        }

        if(movement.moving!=0 && movement.progress>=0)                              //Transporter befindet sich bereits auf nächstem Feld
        {
            if(poscmp(pos,game->getTargetFieldCoord(position->position,movement.richtung)))     //nächstes Feld=gesuchte Position
                return this;                                                                    //Transporteradresse zurückgeben
        }
    }

    if(next!=NULL)  return next->TransporterOnField(pos,ignore);                                //nächsten Transporter überprüfen
    else            return NULL;
}

void TRANSPORTER::run()                                                     //Führt einen Simulationsschritt durch
{   if(minVisibility>0) minVisibility-=0.025;                               //Schienenelemente langsam ausblenden


    if(movement.moving==1)
    {   movement.progress+=(AVATAR_SPEED*data->speed);                                      //weiter bewegen

//        if(movement.progress>=OccupiedLimit && movement.progress<OccupiedLimit+data->speed) //Bei genau diesem Schritt könnte ein anderer Tranporter (od. ein spzialelement) diesen Transporter blockieren
//        {
//            if((direction==0 && !game->isDriveable(position->next,this,0))/* || (direction==1 && !game->isDriveable(position->prev,0))*/) //Blockieren
//            {   if(data->reverse)//Umkehren erlaubt
//                {   movement.moving=0;
//
//                }else
//                {   movement.progress=OccupiedLimit-1;
//                }
//
//            }
//        }


        ///Prüfen, ob die Bewegung schon abgeschlossen wurde:
        if(movement.progress>=100)                                          //Im Zielfeld angekommen
        {   movement.lastRichtung=movement.richtung;
            //Neues Feld belegen:
            if(direction==0)    position=position->next;
            else                position=position->prev;
            movement.moving=0;                                              //Anhalten. Im nächsten "if(moving==0)" wird auf ein Schloss und einen fremden Transporter überprüft und die bewegung wenn möglich fortgesetzt.
        }
    }
    if(movement.moving==0)
    {   if(!game->isLocked(position->position))                             //Die aktuelle Position des Transporters wird nicht von einem Schloss blockiert
        {
            char timeout=0;
            RAIL *next=NULL;
            do
            {   if(direction==0 && game->isDriveable(position->next,this,0)) //Nach vorne
                {   next=position->next;
                }else
                if(direction==1 && game->isDriveable(position->prev,this,0)) //Nach hinten
                {   next=position->prev;
                }else if(data->reverse)                                 //Umkehren
                {   if(direction==0)    direction=1;
                    else                direction=0;
                }else                                                   //Der Weg ist blockiert und das umkehren ist nicht erlaubt
                {   timeout=100;
                }
                timeout++;
            }while(next==NULL && timeout<2);                            //Wenn das Timeout 2 ist: beide Nachbarfelder sind blockiert. Oder es gibt nur ein Nachbarfeld und dieses ist blockiert

            if(next!=NULL)                                              //Der Transporter wird nicht blockiert (darf sich bewegen)
            {   //Weiter bewegen
                movement.moving=1;
                movement.progress=0;
                movement.richtung=game->getRailDirection(position->position,next->position);
            }
        }
    }

    ///INTERARKTION MIT ANDEREN ELEMENTEN:
    if(data->type==0)   interactNormal();
    else                interactDeath();

    if(next != NULL)  next->run();                                      //Nächsten Transporter auch simulieren
}


void TRANSPORTER::interactNormal()                                      //Lässt einen normalen Transporter mit anderen Elementen interargieren
{

}

void TRANSPORTER::interactDeath()                                       //Lässt einen tödlichen Transporter mit anderen Elementen interargieren
{   if(movement.moving==0 || (movement.moving!=0 && movement.progress<=100-OccupiedLimit))
    {   game->killObjectsOnField(position->position);                   //Alle Elemente an dieser Position vernichten
    }
    if(movement.moving!=0 && movement.progress>=OccupiedLimit)
    {   if(direction==0)
        {   /*if(position->next==NULL)
            {   error("TRANSPORTER::interactDeath(); next","Das Feld, in das sich der Transporter bewegt, hat die Adresse NULL");return;
            }else*/
            {   game->killObjectsOnField((position->next)->position);       //Alle Elemente der nächsten Position vernichten (next)
            }
        }else
        {   /*if(position->prev==NULL)
            {   error("TRANSPORTER::interactDeath(); prev","Das Feld, in das sich der Transporter bewegt, hat die Adresse NULL");return;
            }else*/
            {   game->killObjectsOnField((position->prev)->position);       //Alle Elemente der nächsten Position vernichten (prev)
            }
        }
    }
}
