#ifndef AVATAR_H_INCLUDED
#define AVATAR_H_INCLUDED


class AVATAR
{
    private:
        ///Allgemeine Daten:
            TEXTURE *levelSprites;
            class GAME *game;           //Pointer auf das GAME-Objekt, dass den Avatar beinhaltet

            //Zur Korrekten Darstellung:
            POS *origin;                //Urpsrungs-Koordinaten des gesamten Levels (Ecke links-unten)
            int *elsize;                //Größe eines Elements (Feldes)
            POS *levelSize;             //Levelgröße

        ///Daten:
            int death;                  //Wenn der Avatar Tot ist: Animations-ID, oder -1 (wenn es keine Animation gibt);   0: Der Avatar ist noch nicht Tot

            POS position;
            MOVEMENT movement;          //Bewegung des Avatars
            bool userinput;             //1: Der Avatar bewegt sich, weil der User das gewollt hat (Wird nur abgespeichert, damit der Gamelog nach dem erreichen des Zielfelds weiß, warum sich der Avatar bewegt hat. NICHT FÜR DIE PHYSIK notwendig)
                                        //Befindet sich nicht der Struktur "movement", weil diese Variable nur für den Avatar wichtig ist

        ///Avatar-Einblende-Animation:
            int buildupCounter;         //Fortschritts-zähler
            int buildupImage;

        ///Sonstiges:

    public:
        AVATAR(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS avatarOrigin);

        //bool setupAnimation();                          //langsames Einblenden


        void print();

        void move(DIRECTION richtung,bool userinput);   //Der Avatar wird angestoßen (userinput=1: Benutzereigabe -nur wenn sich sonst nichts bewegt)
        void stopMovementTo(POS pos,int limit);         //Wenn sich der Avatar gerade auf dieses Feld zubewegt: abprallen lassen

        bool AvatarOnField(POS pos);                    //Überprüft, ob der Avatar dieses Feld blockiert
        void killOnField(POS pos);                      //tötet den Avatar, wenn er die übergebene Position blockiert

        void run();                                     //Führt einen Simulationsschritt durch

        bool isMoving();                                //gibt zurück, ob sich der Avatar gerade bewegt

        bool isDead();                                  //Gibt zurück ob der Avatar tot ist, und die Todesanimation abgeschlossen ist (DeatchProgress>=100)

};


#endif // AVATAR_H_INCLUDED
