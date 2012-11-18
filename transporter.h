#ifndef TRANSPORTER_H_INCLUDED
#define TRANSPORTER_H_INCLUDED



struct RAIL
{   POS position;
    RAIL *next;
};

struct TRANSPORTERorigin
{   RAIL *start;                //Gleise / Weg des Transporters
    bool bidirectional;         //1: Transporter f�hrt hin+zur�ck; 0: Transporter f�hrt nur in eine Richtung
    RAIL *origin;               //Zeigt auf das erste Gleis, auf dem sich der Transporter am Anfang befindet
    int speed;                  //Geschwindigkeit (in %, wieviel der Strecke der Transporter in einem Durchgang zur�cklegt)

    int type;                   //0: normaler Transporter; 1: t�dlicher Transporter

    TRANSPORTERorigin *next;    //N�chstes Element
};


















#endif // TRANSPORTER_H_INCLUDED
