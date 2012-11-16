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





        ///Einblende-Animation für spez. Elemente:      (Avatar, Kugeln, Schienen, Transporter, Schlösser,...)
            bool avatarAnimationCompleted,kugelAnimationCompleted;  //Ob die einzelnen Animationen, die parallel ausgeführt werden, fertig abgeschlossen sind

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

        //void lavafallAnimation(POS position);                                                 //Ezrueg eine Animation für ein in die Lava gefallenes Objekt
        void addFieldEffect(POS position,FIELDEFFECT effect,DIRECTION richtung,COLOR color);    //Ezreugt eine Animation


        void addGameLogEvent(GameEventType type,DIRECTION richtung);                            //Event hinzufügen

        POS getTargetFieldCoord(POS position,DIRECTION richtung);                   //Gibt die Koorinaten des Nachbarfeldes, abhängig von der Richtung, zurück bzw. das Feld des Zielbeamers

        void setupGameData();                                                       //Lädt die Leveldaten und Initialisiert alle Game-Daten (muss vor dem Spielen aufgerufen werden)
        void initBuildupAnimationSpecialElements();                                 //Animation für das Einblenden der Overlay-Elemente (zB. Avatar, Kugeln, Transporter+Schienen, Schlösser)

        //void deleteKugel(KUGEL *pointer);               //Löscht eine Kugel aus der Liste

        void printGameLogBackground();                  //Gibt nur den linken Rand und den Hintergrund des GameLog-Bereiches aus (wird von GAMELOG::print() auch erledigt)

        void printMovingObject(MOVEMENT *movement,POS position,int spriteNum);      //Gibt ein Objekt am Spielfeld aus, dass sich darauf bewegen kann
        void print();                                   //Ausgabe des Spielzustandes
        void run();                                     //Führt einen weiteren Simulationsschritt durch

        void move(DIRECTION richtung);                  //Der Avatar wird vom Spieler angestoßen

        void stopMovementsTo(POS pos,int limit);        //Objekte, die sich in dieses Feld bewegen wollen abprallen lassen, weil es ein anderes Objekt auf dieses Feld muss


        int isPrepared();                               //Gibt zurück, ob das Spiel erfolgreich vorbereitet und gespielt werden kann


        KUGEL* KugelOnField(POS position,KUGEL* ignore);//Überprüft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zurückgegeben
        LOCK* KeyOnField(POS position);                 //Überprüft, ob sich ein Schlüssel auf diesem Feld befindet. Wenn ja wird der Pointer auf dieses Lock-Objekt zurückgegeben

        bool AvatarOnField(POS position);               //Überprüft, ob der Avatar dieses Feld blockiert
        int isWalkable(OBJEKT object,POS position);     //Ob ein bestimmtes Feld von einem Objekt betreten werden darf, und wenn ja, wie weit. (Prozentwert 0-100. 100=vollständig betretbar.  -1=Fehler) (Es wird nur der Feldtyp geprüft, nicht ob sich andere Elemente wie zB. Kugeln darauf befinden)

        void openLock(LOCK* lockAdr);                   //Öffnet (=löscht) das Schloss

        bool isMoving(OBJEKT object);                   //Gibt zurück, ob sich ein solches Objekt gerade am Bildschirm bewegt

        void makeLavaSecure(POS position);              //Lavafeld mit Block befüllen (Dezimalwert des Spielfeldes ändern. Muss beim Neustart des Levels wieder Rückgängig gemacht werden)

        ~GAME();                                        //Destruktor (Speicherfreigabe usw.)

        void cleanupKugeln();                           //Alle Kugeln, die zum löschen markiert wurden jetzt löschen

};

#endif // GAMECLASS_H_INCLUDED
