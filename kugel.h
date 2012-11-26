#ifndef KUGEL_H_INCLUDED
#define KUGEL_H_INCLUDED

struct KUGELorigin              //Verkettete Liste mit allen Kugel-Daten die aus der Level-Datei geladen werden
{   POS origin;                 //Startposition
    int type;                   //Kugeltyp
    KUGELorigin *next;          //N�chste Kugel
};

class KUGEL
{
    private:
            KUGEL *next;                ///VERKETTETE KLASSE! --> Zeiger auf das n�chste Objekt

        ///Allgemeine Daten:
            class GAME *game;           //Pointer auf das GAME-Objekt, dass den Avatar beinhaltet

            //Zur Korrekten Darstellung:
            POS *origin;                //Urpsrungs-Koordinaten des gesamten Levels (Ecke links-unten)
            int *elsize;                //Gr��e eines Elements (Feldes)
            POS *levelSize;             //Levelgr��e

        ///Daten:
            int type;                   //Kugeltyp
                //-1: Diese Kugel hat sich selbst zum l�schen markiert. --> Sie wird beim n�chsten GAME::cleanup() - Aufruf entfernt
                //0: normal
                //1: runder Block (kann Lava verdr�ngen)
            POS position;
            MOVEMENT movement;          //Bewegung der Kugel

        ///Kugel-Einblende-Animation:
            int buildupCounter;         //Fortschritts-z�hler
            int buildupImage;

    public:
        KUGEL(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS kugelOrigin,int _type,KUGEL *nextKugel);
        KUGEL* getNextObject();                                     //Liefert die Adresse des n�chsten Objekts zur�ck
        void setNextObject(KUGEL* nextPointer);                      //�ndert den Zeiger des n�chsten Elements

        int getType();                                              //Gibt den Kugeltyp zur�ck
        //*alle Elemente*/bool setupAnimation();                     //langsames Einblenden
        /*alle Elemente*/void print();
        /*alle Elemente*/void run();                                //F�hrt einen Simulationsschritt durch
        /*alle Elemente*/KUGEL* KugelOnField(POS position,KUGEL* ignore);         //�berpr�ft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zur�ckgegeben
        /*alle Elemente*/void killOnField(POS pos);                 //zerst�rt die Kugel, wenn sie die �bergebene Position blockiert

        void move(DIRECTION richtung);                              //Kugel ansto�en
        /*alle Elemente*/void stopMovementsTo(POS pos,int limit);   //Wenn sich eine Kugel gerade auf dieses Feld zubewegt: abprallen lassen
        /*alle Elemente*/bool isMoving();                           //gibt zur�ck, ob sich gerade eine Kugel bewegt
};

#endif // KUGEL_H_INCLUDED
