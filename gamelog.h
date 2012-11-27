#ifndef GAMELOG_H_INCLUDED
#define GAMELOG_H_INCLUDED

#include "button.h"
#include "definitions.h"


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
    KUGELDESTROYED,                                 //Eine Kugel (beide Typen) wurde zerstört

    LOCKOPENED,                                     //Ein Schloss wurde mit einem Schlüssel geöffnet

    GAMEOVER                                        //Das Spiel wurde verloren

};

struct GameEvent
{
    GameEventType type;                             //Was passiert ist



    char size;                                      //Wieviele Daten in deisem Event gespeichet sind (1-GAMELOG_ICONS_PER_BOX)
    unsigned long time[GAMELOG_ICONS_PER_BOX];                          //Wann es passiert ist
    DIRECTION richtung[GAMELOG_ICONS_PER_BOX];                          //Für bewegungen

    GameEvent *next;                                //Nächstes Element
};


class GAMELOG
{
    private:
        ///Status:
            bool displayGameLog;                            //Ob der GameLog angezeigt wird (1=ja)

        ///Daten zum Spiel:
            unsigned long playtime;                     //Spielzeit: Anzahl der Schleifendurchläufe bis jetzt
            unsigned int events;                        //Anzahl der bisherigen Events

            GameEvent *start;                           //Alle Events die je aufgetreten sind (erstes Element=neuestes Element)

        ///Hilfsvariablen:
            int x,y;                                    //Für Positionsberechnungnen


       ///Zustandsvariablen für die Ausgabe:
            int dispNum;                                //Die wievielte Box gerade ausgegeben wird

            //Für das nach-unten-schieben bei einem neuen Event:

            int progress;                               //Scoll-Fortschritt ind % (0-100)


        ///BUTTONS:
            BUTTON show;                                //Einblende-Button
            BUTTON hide;                                //Ausblende-Button
            BUTTON save;                                //Save-Button


        ///Sonstiges:
            void getGameEventInformation(int type,char *title,COLOR *farbe);                                 //Gibt Titel und Farbe für ein GameEvent zurück
            //void printGameEventInformation();           //Gibt eine Informationsbox aus
    public:
        GAMELOG();                                      //Konstruktor
        ~GAMELOG();                                     //Destruktor

        void addEvent(GameEventType,DIRECTION richtung);//Event hinzufügen


        void printBackground();                         //Gibt nur den Linken Rand und den Hintergrund aus (wird von GAMELOG::print() auch erledigt)
        int getxpos();                                  //Gibt die X-Position zurück, ab der sich der Gamelog befindet

        void run();                                     //Muss bei jedem Schleifendurchlauf des Spieles aufgerufen werden
        void print();                                   //Kümmert sich um die Ausgabe

};


void getTimeString(char *string,int time);              //Wandelt eine Zeitangabe (ms) in eine String um


#endif // GAMELOG_H_INCLUDED
