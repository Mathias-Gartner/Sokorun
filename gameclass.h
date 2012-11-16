#ifndef GAMECLASS_H_INCLUDED
#define GAMECLASS_H_INCLUDED



class GAME : public LEVEL
{
    private:
        ///Allgemeine Zustandsdaten:
            int prepared;
        ///Levelobjekte:
            class AVATAR *avatar;
            class KUGEL  *kugelStartPointer;                //Verkettete Klasse
            class LOCK   *lockStartPointer;                 //Verkettete Klasse





        ///Einblende-Animation f�r spez. Elemente:      (Avatar, Kugeln, Schienen, Transporter, Schl�sser,...)
            bool avatarAnimationCompleted,kugelAnimationCompleted;  //Ob die einzelnen Animationen, die parallel ausgef�hrt werden, fertig abgeschlossen sind

        ///GAME-LOGGER:
            class GAMELOG *gamelog;                               //Speichert alle Ereignisse ab und gibt diese am Bildschirm aus

        ///Sonstiges:
            /*private Prototypen*/
            void collision(POS position,DIRECTION richtung,COLOR color);                        //Erzeugt eine Aufprall-Animation

    public:
        GAME(POS origin,int elsize,const char *LVLpath,const bool skipMinorErrors=0):LEVEL(origin,elsize,LVLpath,skipMinorErrors)//Konstruktor der Oberklasse "LEVEL"
        {   prepared=0;
            avatar=NULL;
            kugelStartPointer=NULL;                                                 //Verkettete Kugel-Klasse beinhaltet keine Elemente
            lockStartPointer=NULL;                                                  //Verkettete Schloss-Klasse beinhaltet keine Elemente

            avatarAnimationCompleted=0;
            kugelAnimationCompleted=0;
        }

        //void lavafallAnimation(POS position);                                                 //Ezrueg eine Animation f�r ein in die Lava gefallenes Objekt
        void addFieldEffect(POS position,FIELDEFFECT effect,DIRECTION richtung,COLOR color);    //Ezreugt eine Animation


        void addGameLogEvent(GameEventType type,DIRECTION richtung);                            //Event hinzuf�gen

        POS getTargetFieldCoord(POS position,DIRECTION richtung);                   //Gibt die Koorinaten des Nachbarfeldes, abh�ngig von der Richtung, zur�ck bzw. das Feld des Zielbeamers

        void setupGameData();                                                       //L�dt die Leveldaten und Initialisiert alle Game-Daten (muss vor dem Spielen aufgerufen werden)
        void initBuildupAnimationSpecialElements();                                 //Animation f�r das Einblenden der Overlay-Elemente (zB. Avatar, Kugeln, Transporter+Schienen, Schl�sser)

        //void deleteKugel(KUGEL *pointer);               //L�scht eine Kugel aus der Liste

        void printGameLogBackground();                  //Gibt nur den linken Rand und den Hintergrund des GameLog-Bereiches aus (wird von GAMELOG::print() auch erledigt)

        void printMovingObject(MOVEMENT *movement,POS position,int spriteNum);      //Gibt ein Objekt am Spielfeld aus, dass sich darauf bewegen kann
        void print();                                   //Ausgabe des Spielzustandes
        void run();                                     //F�hrt einen weiteren Simulationsschritt durch

        void move(DIRECTION richtung);                  //Der Avatar wird vom Spieler angesto�en

        void stopMovementsTo(POS pos,int limit);        //Objekte, die sich in dieses Feld bewegen wollen abprallen lassen, weil es ein anderes Objekt auf dieses Feld muss


        int isPrepared();                               //Gibt zur�ck, ob das Spiel erfolgreich vorbereitet und gespielt werden kann


        KUGEL* KugelOnField(POS position,KUGEL* ignore);//�berpr�ft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zur�ckgegeben
        LOCK* KeyOnField(POS position);                 //�berpr�ft, ob sich ein Schl�ssel auf diesem Feld befindet. Wenn ja wird der Pointer auf dieses Lock-Objekt zur�ckgegeben

        bool AvatarOnField(POS position);               //�berpr�ft, ob der Avatar dieses Feld blockiert
        int isWalkable(OBJEKT object,POS position);     //Ob ein bestimmtes Feld von einem Objekt betreten werden darf, und wenn ja, wie weit. (Prozentwert 0-100. 100=vollst�ndig betretbar.  -1=Fehler) (Es wird nur der Feldtyp gepr�ft, nicht ob sich andere Elemente wie zB. Kugeln darauf befinden)

        void openLock(LOCK* lockAdr);                   //�ffnet (=l�scht) das Schloss

        bool isMoving(OBJEKT object);                   //Gibt zur�ck, ob sich ein solches Objekt gerade am Bildschirm bewegt

        void makeLavaSecure(POS position);              //Lavafeld mit Block bef�llen (Dezimalwert des Spielfeldes �ndern. Muss beim Neustart des Levels wieder R�ckg�ngig gemacht werden)

        ~GAME();                                        //Destruktor (Speicherfreigabe usw.)

        void cleanupKugeln();                           //Alle Kugeln, die zum l�schen markiert wurden jetzt l�schen

};

#endif // GAMECLASS_H_INCLUDED
