#ifndef KUGEL_H_INCLUDED
#define KUGEL_H_INCLUDED

struct KUGELorigin              //Verkettete Liste mit allen Kugel-Daten die aus der Level-Datei geladen werden
{   POS origin;                 //Startposition
    int type;                   //Kugeltyp
    KUGELorigin *next;          //Nächste Kugel
};

class KUGEL
{
    private:
            KUGEL *next;                ///VERKETTETE KLASSE! --> Zeiger auf das nächste Objekt

        ///Allgemeine Daten:
            class GAME *game;           //Pointer auf das GAME-Objekt, dass den Avatar beinhaltet

            //Zur Korrekten Darstellung:
            POS *origin;                //Urpsrungs-Koordinaten des gesamten Levels (Ecke links-unten)
            int *elsize;                //Größe eines Elements (Feldes)
            POS *levelSize;             //Levelgröße

        ///Daten:
            int type;                   //Kugeltyp
                //-1: Diese Kugel hat sich selbst zum löschen markiert. --> Sie wird beim nächsten GAME::cleanup() - Aufruf entfernt
                //0: normal
                //1: runder Block (kann Lava verdrängen)
            POS position;
            MOVEMENT movement;          //Bewegung der Kugel

        ///Kugel-Einblende-Animation:
            int buildupCounter;         //Fortschritts-zähler
            int buildupImage;

    public:
        KUGEL(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS kugelOrigin,int _type,KUGEL *nextKugel);
        KUGEL* getNextObject();                                     //Liefert die Adresse des nächsten Objekts zurück
        void setNextObject(KUGEL* nextPointer);                      //Ändert den Zeiger des nächsten Elements

        int getType();                                              //Gibt den Kugeltyp zurück
        //*alle Elemente*/bool setupAnimation();                     //langsames Einblenden
        /*alle Elemente*/void print();
        /*alle Elemente*/void run();                                //Führt einen Simulationsschritt durch
        /*alle Elemente*/KUGEL* KugelOnField(POS position,KUGEL* ignore);         //Überprüft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zurückgegeben
        /*alle Elemente*/void killOnField(POS pos);                 //zerstört die Kugel, wenn sie die übergebene Position blockiert

        void move(DIRECTION richtung);                              //Kugel anstoßen
        /*alle Elemente*/void stopMovementsTo(POS pos,int limit);   //Wenn sich eine Kugel gerade auf dieses Feld zubewegt: abprallen lassen
        /*alle Elemente*/bool isMoving();                           //gibt zurück, ob sich gerade eine Kugel bewegt
};

#endif // KUGEL_H_INCLUDED
