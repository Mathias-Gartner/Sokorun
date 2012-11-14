#ifndef LEVELCLASS_H_INCLUDED
#define LEVELCLASS_H_INCLUDED


struct KUGELorigin      //Verkettete Liste mit allen Kugel-Daten die aus der Level-Datei geladen werden
{   POS origin;
    int type;           //Kugeltyp
    KUGELorigin *next;
};


struct LOCKorigin       //Verkettete Liste mit allen Schloss-Daten die aus der Level-Datei geladen werden
{   POS lock;
    POS key;
    COLOR color;
    LOCKorigin *next;
};


///SPIELFELDWERTE: VERALTET
/*
     0,         Leeres Feld (selbe physikalische Eigenschaften wie 25 (Wand)
     1,         Freies Feld (kann von jedem Betreten werden)
     2,         Kugel kann nicht auf dieses Feld geschoben werden
     3,         Avatar kann dieses Feld nicht betreten
     4- 7,      Kugel: wird (wenn mögl.) weitergeschoben; Für Avatar wie 1 (freies Feld)
     8-11,      Kugel: wie 4-7 (wird weiter geschoben; Avatar: wie 3 (nicht betretbar)
    12-15,      Avatar: wird (wenn mögl.) weitergeschoben;  Kugel: wie 1 (freies Feld)
    16-19,      Avatar: wie 12-15 (wird weitergeschoben); Kugel: wie 2 (kann nicht auf dieses Feld geschobenw erden)
    20-23,      Avatar und Kugeln werden weitergeschoben
    24,         Start-Beamer
    25,         Wand (kann von Avatar und Kugel nicht betreten werden
    26,         Eis (Avatar und Kugeln rutschein (wenn mögl.) in die selbe Richtung weiter, wie sie gekommen sind
    27,         Zielfeld (Hier muss eine Kugel hingeschoben werden. Die Kugel kann auch wieder herausgeschoben werden)
    28*,        Kugel
    29*,        Avatar
    30*,        Transporter
    31**,       Feld, dass bei der buildupAnimation verwendet wird (Beim einblenden eines Levels)
    32,
    33,
    34,         Lava (Avatar und Kugeln werden zuerstört)
    35,         Zielbeamer (darf in jedem Level nur einmal vorkommen; Wenn der Avatar oder eine Kugel das Feld 24 (Start-Beamer) betritt, wird er hierher gesetzt
    36*,        Tödlicher Transporter
    37,         fixes Zielfeld (Hier muss eine Kugel hingeschoben werden. Die Kugel kann nicht mehr herausgeschoben werden)
    38,
    39,
    40-42**,    Animation für das Generieren eines Geschosses (Kanonenkugel)
    43*,        Geschoss (Kanonenkugel)
    44-46**,    Animation für das Generieren eines Geschosses (Kanonenkugel)
    47*,        Geschoss (Kanonenkugel)
    48-50**,    Animation für das Generieren eines Geschosses (Kanonenkugel)
    51*,        Geschoss (Kanonenkugel)
    52-54**,    Animation für das Generieren eines Geschosses (Kanonenkugel)
    55*,        Geschoss (Kanonenkugel)
    56-58**,    Animation für das Generieren eines Geschosses (Kanonenkugel)
    59*,        Geschoss (Kanonenkugel)
    60-63**,    Animation, wenn eine Kugel ins Feld 34 (Lava) fährt und zerstört wird
    64-67**,    Animation, wenn der Avatar ins Feld 34 (Lava) färht und zerstört wird
    68-71**,    Animation, wenn die Kugel von einem Geschoss (43,47,51,59) getroffen wird und zerstört wird
    72-75**,    Animation, wenn der Avatar von einem Geschoss (43,47,51,59) getroffen wird und zerstört wird
    81-83,      Kanone, die Geschosse abfeuert
    84-86***,     Schienen für den Tödlichen Transporter
    87-89*,     Schienen für den Transporter

    *)      Kann kein Spielfeldwert sein, sondern wird auf einem Spielfeld platziert und kann die Position verändern
    **)     Darf nicht als Spielfeldwert verwendet werden, dient nur als Bilddatei für Effekte
    ***)    Kann kein Spielfeldwert sein, sondern wird auf einem Spielfeld platziert und kann die Position NICHT verändern
*/


class LEVEL{
    protected:
        ///Allgemeine Hilfsvariablen:
            POS pos;

        ///Allgemeine Zustandsdaten:
            int status;                         //Status
                //0:    Keine gültigen Daten geladen
                //1:    gültige Leveldaten geladen

            POS origin;                         //Urpsrungs-Koordinate, an denen das Level ausgegebenw wird (links-unten)
            int elsize;                         //Größe eines Feldes bei der Ausgabe


   //GLuint gamefloor;
   //bool gamefloorPrepared;


        ///Level-Metadaten:
            char path[256];                     //Pfad der Leveldatei (falls das Level aus dem Editor kommt: kein Pfad)
            char CreateDate[256];               //Erstelldatum der Leveldatei
            char CreateTime[256];               //Erstellzeitpunkt der Leveldatei


        ///Leveldaten:
            POS size;                           //Größe des Spielfeldes


            char spielfeld[MaxYsize][MaxXsize]; //Spielfeld
            POS avatarOrigin;                   //Startposition der Spielfigur
            KUGELorigin *kugelOriginStart;      //Start der Verketteten Liste mit allen Kugel-Startpositionen
            LOCKorigin  *lockOriginStart;       //Start der Verketteten Liste mit allen Schloss-Daten


        ///Level-Einblende-Animation:
            int animationType;                  //Welche Animation verwendet wird
            unsigned char variation;            //Zum variieren der Variationen
            char* visible;

            int buildupAnimationProgress;       //Fortschritts-zähler
            POS buildupAnimationOrigin;         //Ursprungsposition von wo aus die Animation erfolgt
            bool buildupAnimationCompleted;     //Zur Überprüfung, ob die Animation fertig ist

        ///Sonstiges:
            bool loadLevel(const char *LVLpath,const bool skipMinorErrors);//Lädt ein Level aus einer Datei
            int convertLevel();                 //Konvertiert die Daten eines veralteten Levelformates

            void printFloorElement(int element,POS coord,COLOR color);  //Ein einzelnes Feld der Spielfläche ausgeben
    public:
        LEVEL(POS origin,int elsize,const char *LVLpath,const bool skipMinorErrors);

        char* getLevelPath();                               //Gibt den Pfad der Leveldatei zurück, die geladen wurde
        void getMetaData(char **dateP,char **timeP);        //Gibt Metadaten wie Erstelldatum und Erstellzeitpunkt zurück
        POS getSize();                                      //Gibt die Spielfeldgröße zurück

        void printPreview();                                //Ausgabe des gesametem Levels
        void printFloor();                                  //Spielfläche ausgeben
        void printKugelnAtOrigins();                        //Kugeln an den Startpositionen ausgeben
        void printAvatarAtOrigin();                         //Speilfigur an der Startposition ausgeben
        void printLocksAtOrigins();                         //Schlösser und Schlüssel an deren Position ausgeben
        void marker(POS position,COLOR color);              //Umrahmt ein bestimmtes Feld (Für debugging-Zwecke)

        bool runBuildupAnimation();                         //Animation für das Einblenden des Levels

        void setDisplayOptions(POS _origin,int _elsize);    //Darstellungsoptionen ändern
        int getStatus();                                    //Statusinformationen erhalten

        int getField(POS position);                         //Gibt den Wert des Spielfeldes zurück

        bool checkPos(const POS position);                  //Überprüft, ob sich die aktuelle Position auf dem Spielfeld befindet (0=ja) (position muss >=0 und <size sein)

        POS getTargetBeamer();                              //Gibt die Position des Zielbeamers zurück

        FIELDPROPERTY getFieldProperty(OBJEKT object,POS position);       //Gibt die Eigenschaften eines Feldes für ein bestimmtes Objekt zurück
        ~LEVEL();                                           //Destruktor - Speicher wieder freigeben

};
#endif // LEVEL_H_INCLUDED
