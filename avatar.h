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
            int *elsize;                //Gr��e eines Elements (Feldes)
            POS *levelSize;             //Levelgr��e

        ///Daten:
            int deathProgress;          //Wenn der Avatar Tot ist: Todes-Animation (1-100%)
                //0: Der Avatar ist noch nicht Tot

            POS position;
            MOVEMENT movement;          //Bewegung des Avatars
            bool userinput;             //1: Der Avatar bewegt sich, weil der User das gewollt hat (Wird nur abgespeichert, damit der Gamelog nach dem erreichen des Zielfelds wei�, warum sich der Avatar bewegt hat. NICHT F�R DIE PHYSIK notwendig)
                                        //Befindet sich nicht der Struktur "movement", weil diese Variable nur f�r den Avatar wichtig ist

        ///Avatar-Einblende-Animation:
            int buildupCounter;         //Fortschritts-z�hler
            int buildupImage;

        ///Sonstiges:

    public:
        AVATAR(GAME *gamePointer,POS *originPointer,int *elsizePointer,POS *levelSizePointer,POS avatarOrigin);

        //bool setupAnimation();                          //langsames Einblenden


        void print();

        void move(DIRECTION richtung,bool userinput);   //Der Avatar wird angesto�en (userinput=1: Benutzereigabe -nur wenn sich sonst nichts bewegt)
        void stopMovementTo(POS pos,int limit);         //Wenn sich der Avatar gerade auf dieses Feld zubewegt: abprallen lassen

        bool AvatarOnField(POS pos);                    //�berpr�ft, ob der Avatar dieses Feld blockiert

        void run();                                     //F�hrt einen Simulationsschritt durch

        bool isMoving();                                //gibt zur�ck, ob sich der Avatar gerade bewegt

};


#endif // AVATAR_H_INCLUDED
