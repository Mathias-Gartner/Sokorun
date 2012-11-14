#ifndef LOCK_H_INCLUDED
#define LOCK_H_INCLUDED

class LOCK
{
    private:
            LOCK *next;///VERKETTETE KLASSE! --> Zeiger auf das n�chste Objekt

        ///Allgemeine Daten:
            class GAME *game;           //Pointer auf das GAME-Objekt, dass den Avatar beinhaltet

            //Zur korrekten Darstellung:
            POS *origin;                //Urpsrungs-Koordinaten des gesamten Levels (Ecke links-unten)
            int *elsize;                //Gr��e eines Elements (Feldes)
            POS *levelSize;             //Levelgr��e

        ///Daten:
            POS lock;
            POS key;
            COLOR color;

        ///Hilfsvariablen:
            POS pos;
    public:
        LOCK(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS _lock,POS _key,COLOR _color,LOCK *_next);  //Konstruktor
        LOCK* getNextObject();                                      //Liefert die Adresse des n�chsten Objekts zur�ck
        void setNextObject(LOCK* nextPointer);                      //�ndert den Zeiger des n�chsten Elements

        /*alle Elemente*/void print();
        /*alle Elemente*/bool isLocked(POS position);               //�berpr�ft, ob diese Position von irgendeinem Schloss versperrt wird
        /*alle Elemente*/LOCK* KeyOnField(POS position);            //�berpr�ft, ob sich auf dieser Position ein Schl�ssel befindet

};


#endif // LOCK_H_INCLUDED
