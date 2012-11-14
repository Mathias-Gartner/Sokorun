//Version 0.1
//Jakob Maier
//...
//Allgemeine Funktionen



bool poscmp(POS a,POS b)            //Vergleicht 2 Positionen auf �bereinstimmung
{   if(a.x==b.x&&a.y==b.y)
       return 1;
    return 0;
}


//�bergabeparameter:
//  1.) position        [POS]       Koordinate, deren y-Wert gespiegelt werden soll
//R�ckgabewert:
//  1.) position        [POS]       gespiegelte Koordinate
void transformY(POS *position)//Die Y-Koordinate wird gespiegelt (Ursprung oben/unten vertauscht)
{   position->y=windowSize.y-position->y;
}


//�bergabeparameter:
//  1.) coord           [POS]       Koordinaten, die �berpr�ft werden sollen
//  2.) gebiet          [AREA]      Gebiet, das �berpr�ft werden soll
//R�ckgabewert:
//  -)  Ob sich die Maus im Gebiet befindet (true: ja, false: nein)
bool coordInside(POS coord,AREA gebiet)//Meldet, ob sich die �bergebenen Koordinaten innerhalb des �bergebenen Gebiets befinden (zB. f�r "Maus �ber Bild?)
{   if((coord.x>=gebiet.a.x && coord.x<=gebiet.b.x) && (coord.y>=gebiet.a.y && coord.y<=gebiet.b.y))
        return true;
    return false;
}




//�bergabeparameter:
// 1.) *data            [AREA]
//          data->a         Startposition
//          data->b         Gr��e
//R�ckgabeparameter:
// 1.) *data            [AREA]
//          data->a         Startposition
//          data->b         Endposition
void PosSizeToArea(AREA *data)//Wandelt eine Struktur, die aus Position+Gr��e besteht um, in Startposition+Endposition
{   data->b.x += data->a.x;
    data->b.y += data->a.y;
}
AREA PosSizeToArea(AREA data)//Wandelt eine Struktur, die aus Position+Gr��e besteht um, in Startposition+Endposition
{   data.b.x += data.a.x;
    data.b.y += data.a.y;
    return data;
}



bool propRicCmp(DIRECTION richtung,FIELDPROPERTY property)              //Vergleicht die beiden unterschiedlichen Datentypen auf Gleichheit (1=gleich)
{
    if((richtung==UP && property==up) || (richtung==DOWN && property==dn) || (richtung==RIGHT && property==rt) || (richtung==LEFT && property==lt) || (richtung==BEAM && property==bm))
        return 1;
    return 0;
}

bool colorcmp(COLOR a,COLOR b)  //Vergleicht 2 Farben auf gleichheit (1=gleich)
{   if(a.r==b.r && a.g==b.g && a.b==b.b)
        return 1;
    return 0;
}

DIRECTION turnRight(DIRECTION richtung)                                 //Gibt die um 90� im Uhrzeigersinn gedrehte Richtung zur�ck
{   switch(richtung)
    {   case UP:    return RIGHT;
        case RIGHT: return DOWN;
        case DOWN:  return LEFT;
        case LEFT:  return UP;
    }
    return NONE;
}

DIRECTION turnLeft(DIRECTION richtung)                                 //Gibt die um 90� gegen den Uhrzeigersinn gedrehte Richtung zur�ck
{   switch(richtung)
    {   case UP:    return LEFT;
        case RIGHT: return UP;
        case DOWN:  return RIGHT;
        case LEFT:  return DOWN;
    }
    return NONE;
}
