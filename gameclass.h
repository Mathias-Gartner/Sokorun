#ifndef GAMECLASS_H_INCLUDED
#define GAMECLASS_H_INCLUDED

#include "animation.h"
#include "levelclass.h"
#include "gamelog.h"
#include "globals.h"

class GAMEBACKGROUND
{
    private:
        AREA area;              //Soll-Ausgabefl�che


        double alpha;

        double colorProgress;   //Zu wieviel % die neue Farbe g=nommen wird

        COLOR old;              //Letze Farbe
        COLOR final;            //Soll-Farbe

        COLOR splash;           //Kurzzeitige Soll-Farbe
        //bool splashEnabled;     //1: Die Splashfarbe=Zielfarbe
        int splashProgress;     //1-100% - zu wieviel % die Splash-Farbe eingeblendet werden soll; 0=keine Splahfarbe
        bool splashDirection;   //0: Die Splashfarbe wird eingeblendet; 1: Die Splashfarbe wird ausgeblendet

        //bool colorVariation;     //0: Ziel = target-Farbe    1: Ziel = abgedunkelte Target-Farbe


    public:
        GAMEBACKGROUND(AREA _area);

        void print(int bx,bool printshine=1);   //Ausgabe. bx = X-position, bis zu der der Hintergrund ausgegeben werden soll (gilt nicht f�r shine)
        void setColor(COLOR _target);           //Neue, endg�ltige Farbe w�hlen
        void setSplashColor(COLOR _splash);     //Farbe einstellen, die kurzfristig verwendet werden soll
        void reset();                           //Setzt den Hintergrund zurueck
};

class GAME : public LEVEL
{
    private:
        ///Allgemeine Zustandsdaten:
            int prepared;
            bool levelfieldAnimationsPrepared;  //Gibt an, ob die Animationen f�r animierte Levelfelder bereits im Animationshanlder eingetragen wurden
        ///Levelobjekte:
            class AVATAR *avatar;
            class KUGEL  *kugelStartPointer;                //Verkettete Klasse
            class LOCK   *lockStartPointer;                 //Verkettete Klasse
            class TRANSPORTER *transporterStartPointer;


        ///Hintergrund:
            GAMEBACKGROUND gamebackground;                  //Hintergrund bestehend aus Hintergrundbild + Leuchten


        ///Einblende-Animation f�r spez. Elemente:      (Avatar, Kugeln, Schienen, Transporter, Schl�sser,...)
            bool avatarAnimationCompleted,kugelAnimationCompleted;  //Ob die einzelnen Animationen, die parallel ausgef�hrt werden, fertig abgeschlossen sind

        ///GAME-LOGGER:
            class GAMELOG *gamelog;                               //Speichert alle Ereignisse ab und gibt diese am Bildschirm aus

        ///Sonstiges:
            /*private Prototypen*/
            int collision(POS position,DIRECTION richtung,COLOR color);             //Erzeugt eine Aufprall-Animation (R�ckgabewert: Animations-ID)
            void prepareLevelfieldAnimations();                                     //F�gt die Animationen f�r animierte Levelfelder (zB. Lava) in den Animationshandler ein
    public:
        GAME(POS origin,int elsize,const char *LVLpath,const bool skipMinorErrors=0):   LEVEL(origin,elsize,LVLpath,skipMinorErrors), //Konstruktor der Oberklasse "LEVEL"
                                                                                        gamebackground(AREA{{0,0},{windX-GameLogWidth,windY}})
        {   prepared=0;
            levelfieldAnimationsPrepared=0;                                         //Animationen f�r Levelfelder (zB. Lava) noch nicht erstellt
            avatar=NULL;
            kugelStartPointer=NULL;                                                 //Verkettete Kugel-Klasse beinhaltet keine Elemente
            lockStartPointer=NULL;                                                  //Verkettete Schloss-Klasse beinhaltet keine Elemente
            transporterStartPointer=NULL;

            avatarAnimationCompleted=0;
            kugelAnimationCompleted=0;
        }

        //void lavafallAnimation(POS position);                                     //Erzeugt eine Animation f�r ein in die Lava gefallenes Objekt
        int addFieldEffect(POS position,FIELDEFFECT effect,DIRECTION richtung=NONE,COLOR color=WHITE,double progress=0);    //Erzeugt eine Animation (R�ckgabewert: Animations-ID)


        void addGameLogEvent(GameEventType type,DIRECTION richtung);                            //Event hinzuf�gen

        POS getTargetFieldCoord(POS position,DIRECTION richtung);                   //Gibt die Koorinaten des Nachbarfeldes, abh�ngig von der Richtung, zur�ck bzw. das Feld des Zielbeamers
        bool getLevelfieldAnimationsPrepared();                                     //Abfragen, ob die Animierten Levelfelder bereits erstellt wurden

        void setupGameData();                                                       //L�dt die Leveldaten und Initialisiert alle Game-Daten (muss vor dem Spielen aufgerufen werden)
        void initBuildupAnimationSpecialElements();                                 //Animation f�r das Einblenden der Overlay-Elemente (zB. Avatar, Kugeln, Transporter+Schienen, Schl�sser)

        //void deleteKugel(KUGEL *pointer);               //L�scht eine Kugel aus der Liste

        void printGameLogBackground();                  //Gibt nur den linken Rand und den Hintergrund des GameLog-Bereiches aus (wird von GAMELOG::print() auch erledigt)
        void EnablePauseButton(bool enable);            //Ativiert/Deaktiviert den Pause-Button im Gamelog


        void printMovingObject(MOVEMENT *movement,POS position,int spriteNum,POS beamTarget=POS{-1,-1});      //Gibt ein Objekt am Spielfeld aus, dass sich darauf bewegen kann
        void print();                                   //Ausgabe des Spielzustandes
        int run();                                      //F�hrt einen weiteren Simulationsschritt durch; R�ckgabewert: Spielstatus (-1: Gameover; >=0: Anzahl der Noch freien Zielfelder)

        void move(DIRECTION richtung);                  //Der Avatar wird vom Spieler angesto�en

        void stopMovementsTo(POS pos,int limit);        //Objekte, die sich in dieses Feld bewegen wollen abprallen lassen, weil es ein anderes Objekt auf dieses Feld muss
        void killObjectsOnField(POS pos);               //Zerst�rt alle Kugeln und den Avatar, wenn diese dieses Feld blockieren (wird zB. duch den t�dlichen Transporter ausgel�st)

        void printPreview();                            //Vorschau anzeigen (inkl. Levelhintergrund)
        bool runBuildupAnimation();                     //Level aufbauen (inkl. Levelhintergrund)
        void printFloor(bool printAnimatedFields=0);    //Spielfl�che ausgeben (inkl. Levelhintergrund)
        void setGameBackgroundSplashColor(COLOR splash);//kurzfristige Farbe setzen

        void setGameBackgroundColor(COLOR target);      //Setzt die Farbe des Leuchtens im Spielhintergrund

        int isPrepared();                               //Gibt zur�ck, ob das Spiel erfolgreich vorbereitet und gespielt werden kann


        KUGEL* KugelOnField(POS position,KUGEL* ignore);//�berpr�ft, ob sich eine Kugel auf diesem Feld befindet (zu mind. OccupiedLimit %). Wenn ja wird der Pointer auf diese Kugel zur�ckgegeben
        LOCK* KeyOnField(POS position);                 //�berpr�ft, ob sich ein Schl�ssel auf diesem Feld befindet. Wenn ja wird der Pointer auf dieses Lock-Objekt zur�ckgegeben

        bool AvatarOnField(POS position);               //�berpr�ft, ob der Avatar dieses Feld blockiert
        int isWalkable(OBJEKT object,POS position);     //Ob ein bestimmtes Feld von einem Objekt betreten werden darf, und wenn ja, wie weit. (Prozentwert 0-100. 100=vollst�ndig betretbar.  -1=Fehler) (Es wird nur der Feldtyp gepr�ft, nicht ob sich andere Elemente wie zB. Kugeln darauf befinden)
        bool isDriveable(RAIL *r,TRANSPORTER *ignore,bool loaded);          //Gibt zur�ck ob sich ein Transporter auf dieses Feld bewegen darf
        bool isLocked(POS position);                    //�berpr�ft, ob ein bestimmtes Feld von einem Schloss versperrrt wird

        void openLock(LOCK* lockAdr);                   //�ffnet (=l�scht) das Schloss

        bool isMoving(OBJEKT object);                   //Gibt zur�ck, ob sich ein solches Objekt gerade am Bildschirm bewegt

        void makeLavaSecure(POS position);              //Lavafeld mit Block bef�llen (Dezimalwert des Spielfeldes �ndern. Muss beim Neustart des Levels wieder R�ckg�ngig gemacht werden)


        bool isPauseButtonClicked();                    //Gibt zur�ck, ob der Pause-Button seit dem letzten Aufruf gedr�ckt worden ist (im Gamelog)

        ~GAME();                                        //Destruktor (Speicherfreigabe usw.)
        void clearGameData();                           //L�scht alle Spieledaten, damit diese sp�ter wieder neu geladen werden k�nnen

        void cleanupKugeln();                           //Alle Kugeln, die zum l�schen markiert wurden jetzt l�schen

        //Access to events for score and replay
        GAMELOG* getGameLog() { return gamelog; };
};


#endif // GAMECLASS_H_INCLUDED
