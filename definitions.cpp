//Version 0.1
//Jakob Maier
//...
//Allgemeine Funktionen


const fAREA stdTextArea={{0.0f,0.0f},{1.0f,1.0f}};




//Übergabeparameter:
//  1.) position        [POS]       Koordinate, deren y-Wert gespiegelt werden soll
//Rückgabewert:
//  1.) position        [POS]       gespiegelte Koordinate
void transformY(POS *position)//Die Y-Koordinate wird gespiegelt (Ursprung oben/unten vertauscht)
{   position->y=windowSize.y-position->y;
}


//Übergabeparameter:
//  1.) coord           [POS]       Koordinaten, die überprüft werden sollen
//  2.) gebiet          [AREA]      Gebiet, das Überprüft werden soll
//Rückgabewert:
//  -)  Ob sich die Maus im Gebiet befindet (true: ja, false: nein)
bool coordInside(POS coord,AREA gebiet)//Meldet, ob sich die übergebenen Koordinaten innerhalb des übergebenen Gebiets befinden (zB. für "Maus über Bild?)
{   if((coord.x>=gebiet.a.x && coord.x<=gebiet.b.x) && (coord.y>=gebiet.a.y && coord.y<=gebiet.b.y))
        return true;
    return false;
}


//Rückgabeparameter:
//  1.) coord           [POS]   Position der Maus im Koordinatensystem
void getMousePos(POS *coord)//Gibt die Position der Maus im Koordinatensystem zurück
{   glfwGetMousePos(&(coord->x),&(coord->y));
    //Koordinatensystem umrechnen:
    coord->x/=coordPixel;
    coord->y/=coordPixel;
    transformY(coord);
}

//Übergabeparameter:
// 1.) *data            [AREA]
//          data->a         Startposition
//          data->b         Größe
//Rückgabeparameter:
// 1.) *data            [AREA]
//          data->a         Startposition
//          data->b         Endposition
void PosSizeToArea(AREA *data)//Wandelt eine Struktur, die aus Position+Größe besteht um, in Startposition+Endposition
{   data->b.x += data->a.x;
    data->b.y += data->a.y;
}
AREA PosSizeToArea(AREA data)//Wandelt eine Struktur, die aus Position+Größe besteht um, in Startposition+Endposition
{   data.b.x += data.a.x;
    data.b.y += data.a.y;
    return data;
}
