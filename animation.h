#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED
//

#include "graphics.h"

enum ANITYPE
{
    BASIC,
    LEVELFIELD,
    LEVELEFFECT,
    OBJECTBUILDUP,      //Beim Aufbauen des Levels: Einblendeanimationen von Objekten
    MOVEMENTINFO        //Im Spiel: drehendes Element, dass anzeigt dass sich der Avatar und/oder eine Kugel bewegen
};

//Animatiostypen im Spiel:
    enum FIELDEFFECT            //Animationen, die auf dem Spielfeld ausgegeben werden k�nnen
    {       COLLISION,          //Ein Objekt prallt ab
            LAVAFALL,           //Eine Kugel (Typ 1) f�llt in die Lava
            AVATARLAVA,         //Der Avatar f�llt in die Lava
            KUGELLAVA,          //Eine Kugel (Typ 0) f�llt in die Lava
            AVATARKILL,         //Der Avatar wird get�tet
            KUGELKILL,          //Eine Kugel (Typ 0) wird zerst�rt
            KUGELBLOCKKILL,     //Eine Kugel (Typ 1) wird zerst�rt
            AVATARBUILDUP,      //Der Avatar wird eingeblendet
            KUGELBUILDUP,       //Eine Kugel (Typ 0) wird eingeblendet
            KUGELBLOCKBUILDUP   //Eine Kugel (Typ 1) wird eingeblendet
    };

class ANIMATION
{   private:
            ANITYPE anitype;                //Animationstyp zum identifizieren
            int identification;             //eindeutige ID in einer ANIMATIONGROUP

            ANIMATION *next;                //F�r eine Animationsgruppe (Klasse ANIMATIONGROUP) --> Verkettete Klasse (wird innherlab dieser Klasse nicht verwendet, nur f�r den Zugriff von au�erhalb)

        ///Allgemeine Einstellungen:
            int type;                           //Welche Art von Animation verwendet wird
                //-1: keine Animation (zB. weg. Fehlerhafter Daten)
                //0: In einer Farbe langsam einf�rben
                //1: Drehen
                //2: Textur-Folge
                //3: Langsam ein- bzw. ausblenden (Alpha-Wert wird ver�ndert). Ansonsten gleich wie type=0

            bool richtung;                      //0: normal; 1: r�ckw�rts

            int start,end;                      //Anfangswert und Endwert der Animation
                //type=0:   Anfangs- und Endabdunklung  (0-100%)
                //type=1:   Anfangswinkel und Endwinkel (0-100%)
                //type=2:   erste und letzte Bildnummer (0-x)
                //type=3:   Anfangs- und End-Alphawert  (0-100%)

            int reverse;                        //Was nach Abschluss der Animation passieren soll
                //0: Nach Ende der Simulation soll diese gel�scht werden (wird nicht mehr ausgegeben)
                //1: Richtung �ndern (Variable "richtung")
                //2: Nein - vom Anfang wiederholen
                //3: Nach Ende der Simulation wird sie gestoppt und das letzte Bild immer wieder ausgegeben

            float speed;                        //Animationsgeschwindigkeit (% pro Simulationsschritt)
                //0: manuelle Erh�hung

        ///Ausgabeeinstellungen:
            TEXTURE *texture;                   //Welche Textur f�r die Animation verwendet wird
            POS spritePos;                      //Welches Sprite dieser Textur verwendet wird
                    //type=2: Wert wird ignoriert

            COLOR overlay;
                //type=0:   Farbe in der die Animation eingef�rbt wird
                //rest:     Overlay-Farbe

            AREA output;                        //Gebiet f�r die Ausgabe - dient auch zum spiegeln eines Bildes
                //type=1 (drehen):  Nur Quadratische Bilder m�glich
                //                  --> a =     Position (Bildmittelpunkt)
                //                      b.x =   size (Bildgr���)

        ///Statusinformationen:
            float progress;                     //0-100%
            bool finished;                      //Wenn 1: Die Animation ist zu Ende (reverse==0: Keine Aushabe mehr erlaubt; reverse==3: keine Simulation mehhr erlaubt, Ausgabe schon)


        ///Prototypen:
            void print_T0();                    //Ausgabefunktion f�r Typ 0
            void print_T1();                    //Ausgabefunktion f�r Typ 1
            void print_T2();                    //Ausgabefunktion f�r Typ 2
            void print_T3();                    //Ausgabefunktion f�r Typ 3

    public:
        ANIMATION(ANITYPE _anitype,int _identification,int _type,bool _richtung,int _start,int _end,int _reverse,float _speed,TEXTURE *_texture,POS _spritePos,AREA _output,COLOR _overlay=WHITE);//Erstellung einer neuen Animation

        void setProgress(int newProgress);                  //Fortschritt manuell �ndern
        void setSpritePos(POS _spritePos);                  //Sprite �ndern

        //void setSpriteArea(fAREA _spriteArea);              //spriteArea setzen

        int getIdentification();                            //Gibt die Identifikationsnummer zur�ck
        ANITYPE getAnitype();                               //Gibt den Animationstypen zur�ck
        int getType();                                      //Gibt den Animationstyp zur�ck
        int getReverse();                                   //Gibt den Reverse-Wert zur�ck
        bool isFinished();                                  //Gibt zur�ck, ob die Animation bereits abgeschlossen wurde (1=ja)

        void setNextPointer(ANIMATION* _next);              //Pointer setzen
        ANIMATION* getNextPointer();                        //Gibt Pointer zur�ck

        void print();                                       //Ausgabe
        bool run();                                         //Simulationsschritt durchf�hren. R�ckgabewert=1: Simulation in eine Richtung abgeschlossen, beim n�chsten Durchgang wiederholt sich alles

};


class ANIMATIONGROUP                                        //Verwaltet mehrere Animationen
{
    private:
        ANIMATION *start;                                   //Verkettete Liste
        int IDcounter;
    public:
        ANIMATIONGROUP();                                   //Konstruktor
        int add(ANITYPE _anitype,int _type,bool _richtung,int _start,int _end,int _reverse,float _speed,TEXTURE *_texture,POS _spritePos,AREA _output,COLOR _overlay=WHITE);//Hinzuf�gen einer neuen Animation
        //void setSpriteArea(int identification,fAREA _spriteArea);   //spriteArea setzen (zB. zum spiegeln)
        void setSpritePos(int identification,POS _spritePos);        //Sprite �ndern

        //int getAnimationAnz(ANITYPE group);                 //Gibt die Anzahl der Animationen mit diesem Typ zur�ck
        int getActiveAnimationAnz(ANITYPE group);           //Gibt die Anzahl der Animationen mit diesem Typ zur�ck, welche noch nicht abgeschlossen sind

        void print(int identification);                     //Gibt alle Animationen einer bestimmten ID aus (0=ALLE)
        bool run(int identification);                       //Simuliert alle Animationen einer bestimmten ID (0=ALLE) (gibt zur�ck ob ein Element fertig ist und gel�scht wurde)

        void print(ANITYPE group);                          //Gibt alle Animationen einer bestimmten Gruppe aus
        bool run(ANITYPE group);                            //Simuliert alle Animationen einer bestimmten Gruppe (gibt zur�ck ob ein Element fertig ist und gel�scht wurde)

        void remove(ANITYPE group);                         //L�scht alle Animationen einer bestimmten Gruppe aus der Liste (0=Alle l�schen)
        void remove(int identification);                    //L�scht eine bestimmte Animation

        ~ANIMATIONGROUP();                                  //Destruktor - L�scht alle Daten
};

#endif // ANIMATION_H_INCLUDED
