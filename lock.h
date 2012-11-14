#ifndef LOCK_H_INCLUDED
#define LOCK_H_INCLUDED

class LOCK
{
    private:
            LOCK *next;///VERKETTETE KLASSE! --> Zeiger auf das nächste Objekt

        ///Allgemeine Daten:
            class GAME *game;           //Pointer auf das GAME-Objekt, dass den Avatar beinhaltet

            //Zur korrekten Darstellung:
            POS *origin;                //Urpsrungs-Koordinaten des gesamten Levels (Ecke links-unten)
            int *elsize;                //Größe eines Elements (Feldes)
            POS *levelSize;             //Levelgröße

        ///Daten:
            POS lock;
            POS key;
            COLOR color;

        ///Hilfsvariablen:
            POS pos;
    public:
        LOCK(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS _lock,POS _key,COLOR _color,LOCK *_next);  //Konstruktor
        LOCK* getNextObject();                                      //Liefert die Adresse des nächsten Objekts zurück
        void setNextObject(LOCK* nextPointer);                      //Ändert den Zeiger des nächsten Elements

        /*alle Elemente*/void print();
        /*alle Elemente*/bool isLocked(POS position);               //Überprüft, ob diese Position von irgendeinem Schloss versperrt wird
        /*alle Elemente*/LOCK* KeyOnField(POS position);            //Überprüft, ob sich auf dieser Position ein Schlüssel befindet

};


#endif // LOCK_H_INCLUDED
