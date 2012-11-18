#ifndef TRANSPORTER_H_INCLUDED
#define TRANSPORTER_H_INCLUDED



struct RAIL
{   POS position;
    RAIL *next;
};

struct TRANSPORTERorigin
{   RAIL *start;                //Gleise / Weg des Transporters
    bool bidirectional;         //1: Transporter fährt hin+zurück; 0: Transporter fährt nur in eine Richtung
    RAIL *origin;               //Zeigt auf das erste Gleis, auf dem sich der Transporter am Anfang befindet
    int speed;                  //Geschwindigkeit (in %, wieviel der Strecke der Transporter in einem Durchgang zurücklegt)

    int type;                   //0: normaler Transporter; 1: tödlicher Transporter

    TRANSPORTERorigin *next;    //Nächstes Element
};


















#endif // TRANSPORTER_H_INCLUDED
