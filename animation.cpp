//Beinhaltet Daten für Animationen und den Animationshandler
//Version 1.0
//Jakob Maier


ANIMATION::ANIMATION(ANITYPE _anitype,int _identification,int _type,bool _richtung,int _start,int _end,int _reverse,float _speed,TEXTURE *_texture,POS _spritePos,AREA _output,COLOR _overlay=WHITE)//Erstellung einer neuen Animation
{
    next=NULL;

    anitype=_anitype;
    identification=_identification;
    if(_type<0 || _type>2)
    {   error("ANIMATION::ANIMATION()","Ungueltiger Wert fuer den Animationstypen uebergeben. _type=%d",_type);
        _type=-1;
        return;
    }
    type=_type;
    richtung=_richtung;
    texture=_texture;
    spriteArea=stdTextArea;

    if(type!=2 && (_spritePos.x<0 || _spritePos.y<0 || _spritePos.x>=(texture->getSprites()).x || _spritePos.y>=(texture->getSprites()).y ))
    {   error("ANIMATION::ANIMATION()","Ungueltigen Wert fuer die Spriteposition uebergeben. _spritePos=(%dx%d)",_spritePos.x,_spritePos.y);
        _type=-1;
        return;
    }
    if(type!=2)     spritePos=_spritePos;
    else            _spritePos={-1,-1};     //Wert wird ignortiert


    if( (type==2 && _start<0) || (type!=2 && (_start<0||_start>100)  ) )
    {   error("ANIMATION::ANIMATION()","Ungueltiger Startwert uebergeben. _start=%d",_start);
        _type=-1;
        return;
    }
    start=_start;

    if( (/*type!=1 &&*/ _end<=start) || (type==2 && _end>=texture->getSpriteAnz()) || (type!=2 && (_end<0||_end>100)  ) )
    {   error("ANIMATION::ANIMATION()","Ungueltiger Endwert uebergeben. _end=%d",_end);
        _type=-1;
        return;
    }
    end=_end;

    if(_reverse<0||_reverse>3)
    {    error("ANIMATION::ANIMATION()","Ungueltiger reverse-Wert uebergeben. _reverse=%d",_reverse);
        _type=-1;
        return;
    }
    reverse=_reverse;
    if(_speed>100)
    {   error("ANIMATION::ANIMATION()","Ungueltige Geschwindigkeit uebergeben. speed wird auf den Maximalwert 100 gesetzt. _speed=%d",_speed);
        _speed=100;
    }
    speed=_speed;

    output=_output;
    overlay=_overlay;
    if(colorcmp(overlay,BLACK) && type==0)   overlay=WHITE;//Umbessern
    if(richtung==0)//normal
        if(type==2)         progress=0;         //0% (=erstes Bild)
        else                progress=start;
    else//Rückwärts
        if(type==2)         progress=100;       //100% (=letztes Bild)
        else                progress=end;
    finished=0;
}

void ANIMATION::setProgress(int newProgress)                  //Fortschritt manuell ändern
{   if(type==2)
    {   if(newProgress<0||newProgress>100)
            error("ANIMATION::setProgress()","Ungueltiger Fortschrittswert uebergeben. newProgress=%d",newProgress);
        else
            progress=newProgress;
        return;
    }else
    {   if(newProgress<start||newProgress>end)
            error("ANIMATION::setProgress()","Ungueltiger Fortschrittswert uebergeben. newProgress=%d, start=%d, end=%d",newProgress,start,end);
        else
            progress=newProgress;
        return;
    }
}


int ANIMATION::getIdentification()                              //Gibt die Identifikationsnummer zurück
{   return identification;
}

void ANIMATION::print()                                         //Ausgabe
{   if(finished && reverse==0)  return;                         //Keine Ausgabe mehr
    switch(type)
    {   case 0: print_T0(); break;
        case 1: print_T1(); break;
        case 2: print_T2(); break;
    }
}

void ANIMATION::print_T0()                                      //Ausgabe für Typ 0
{   /** EINFÄRBEN **/
    texture->print(output,spritePos,spriteArea,COLOR{overlay.r*progress/100,overlay.g*progress/100,overlay.b*progress/100});

    if(ImgDebug)    markArea(output,MAGENTA);
}

void ANIMATION::print_T1()                                      //Ausgabe für Typ 1
{   /** DREHEN **/
    texture->print(output.a,output.b.x/*=size*/,spritePos,progress*3.6,overlay);
}

void ANIMATION::print_T2()                                      //Ausgabe für Typ 2
{   /** BILDFOLGE **/

    //Feststellen, welche Bildnummer ausgegeben werden soll:

    int spriteAnz=end-start+1;                                  //Anzahl der Sprites die verwendet werden
    int spriteNum=start+progress/(100/spriteAnz);
    if(spriteNum>=end)  spriteNum=end;                          //bei 100% wird ein ungültiges Bild errechnet (0-100% möglich --> 101 Möglichkeiten)

    //Sprite-Position ermitteln:
    POS sp=texture->getSprites();
    texture->print(output,{spriteNum%sp.x,spriteNum/sp.x},spriteArea,overlay);

    if(ImgDebug)    markArea(output,MAGENTA);
}

bool ANIMATION::isFinished()                                    //Gibt zurück, ob die Animation bereits abgeschlossen wurde (1=ja)
{   if(finished && (reverse==3||reverse==0))  return 1;         //Keine Simulation mehr
    return 0;
}

bool ANIMATION::run()                                           //Simulationsschritt durchführen. Rückgabewert=1: Simulation in eine Richtung abgeschlossen, beim nächsten Durchgang wiederholt sich alles
{   if(finished && (reverse==3||reverse==0))  return 1;         //Keine Simulation mehr
    if(richtung)//Rückwärts
    {   progress-=speed;
        if(type==2)
        {   if(progress<=0)
            {   if(reverse==1)      {progress=0; richtung=0;}
                else if(reverse==2)  progress=100;
                else                {progress=0; finished=1;}   //reverse= 0/3
                return 1;
            }
        }else
        if(progress<=start)
        {   if(reverse==1)          {progress=start; richtung=0;}
            else  if(reverse==2)     progress=end;
            else                    {progress=100;   finished=1;}   //reverse= 0/3
            return 1;
        }
    }else//Vorwärts
    {   progress+=speed;
        if(type==2)//Texturen
        {   if(progress>=100)
            {   if(reverse==1)      {progress=100; richtung=1;}
                else if(reverse==2)  progress=0;
                else finished=1;
                return 1;
            }
        }else
        if(progress>=end)
        {   if(reverse==1)          {progress=end;   richtung=1;}
            else if(reverse==2)      progress=start;
            else finished=1;
            return 1;
        }
    }
    return 0;
}

int ANIMATION::getType()                                        //Gibt den Animationstyp zurück
{   return type;
}

void ANIMATION::setNextPointer(ANIMATION* _next)                //Pointer setzen
{   next=_next;
}

ANIMATION* ANIMATION::getNextPointer()                          //Gibt Pointer zurück
{   return next;
}

int ANIMATION::getReverse()                                     //Gibt den Reverse-Wert zurück
{   return reverse;
}

ANITYPE ANIMATION::getAnitype()                                 //Gibt den Animationstypen zurück
{   return anitype;
}

void ANIMATION::setSpriteArea(fAREA _spriteArea)                //spriteArea setzen
{   spriteArea=_spriteArea;
}

void ANIMATION::setSpritePos(POS _spritePos)                    //Sprite ändern
{   spritePos=_spritePos;
}



ANIMATIONGROUP::ANIMATIONGROUP()                                //Konstruktor
{   start=NULL;
    IDcounter=0;
}

int ANIMATIONGROUP::add(ANITYPE _anitype,int _type,bool _richtung,int _start,int _end,int _reverse,float _speed,TEXTURE *_texture,POS _spritePos,AREA _output,COLOR _overlay=WHITE)//Hinzufügen einer neuen Animation
{   ANIMATION *neu;
    neu=new class ANIMATION(_anitype,++IDcounter,_type,_richtung,_start,_end,_reverse,_speed,_texture,_spritePos,_output,_overlay);
    if(neu->getType() <0 )//Fehler
    {    delete neu;
        return -1;
    }
    else//Am Anfang einfügen
    {   neu->setNextPointer(start);
        start=neu;
        return IDcounter;   //ID zurückgeben
    }
}

void ANIMATIONGROUP::print(int identification)                  //Gibt alle Animationen einer bestimmten ID aus (0=ALLE)
{
    ANIMATION *p=start;
    while(p!=NULL)
    {   if(identification==0 || p->getIdentification()==identification)
            p->print();                                         //Ausgeben
        p=p->getNextPointer();                                  //Nächstes Element
    }
}

ANIMATIONGROUP::~ANIMATIONGROUP()                               //Destruktor - Löscht alle Daten
{   ANIMATION *p=start,*q;
    while(p!=NULL)
    {   q=p->getNextPointer();                                  //Nächstes Element
        delete p;
        p=q;
    }
}

void ANIMATIONGROUP::setSpriteArea(int identification,fAREA _spriteArea)        //spriteArea setzen (zB. zum spiegeln)
{
    ANIMATION *p=start;
    while(p!=NULL)
    {   if(identification==0 || p->getIdentification()==identification)
        {   p->setSpriteArea(_spriteArea);
            return;
        }
        p=p->getNextPointer();                                  //Nächstes Element
    }
}

void ANIMATIONGROUP::setSpritePos(int identification,POS _spritePos)        //spritePosition ändern
{
    ANIMATION *p=start;
    while(p!=NULL)
    {   if(identification==0 || p->getIdentification()==identification)
        {   p->setSpritePos(_spritePos);
            return;
        }
        p=p->getNextPointer();                                  //Nächstes Element
    }
}

bool ANIMATIONGROUP::run(int identification)//Simuliert alle Animationen einer bestimmten ID (0=ALLE) (gibt zurück ob ein Element fertig ist und gelöscht wurde)
{
    ANIMATION *p=start,*last=NULL;
    bool finished=0;

    while(p!=NULL)
    {   if(identification==0 || p->getIdentification()==identification)
        {   if(p->run()==1 && p->getReverse()==0)               //Falls die Simulation fertig ist und gelöscht werden soll
            {   if(last==NULL)  //Erstes Element
                {   start=p->getNextPointer();
                    delete p;
                    p=start;
                }else
                {   last->setNextPointer(p->getNextPointer());
                    delete p;
                    p=last->getNextPointer();
                }
                finished=1;
                continue;
            }
        }
        last=p;
        p=p->getNextPointer();                                  //Nächstes Element
    }
    return finished;
}

void ANIMATIONGROUP::print(ANITYPE group)                                       //Gibt alle Animationen einer bestimmten Gruppe aus
{
    ANIMATION *p=start;
    while(p!=NULL)
    {   if(p->getAnitype()==group)
            p->print();                                         //Ausgeben
        p=p->getNextPointer();                                  //Nächstes Element
    }
}

bool ANIMATIONGROUP::run(ANITYPE group)                         //Simuliert alle Animationen einer bestimmten Gruppe (gibt zurück ob ein Element fertig ist und gelöscht wurde)
{
    ANIMATION *p=start,*last=NULL;
    bool deleted=0;                                             //Ob ein Element gelöscht wurde

    while(p!=NULL)
    {   if(p->getAnitype()==group)
        {
            if(p->run()==1 && p->getReverse()==0)               //Falls die Simulation fertig ist und gelöscht werden soll
            {   if(last==NULL)  //Erstes Element
                {   start=p->getNextPointer();
                    delete p;
                    p=start;
                }else
                {   last->setNextPointer(p->getNextPointer());
                    delete p;
                    p=last->getNextPointer();
                }
                deleted=1;
                continue;
            }
        }
        last=p;
        p=p->getNextPointer();                                  //Nächstes Element
    }
    return deleted;
}

//int ANIMATIONGROUP::getAnimationAnz(ANITYPE group)              //Gibt die Anzahl der Animationen mit diesem Typ zurück
//{   int anz=0;
//    ANIMATION *p=start;
//    while(p!=NULL)
//    {   if(p->getAnitype()==group)
//            anz++;                                              //Ausgeben
//        p=p->getNextPointer();                                  //Nächstes Element
//    }
//    return anz;
//}

int ANIMATIONGROUP::getActiveAnimationAnz(ANITYPE group)        //Gibt die Anzahl der Animationen mit diesem Typ zurück, welche noch nicht abgeschlossen sind
{   int anz=0;
    ANIMATION *p=start;
    while(p!=NULL)
    {   if(p->getAnitype()==group && !p->isFinished())
            anz++;                                              //Ausgeben
        p=p->getNextPointer();                                  //Nächstes Element
    }
    return anz;
}

void ANIMATIONGROUP::remove(ANITYPE group)                      //Löscht alle Animationen einer bestimmten Gruppe aus der Liste
{
    ANIMATION *p=start,*last=NULL;

    while(p!=NULL)
    {   if(group==0 || p->getAnitype()==group)
        {   if(last==NULL)  //Erstes Element
            {   start=p->getNextPointer();
                delete p;
                p=start;
            }else
            {   last->setNextPointer(p->getNextPointer());
                delete p;
                p=last->getNextPointer();
            }
            continue;
        }
        last=p;
        p=p->getNextPointer();                                  //Nächstes Element
    }
}

void ANIMATIONGROUP::remove(int identification)                 //Löscht eine bestimmte Animation
{
    ANIMATION *p=start,*last=NULL;

    while(p!=NULL)
    {   if(p->getIdentification()==identification)
        {   if(last==NULL)  //Erstes Element
            {   start=p->getNextPointer();
                delete p;
            }else
            {   last->setNextPointer(p->getNextPointer());
                delete p;
            }
            return;
        }
        last=p;
        p=p->getNextPointer();                                  //Nächstes Element
    }
}
