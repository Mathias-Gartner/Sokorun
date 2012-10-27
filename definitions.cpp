//Version 0.1
//Jakob Maier
//...
//Allgemeine Funktionen


const fAREA stdTextArea={{0.0f,0.0f},{1.0f,1.0f}};




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


//R�ckgabeparameter:
//  1.) coord           [POS]   Position der Maus im Koordinatensystem
void getMousePos(POS *coord)//Gibt die Position der Maus im Koordinatensystem zur�ck
{   glfwGetMousePos(&(coord->x),&(coord->y));
    //Koordinatensystem umrechnen:
    coord->x/=coordPixel;
    coord->y/=coordPixel;
    transformY(coord);
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
