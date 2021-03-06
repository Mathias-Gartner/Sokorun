#ifndef GAMELOG_H_INCLUDED
#define GAMELOG_H_INCLUDED

#include "button.h"
#include "definitions.h"
#include "globals.h"


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



    char size;                                      //Wieviele Daten in deisem Event gespeichet sind (1-GAMELOG_ICONS_PER_BOX)
    unsigned long time[GAMELOG_ICONS_PER_BOX];      //Wann es passiert ist
    DIRECTION richtung[GAMELOG_ICONS_PER_BOX];      //F�r bewegungen

    GameEvent *next;                                //N�chstes Element
};


class GAMELOG
{
    private:
        ///Status:
            bool displayGameLog;                        //Ob der GameLog angezeigt wird (1=ja)

        ///Daten zum Spiel:
            //unsigned int  minorPlaytime;                //Spielzeit: Anzahl der Frames bis jetzt, von 0-999; bei 1000: playtime++
            //unsigned long playtime;                     //Spielzeit: Anzahl von 1000 Frames bis jetzt
            unsigned long startTime;                    //Zeitpunkt des des Spielstarts (1. Frame)

            unsigned int useravatarmoves;
            unsigned int events;                        //Anzahl der bisherigen Events

            GameEvent *start;                           //Alle Events die je aufgetreten sind (erstes Element=neuestes Element)

        ///Hilfsvariablen:
            int x,y;                                    //F�r Positionsberechnungnen


       ///Zustandsvariablen f�r die Ausgabe:
            int dispNum;                                //Die wievielte Box gerade ausgegeben wird

            //F�r das nach-unten-schieben bei einem neuen Event:

            double progress;                            //Scoll-Fortschritt ind % (0-100)


        ///BUTTONS:
            BUTTON show;                                //Einblende-Button
            BUTTON hide;                                //Ausblende-Button
            BUTTON save;                                //Save-Button
            BUTTON pause;                               //Pause-Button

            bool pauseButtonEnable;                     //Ob der Pause-Button ausgegeben werden soll oder nicht (und ob er gedr�ckt werden darf)
            bool pauseButtonClicked;                    //Zeigt an, ob der User den Pause-Button geklickt hat (seit dem letzten "getPauseButtonClicked()"-Aufruf)


        ///Sonstiges:
            void getGameEventInformation(int type,char *title,COLOR *farbe);                                 //Gibt Titel und Farbe f�r ein GameEvent zur�ck
            //void printGameEventInformation();           //Gibt eine Informationsbox aus
    public:
        GAMELOG();                                      //Konstruktor
        ~GAMELOG();                                     //Destruktor

        void addEvent(GameEventType,DIRECTION richtung);//Event hinzuf�gen


        void printBackground();                         //Gibt nur den Linken Rand und den Hintergrund aus (wird von GAMELOG::print() auch erledigt)
        int getxpos();                                  //Gibt die X-Position zur�ck, ab der sich der Gamelog befindet

        void run();                                     //Muss bei jedem Schleifendurchlauf des Spieles aufgerufen werden
        void print();                                   //K�mmert sich um die Ausgabe

        bool isPauseButtonClicked();                    //Gibt zur�ck, ob der Pause-Button seit dem letzten Aufruf gedr�ckt worden ist
        void EnablePauseButton(bool enable);            //Ativiert/Deaktiviert den Pause-Button

        //property getter
        GameEvent* getEventStart() { return start; }
        unsigned int getEventCount() { return events; }
        unsigned long getPlayTime();
        unsigned int getUserAvatarMoves() { return useravatarmoves; }
};


void getTimeString(char *string,int time);              //Wandelt eine Zeitangabe (ms) in eine String um


#endif // GAMELOG_H_INCLUDED
