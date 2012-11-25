//Daten für die Transporter, die sich durchs Spielfeld bewegen
//Version 1.0
//Jakob Maier


TRANSPORTER::TRANSPORTER(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,TRANSPORTERorigin *_data,TRANSPORTER *_next)       //Initialisiert den Transporter
{
    next=_next;

    game=gamePointer;
    origin=originPointer;                       //Position, wo das LEvel ausgegeben wird (links unten)
    elsize=elsizePointer;                       //Größe eines Elementes/Feldes
    levelSize=levelSizePointer;                 //Levelgröße

    data=_data;
    //position=(data->origin)->position;
    position=data->origin;

    movement.moving=1;
    movement.richtung=UP;                                                      //Bewegt sich gerade nicht
    movement.progress=10;
    movement.lastRichtung=NONE;
    movement.blocking=0;

    direction=data->startDirection;             //Richtung, in die sich der Transporter los bewegt

    minVisibility=1.0f;
}

TRANSPORTER* TRANSPORTER::getNextObject()                                   //Liefert die Adresse des nächsten Objekts zurück
{  return next;
}


#define RAILTRACKS_VISIBILITY_NXT 0.25f     //Um wieviel % die Sichtbarkeit eines Schienenstücks abnimmt - für die nächsten Schienen
#define RAILTRACKS_VISIBILITY_PRV 0.9f      //Um wieviel % die Sichtbarkeit eines Schienenstücks abnimmt - für die bereits passierten Schienen

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
                }else   fkt=1;
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
        for(;;);
    }
    game->printMovingObject(&movement,position->position,data->type==0?TILE_TRANSPORTER:TILE_DEATHTRANSPORTER,nxt);
    if(next!=NULL)  next->print();  //Alle Transporter ausgeben
}



void TRANSPORTER::run()                                                     //Führt einen Simulationsschritt durch
{   if(minVisibility>0) minVisibility-=0.025;                               //Schienenelemente langsam ausblenden


    movement.blocking=0;                                                   //Falls der Avatar blockiert wird dieser immer auf moving=1 und -1 gesetzt --> collision-Animation muss verhindert werden können

    if(movement.moving!=0)
    {   movement.progress+=movement.moving*data->speed;                   //Vor bzw. Zurück Bewegen




//        if(movement.moving==1 && movement.limit<100 && movement.progress>=movement.limit)          //Prüfen, ob er jetzt vlt. abprallt
//        {   movement.moving=-1;                                         //Abprallen
//            movement.progress=movement.limit;                           //Genau an die Kante setzen
//            if(!movement.blocking)
//            {   game->addFieldEffect(position,COLLISION,movement.richtung,AVATARCOLLITIONCOLOR);
//                /*Überprüfen, ob sich ein anderes Objekt gerade in dieses Feld bewegen will, in die der Avatar jetzt zurückkehren muss:*/
//                game->stopMovementsTo(position,Wava); //Alle Objekte nach Wava % abprallen lassen (sofern möglich.)
//            }
//        }

        ///Prüfen, ob die Bewegung schon abgeschlossen wurde:
//        if(movement.moving==-1 && movement.progress<=0)                 //Ins Startfeld zurückgekehrt
//        {   movement.moving=0;
//            movement.lastRichtung=NONE;
//        }
        if(movement.moving==1 && movement.progress>=100)                //Im Zielfeld angekommen
        {   movement.lastRichtung=movement.richtung;
            //Neues Feld belegen:
            if(direction==0)    position=position->next;
            else                position=position->prev;

            //Nächstes Feld suchen (und Richtung herausfinden. Wenn nötig den Transporter umkehren lassen):
            RAIL *next;
            if(direction==0)//Nach vorne
            {   if(position->next!=NULL)
                    next=position->next;
                else                                                    //umkehren
                {   next=position->prev;
                    direction=1;
                }
            }else//Nach hinten
            {   if(position->prev!=NULL)
                    next=position->prev;
                else                                                    //umkehren
                {   next=position->next;
                    direction=0;
                }
            }
            movement.progress=0;
            movement.richtung=game->getRailDirection(position->position,next->position);
            movement.limit=100;
        }
    }
//    if(movement.moving==0)                                              //Steht still --> prüfen, ob sich der Avatar auf einem Spezialfeld befindet
//    {   switch(game->getFieldProperty(OBJ_AVATAR,position))             //Dieses Feld hat vlt. eine spezielle Eigenschaft
//        {   case up: move(UP);      break;
//            case dn: move(DOWN);    break;
//            case lt: move(LEFT);    break;
//            case rt: move(RIGHT);   break;
//            case ic: move(movement.lastRichtung);    break;             //Weiter rutschen
//            case bm: move(BEAM);    break;                              //Zum Ziel-Beamer versetzen
//            case tr: move(turnRight(movement.lastRichtung)); break;     //Nach rechts (90° im Uhrzeigersinn) weiterruthschen
//            case tl: move(turnLeft (movement.lastRichtung)); break;     //Nach links (90° gegen Uhrzeigersinn) weiterruthschen
//            case dt:{   /*Tödlich*/
//                        int feld=game->getField(position);              //Feldtyp herausfinden
//                        switch(feld)
//                        {   case TILE_LAVA:     game->addFieldEffect(position,AVATARLAVA); break;                /*lava*/
//                            default:            error("AVATAR::run()","Ubekanntes, toedliches Feld. Es wird keine Animations ausgegeben. feld: %d",feld);
//                        }
//                        game->addGameLogEvent(AVATARDEATH);             //Ereignis berichten
//                        game->addGameLogEvent(GAMEOVER);                //Ereignis berichten
//                        deathProgress=1;                                //Den Avatar zu Tode verurteilen
//                    }break;
//            case fx: /*do nothing*/ break;
//            case nm: /*do nothing*/ break;
//            default: error("AVATAR::run()","Unbekannte Feldeigenschaft gefunden. Verarbeite wert als \"nm\" (Normal). fieldproperty=%d",game->getFieldProperty(OBJ_KUGEL,position));
//        }
//    }



    if(next != NULL)  next->run();                                          //Nächsten Transporter auch simulieren
}
