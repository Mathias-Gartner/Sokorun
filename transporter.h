#ifndef TRANSPORTER_H_INCLUDED
#define TRANSPORTER_H_INCLUDED



struct RAIL                     //Doppelt Verkettete Liste
{   POS position;               //Position
    int outputType;             //Welche Art von Schienenstück ausgegeben werden soll (Zusatzinformation damit das nicht bei jedem Leveldurchlauf berechnet werden muss. Wird beim Levelladen berechnet)
        // 0: (5x6; Y-Mirror)   Beginn nach oben
        // 1: (5x6)             Beginn nach unten
        // 2: (6x6; X-Mirror)   Beginn nach rechts
        // 3: (6x6)             Beginn nach links
        // 4: (2x6)             Vertikale Verbindung
        // 5: (3x6)             Horizintale Verbindung
        // 6: (4x6)             Ecke unten-rechts
        // 7: (4x6; X-Mirror)   Ecke unten-links
        // 8: (4x6; X&Y-Mirror) Ecke oben-links
        // 9: (4x6; Y-Mirror)   Ecke oben-rechts
        //(Bei Beamern werden auch Endstücke(0-3) verwendet

    RAIL *next;                 //Zeiger auf das nächste Element
    RAIL *prev;                 //Zeiger auf das vorherige Objekt
};

struct TRANSPORTERorigin
{   RAIL *start;                //Gleise / Weg des Transporters. Gibt einen Endpunkt an, von dem aus die Liste wieder gelöscht werden kann
                                //(Von diesem Punkt aus kann jedes Schienenelement erreicht werden. Wenn der Transporter Bidirektional ist, kann der start-Pointer beliebig sein,
                                // da die Liste im Kreis verkettet ist. Ansonsten (Unidirektional): Es gibt es einen next- und einen prev-Ponter, der NULL hat, und dadruch muss dieser Start-Pointer
                                // auf die Start-Position zeigen (der prev-Pointer im startelement = NULL)
    //Ob ein Transporter Unidirektional oder Bidirektional ist, entscheidet sich dadurch, ob das letzte schienenelement auf das erste Element zeigt oder NULL hat. (und umgekehrt, ob das erste Element aufs letzte Element zurückzeigt)
                                //bool bidirectional;         //1: Transporter fährt hin+zurück; 0: Transporter fährt nur in eine Richtung
    RAIL *origin;               //Zeigt auf das Gleis, auf dem sich der Transporter am Anfang befindet

    int speed;                  //Geschwindigkeit (in %, wieviel der Strecke der Transporter in einem Durchgang zurücklegt)
    bool startDirection;        //In welche Richtung sich der Transporter am Anfang bewegt. 0=Nach vorne (next-Pointer); 1=Zurück (prev-Pointer)

    int type;                   //0: normaler Transporter; 1: tödlicher Transporter

    TRANSPORTERorigin *next;    //Nächstes Element
};


class TRANSPORTER
{
    private:
            TRANSPORTER *next;                      //Verkette Klasse -> zeigt auf den nächsten Transporter

        ///Allgemeine Daten:
            class GAME *game;           //Pointer auf das GAME-Objekt, dass den Avatar beinhaltet

            //Zur Korrekten Darstellung:
            POS *origin;                //Urpsrungs-Koordinaten des gesamten Levels (Ecke links-unten)
            int *elsize;                //Größe eines Elements (Feldes)
            POS *levelSize;             //Levelgröße


        ///Levelstartanimation:
            float minVisibility;        //Sobald das Spiel gestartet wird, werden alle Schienen langsam ausgeblendet. Dieser Wert gibt an, wie sehr die Schienen sichtbar sind (0.0 - 1.0)

        ///Leveldaten:
            TRANSPORTERorigin *data;
        ///Zustandsdaten:
            RAIL *position;                         //Zeiger auf das Schienenelement, auf dem sich der Transporter geraden befindet
            bool direction;                         //Akt. Richtung, in die sich der Transporter bewegt (0: nach vorne (next-Pointer); 1: nach hinten)
            MOVEMENT movement;                      //Bewegung des Transporters
        ///Sonstiges:
            void printRailTrack(RAIL *r,DIRECTION in,float railtracksVisibility,float *alo,float *aro,float *alu,float *aru,float fkt);         //Gibt ein einzelnes Schienenstück aus
    public:

        TRANSPORTER(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,TRANSPORTERorigin *_data,TRANSPORTER *_next);       //Initialisiert den Transporter

        TRANSPORTER* getNextObject();                                   //Liefert die Adresse des nächsten Objekts zurück
        void setNextObject(TRANSPORTER* nextPointer);                   //Ändert den Zeiger des nächsten Elements

        /*alle Elemente*/void print();                                  //Gibt alle Transporter und deren Schienennetz aus
        /*alle Elemente*/void run();                                    //Simuliert jeden Transporter




        ~TRANSPORTER()
        {   //"data" WIRD NICHT FREIGEGEBEN!
            //Wurde nicht in dieser Klasse reserviert, sondern in der Level-Klasse. Muss daher auch von der Levelklasse wieder freigegeben werden

        }

};





void getRailOutputInformation(int outputType,char *tile,bool *mirrorX,bool *mirrorY);       //Gibt Ausgabeinformationen für ein Schienenelement zurück






#endif // TRANSPORTER_H_INCLUDED
