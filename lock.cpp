//Version 0.0
//Jakob Maier
//LOCK-Klasse
LOCK::LOCK(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS _lock,POS _key,COLOR _color,LOCK *_next)  //Konstruktor
{
    game=gamePointer;
    origin=originPointer;
    elsize=elsizePointer;
    levelSize=levelSizePointer;

    lock=_lock;
    key=_key;
    color=_color;
    next=_next;
}

LOCK* LOCK::getNextObject()                                     //Liefert die Adresse des n�chsten Objekts zur�ck
{   return next;
}

void LOCK::setNextObject(LOCK* nextPointer)                     //�ndert den Zeiger des n�chsten Elements
{   next=nextPointer;
}


void LOCK::print()                                              //Alle Schl�sser (und Schl�ssel) ausgeben
{
    pos.x=origin->x+lock.x* (*elsize);
    pos.y=origin->y+(levelSize->y-lock.y-1)* (*elsize);
    leveltiles.print({pos,{pos.x+(*elsize),pos.y+(*elsize)}},{TILE_LOCK%8,TILE_LOCK/8},color);

    pos.x=origin->x+key.x* (*elsize);
    pos.y=origin->y+(levelSize->y-key.y-1)* (*elsize);
    leveltiles.print({pos,{pos.x+(*elsize),pos.y+(*elsize)}},{TILE_KEY%8,TILE_KEY/8},color);

    if(next!=NULL)  next->print();                              //Alle Schl�sser ausgeben
}

bool LOCK::isLocked(POS position)                               //�berpr�ft, ob diese Position von irgendeinem Schloss versperrt wird
{
    if(poscmp(position,lock))
        return 1;
    if(next!=NULL)  return next->isLocked(position);            //Alle Schl�sser pr�fen
    else            return 0;
}


LOCK* LOCK::KeyOnField(POS position)                            //�berpr�ft, ob sich auf dieser Position ein Schl�ssel befindet
{
    if(poscmp(position,key))
        return this;
    if(next!=NULL)  return next->KeyOnField(position);          //Alle Schl�ssel pr�fen
    else            return NULL;
}







