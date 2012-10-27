//Version 0.1
//Jakob Maier

#ifndef TYPEDEF_H_INCLUDED
#define TYPEDEF_H_INCLUDED


/** EINSTELLUNGEN **/
    #define AutoAdjustWindowSize 1 //Ob bei jeder Loop, in der die Anzeige aktualisiert wird, auch die Fenstergr��e angepasst werden soll





/** TYPE DEFINITIONS **/

    struct POS{
        int x,y;
    };
    struct AREA{
        POS a;
        POS b;
    };
    struct fPOS{
        float x,y;
    };
    struct fAREA{
        fPOS a;
        fPOS b;
    };


    struct COLOR{
        float r,g,b;
    };
    #define WHITE   COLOR{1.0f,1.0f,1.0f}
    #define BLACK   COLOR{0.0f,0.0f,0.0f}
    #define RED     COLOR{1.0f,0.0f,0.0f}
    #define GREEN   COLOR{0.0f,1.0f,0.0f}
    #define BLUE    COLOR{0.0f,0.0f,1.0f}
    #define YELLOW  COLOR{1.0f,1.0f,0.0f}
    #define MAGENTA COLOR{1.0f,0.0f,1.0f}
    #define CYAN    COLOR{0.0f,1.0f,1.0f}


    enum DIRECTION{
        TOP,
        BOTTOM,
        LEFT,
        RIGHT
    };

/** GLOBALE VARIABLEN **/

    const POS windowSize={1200,900};      //Fenstergr��e - Anzahl der Koordinatenpunkte die erreicht werden k�nnen
    #define windX windowSize.x
    #define windY windowSize.y

    float coordPixel=0.7;                 //Wieviele Pixel eine Koordinateneinheit am Bildschirm bekommt





/** PROTOTYPEN **/

    POS transformY(POS position);                                                                   //Die Y-Koordinate wird gespiegelt (Ursprung oben/unten vertauscht)
    bool coordInside(POS coord,AREA gebiet);                                                        //Meldet, ob sich die �bergebenen Koordinaten innerhalb des �bergebenen Gebiets befinden (zB. f�r "Maus �ber Bild?)
    void getMousePos(POS *coord);                                                                   //Gibt die Position der Maus im Koordinatensystem zur�ck
    void PosSizeToArea(AREA *data);                                                                 //Wandelt eine Struktur, die aus Position+Gr��e besteht um, in Startposition+Endposition
    AREA PosSizeToArea(AREA data);                                                                  //Wandelt eine Struktur, die aus Position+Gr��e besteht um, in Startposition+Endposition





#endif // TYPEDEF_H_INCLUDED
