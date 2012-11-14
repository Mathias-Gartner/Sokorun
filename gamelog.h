#ifndef GAMELOG_H_INCLUDED
#define GAMELOG_H_INCLUDED



#define GAMELOGBOXTYPE 3
#define GAMELOGBORDERWITH 20
#define GAMELOGPADDING 5
#define GAMELOGMAINCOLOR COLOR{0,0.3,0}
#define GAMELOGNOTICESPACING 10

#define GameLogTitleFontSize 24
#define GameLogInfoFontSize 20


enum GameEventType                                  //Alle Arten von Spieleevents die es gibt
{   Gevt_NONE=-1,                                   //Gamelog-Intern (=type nicht zugewiesen/unbekannt)
    GAMESTART,
    USERAVATARMOVE,                                 //Der User hat den Avatar in eine Richtung gelenkt
    AVATARMOVE,                                     //Der Avatar wird in eine Richtung gelenkt ohne dass der User es gesagt hat
    AVATARDEATH,                                    //Der Avatar ist gestorben (2. Event (GAMEOVER) wird auch erstellt)

    KUGELMOVE,                                      //Eine Kugel (Typ 0) bewegt sich
    KUGELBLOCKMOVE,                                 //Ein Kugelblock (Typ 1) bewegt sich
    KUGELTARGET,                                    //Eine Kugel ist jetzt auf einem Zielfeld
    KUGELTYPECHANGED0,                              //Eine Kugel hat sich in den Typen 0 verwandelt
    KUGELTYPECHANGED1,                              //Eine Kugel hat sich in den Typen 1 verwandelt
    KUGELBLOCKEDLAVA,                               //Eine Kugel (typ 0) blockiert jetzt ein Lavafeld
    KUGELDESTROYED,                                 //Eine Kugel (beide Typen) wurde zerst�rt

    LOCKOPENED,                                     //Ein Schloss wurde mit einem Schl�ssel ge�ffnet

    GAMEOVER                                        //Das Spiel wurde verloren

};

struct GameEvent
{
    GameEventType type;                             //Was passiert ist
    unsigned long time;                             //Wann es passiert ist

    DIRECTION richtung;                             //F�r bewegungen

    GameEvent *next;                                //N�chstes Element
};


class GAMELOG
{
    private:
        ///Status:
            bool displayGameLog;                            //Ob der GameLog angezeigt wird (1=ja)

        ///Daten zum Spiel:
            unsigned long playtime;                     //Spielzeit: Anzahl der Schleifendurchl�ufe bis jetzt
            unsigned int events;                        //Anzahl der bisherigen Events

            GameEvent *start;                           //Alle Events die je aufgetreten sind

        ///Hilfsvariablen:

            //F�r die einzelnen Eventboxen:

            int x,y;                                    //F�r Positionsberechnungnen


       ///Zustandsvariablen f�r die Ausgabe:
            GameEventType type;                         //Typ der akt. Box
            int dispNum;                                //Die wievielte Box gerade ausgegeben wird
            char title[128];                            //Boxtitel der akt. Box
            COLOR farbe;                                //Boxfarbe der akt. Box
            unsigned int time;                          //Die letzte Zeit der akt. Box

            int icons;                                  //Anzahl der Icons in der akt. Box
            POS iconSet[7];                             //Spritepositionen der Icons, die ausgegeben werden sollen (in Umgekehrter Reihenfolge)


        ///BUTTONS:
            BUTTON show;                                //Einblende-Button
            BUTTON hide;                                //Ausblende-Button
            BUTTON save;                                //Save-Button


        ///Sonstiges:
            void setGameEventInformation();             //Gibt Titel und Farbe f�r ein GameEvent zur�ck
            void printGameEventInformation();           //Gibt eine Informationsbox aus
    public:
        GAMELOG();                                      //Konstruktor
        ~GAMELOG();                                     //Destruktor

        void addEvent(GameEventType,DIRECTION richtung);//Event hinzuf�gen




        void run();                                     //Muss bei jedem Schleifendurchlauf des Spieles aufgerufen werden
        void print();                                   //K�mmert sich um die Ausgabe

};





#endif // GAMELOG_H_INCLUDED
