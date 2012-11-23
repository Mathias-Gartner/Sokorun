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

    movement.moving=0;                                                      //Bewegt sich gerade nicht
    movement.progress=0;
    movement.lastRichtung=NONE;
    movement.blocking=0;
}

TRANSPORTER* TRANSPORTER::getNextObject()                                   //Liefert die Adresse des nächsten Objekts zurück
{  return next;
}

void TRANSPORTER::print()
{
    /*HIER: SCHIENEN AUSGEBEN*/
    game->printMovingObject(&movement,position->position,data->type==0?TILE_TRANSPORTER:TILE_DEATHTRANSPORTER);
    if(next!=NULL)  next->print();  //Alle Transporter ausgeben
}



void TRANSPORTER::run()                                                     //Führt einen Simulationsschritt durch
{

    if(next != NULL)  next->run();                                          //Nächsten Transporter auch simulieren
}
