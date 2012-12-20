#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

    #define DEBUG 1                 //OB DEBUG-BEFEHLE DURCHGEFÜHRT WERDEN SOLLEN
    #define ImgDebug 0              //Ob Debug-Funktionen, die im Grafik-Fenster ausgegeben werden angewandt werden sollen
    #define TIMEDEBUGOUTPUT 0       //Ob die Zeit/Spielschleifenruchlauf ausgegeben werden soll

/** SPIELGESCHWINDIGKEIT **/
    #define frameArraySize 100           //Größe des Ringbuffers. Gibt an, aus wievielen Werten der Durchschnitt berechnet wird

    #define AVATAR_SPEED (400.0f/FPS)           //Geschwindigkeit des Avatars und berechnungsgrundlage aller anderen Werte. Gibt den Fortschritt an, der pro Sekunde zurückgelegt wird (zB. 200% --> 2Felder/sekunde)

    #define KUGEL_SPEED AVATAR_SPEED


    #define GAME_BG_SHINE_COLOR_SPEED   (AVATAR_SPEED*0.75)         //Geschwindigkeit beim Farbwechsels beim Shine im Spielhintergrund
    #define GAME_BG_SHITE_ALPHA_SPEED   (AVATAR_SPEED/1300.0f)      //Geschwindigkeit beim Transparenz erhöhen beim einblenden des Shines zum Spielbeginn
    #define GAMELOG_SCROLL_SPEED    (AVATAR_SPEED*2.25f)            //Scroll-Geschwindigkeit für die Event-Boxen beim Gamelog

    #define GAMEEND_SPEED (AVATAR_SPEED*0.1)  //Geschwindigkeit/Dauer nach dem Ende des Spieles

/** ANIMATIONSGESCHWINDIGKEITEN **/     //Müssen unabhängig von AVATAR_SPEED und FPS definiert werden, da die abhängigkeit der FPS im Porigrammcode selbst erzeugt wird ("animation.cpp", "ANIMATION::run()")
    #define COLISSIONANI_SPEED (200.0f)         //Geschwindigkeit der Kolissionsanimation
    #define MOVEMENTINFO_SPEED (27.0f)          //Geschwindigkeit des sich drehenen Movement-Infos
    #define LAVAFLOW_SPEED     (55.0f)          //Geschwindigkeit der Lavabewegung
    #define LAVABLUBB_SPEED    (100.0f)         //Geschwindigkeit einer Lavablase


    #define BUILDUP_SPCLELEM_SPEED (133.0f)     //Die Einblende geschwindigkeit von Spezialelementen (Avatar, Kugeln, Schienen, Schlösser, Transporter,...)
    #define LAVAFALL_ANI_SPEED (333.0f)         //Geschwindigkeit der Animation, wenn eine Blockkugel in die Lava fällt
    #define LAVA_KILLANI_SPEED (133.0f)         //Geschindigkeit, wenn ein Element durch Lava zerstört wird
    #define KILLANI_SPEED (133.0f)              //Geschindigkeit, wenn ein Element zerstört wird (zB. durch tödl. Transporter)

    #define LEVELBUILDUP_ANI_SPEED (1.3f)       //Geschwindigkeit der Level-Buildup-Animation

/** EINSTELLUNGEN **/
    #define AutoAdjustWindowSize 1              //Ob bei jeder Loop, in der die Anzeige aktualisiert wird, auch die Fenstergröße angepasst werden soll

    #define fontFamilyAnzahl 1
    #define fontTextureImageSize POS{723,1024}  //Größe einer Font-Bilddatei in Pixel (verwendet in der Klasse FONT in der Datei graphics.cpp
    #define fontTextureSpriteAnz POS{10,11}     //Anzahl der Buchstaben/Symbole in einer Font-Bilddatei

    #define MaxXsize 20                         //Maximale Spielfeldgröße in x-Richtung
    #define MaxYsize 15                         //Maximale Spielfeldgröße in y-Richtung
    #define MaxSize POS{MaxXsize,MaxYsize}

    #define movementInfoSize 40                 //Größe der Bewegungsinformation (Drehende Animation)
    #define GameLogWidth 250                    //Breite des Game-Log Feldes


/** GAMELOG-Eigenschaften und Positionierungen: **/
    #define GAMELOG_ICONS_PER_BOX 6             //Wieviele gleiche Evenents in einer Box zusamengefasst werden dürfen (=max. anz. der Icons)

    #define GAMELOGBOXTYPE 3                    //Boxtyp der Eventboxen
    #define GAMELOGBORDERWITH 20                //Rahmenstärke der Eventboxen
    #define GAMELOGPADDING 5                    //Abstände
    #define GAMELOGMAINCOLOR COLOR{0,0.3,0}     //Hauptfarbe
    //#define GAMELOGNOTICESPACING 10
    #define GAMELOGBOXHEIGHT 58                 //Höhe einer Event-Box

    #define GameLogTitleFontSize 24             //Schriftgröße des Titels
    #define GameLogInfoFontSize 20              //Schriftgröße der Zeitangabe
    #define GameLogInfoColor COLOR{0.7,0.7,0.7} //Schriftfarbe der Zeitangabe

    #define GAMELOG_X (windX-GameLogWidth+GAMELOGBORDERWITH/4+GAMELOGPADDING)       //Position, ab der Ausgaben erlaubt sind

    #define GAMELOGBUTTONS_Y windY-movementInfoSize/2-(movementInfoSize-5)/2        //Y-Position für die Gamelog-Buttons
    #define GAMELOGshowBUTTONarea AREA{{windX-movementInfoSize,GAMELOGBUTTONS_Y-50},{windX-movementInfoSize+25,GAMELOGBUTTONS_Y-25}}    //Show-Button AREA
    #define GAMELOGhideBUTTONarea AREA{{GAMELOG_X,GAMELOGBUTTONS_Y-30},{GAMELOG_X+25,GAMELOGBUTTONS_Y-5}}                               //Hide-Button AREA
    #define GAMELOGsaveBUTTONarea AREA{{GAMELOG_X+30,GAMELOGBUTTONS_Y-30},{GAMELOG_X+55,GAMELOGBUTTONS_Y-5}}                            //Save-Button AREA
    #define GAMELOGpauseBUTTONarea AREA{{GAMELOG_X+60,GAMELOGBUTTONS_Y-30},{GAMELOG_X+85,GAMELOGBUTTONS_Y-5}}                           //Pause-Button AREA

/** PAUSE-Menü Eigenschaften und Positionierungen: **/



/** GAME-EINSTELLUNGEN **/
    //#define WALKING_SPEED  13                   //Wieviele % innerhalb eines Simulationszyklusses zurückgelegt werden (Um ein Objekt von einem Feld in ein anderes zu bewegen sind 100% notwendig) bei 2 sind also 50 Simulationszyklen notwendig, bis das Objekt von einem Feld ins nächste kommt
    #define OccupiedLimit Wkgl                  //Zuwieviel % sich ein Objekt in einem Feld befinden muss, damit dieses belegt ist (MUSS Wkgl sein: falls sich 2 Objekte in ein gleiches Feld zum gleichen Zeitpunkt bewegen kommt es sonst zu einer Überlagerung)

    #define RAILTRACKS_VISIBILITY_NXT 0.33f     //Um wieviel % die Sichtbarkeit eines Schienenstücks abnimmt - für die nächsten Schienen
    #define RAILTRACKS_VISIBILITY_PRV 1.0f      //Um wieviel % die Sichtbarkeit eines Schienenstücks abnimmt - für die bereits passierten Schienen


/** TYPE DEFINITIONS **/

    struct POS{
        int x,y;
    };
    struct AREA{
        POS a;
        POS b;
    };
    struct fPOS{
        float x,y;
    };
    struct fAREA{
        fPOS a;
        fPOS b;
    };


    struct COLOR{
        float r,g,b;
    };
    #define WHITE   COLOR{1.0f,1.0f,1.0f}
    #define BLACK   COLOR{0.0f,0.0f,0.0f}
    #define RED     COLOR{1.0f,0.0f,0.0f}
    #define GREEN   COLOR{0.0f,1.0f,0.0f}
    #define BLUE    COLOR{0.0f,0.0f,1.0f}
    #define YELLOW  COLOR{1.0f,1.0f,0.0f}
    #define MAGENTA COLOR{1.0f,0.0f,1.0f}
    #define CYAN    COLOR{0.0f,1.0f,1.0f}

    //Farben die als Overlay verwendet werden, wenn eine Collision-Animation ausgegeben wird:
    #define KUGELCOLLISIONCOLOR         COLOR{0.2,0.2,0.7}
    #define KUGELBLOCKCOLLISIONCOLOR    COLOR{0.5,0.25,0 }   //Kugel des Typs 1 (braun)
    #define AVATARCOLLITIONCOLOR        COLOR{1.0,0.2,0}
    //Farben für Schienen:
    #define TRANSPORTER_COLOR           COLOR{1.0,0.7,0.15}
    #define DEATHTRANSPORTER_COLOR      COLOR{0.6,0.2,0.8}

    enum DIRECTION{
        NONE=0,
        UP=1,
        DOWN=2,
        LEFT=3,
        RIGHT=4,
        BEAM=5
    };

    //Bewegungseigenschaften für ein sich bewegendes Objekt:
    struct MOVEMENT{            //Bewegung für Objekte, die sich am Spielfeld bewegen
        int moving;             //0: Keine Bewegung; 1: Vorwärtsbewegung; -1: rückwärts - er bewegt sich bereits wieder zurück weil er wo abgeprallt ist
        DIRECTION richtung;     //Richtung, in die sich das Objekt bewegt
        double progress;        //Wie weit sich das Objekt schon bewegt hat
        int limit;              //Wie weit sich das Objekt bewegen kann bevor es umkehrt

        bool blocking;          //1: Das Objekt blockiert  (Wird von einem Spezialfeld oder ähnlichem in ein anderes Feld geschoben dass aber nicht betreten werden kann. Dadurch ist das abprallen auch nicht möglich)

        DIRECTION lastRichtung; //Für Eis - welche Bewegungsrichtung zuletzt vollständig durchgeführt wurde (ohne abprallen usw.)
    };

    //Objekte für die Feldeigenschaften definiert sind (walkable, fieldproperty):
    enum OBJEKT{
        OBJ_AVATAR=0,
        OBJ_KUGEL=1
    };


/** GLOBALE VARIABLEN **/


    #define windX windowSize.x
    #define windY windowSize.y



/** FELDEIGENSCHAFTEN **/

    //Welche Nummer bestimmte Feldtypen in der Bilddatei haben:
    #define TILE_AVATAR         56
    #define TILE_KUGEL          57
    #define TILE_KUGELBLOCK     58
    #define TILE_LOCK           59
    #define TILE_KEY            60
    #define TILE_LAVA           46
    #define TILE_BLOCKEDLAVA    47
    #define TILE_TARGET         3
    #define TILE_FIXEDTARGET    4
    #define TILE_TYPECHANGE     40
    #define TILE_TYPECHANGEICE  41
    #define TILE_TARGETBEAMER   49
    #define TILE_ANIMATIONFIELD 63 //Rotes Feld für Einblendeanimationen
    #define TILE_RAIL_VERT      50
    #define TILE_RAIL_HORI      51
    #define TILE_RAIL_EDGE      52
    #define TILE_RAIL_HEND      54
    #define TILE_RAIL_VEND      53
    #define TILE_TRANSPORTER    61
    #define TILE_DEATHTRANSPORTER 62



    #define NW 0                //Not walkable - Das Feld kann überhaupt nicht betreten werden
    #define WB 12               //Walkable until Border - Feld kann bis zu 12% betreten werden, danach muss das Objekt wieder abprallen
    #define WC 22               //Walkable until Center - Feld kann bis zu 22% betreten werden, danach muss das Objekt wieder abprallen (wenn zB. bereits eine andere Kugel in diesem Feld ist)
    #define WA 100              //Das Feld kann ohne Probleme betreten werden.

    #define Wkgl 22             //Walkable until Kugel - Bei welchem Fortschritt eine Kugel berührt wird (muss >= WC sein, damit Kugeln, die sich auf Wänden befinden auch angestoßen werden können)
    #define Wava 18             //Walkable until Avatar - Bei welchem Fortschritt der Avatar berührt wird
    #define Wlock 10            //Walkable until Lock - Bei welchem Fortschritt ein Schloss berührt wird (ACHTUNG: Transporter können sich nur zu 0% in ein versperrtes Feld bewegen)
    #define Wkey 60             //Walkable until Key - Bei welchem Fortschritt ein Schlüssel berührt wird und genommen werden kann

//Werte für das alte Levelformat:
//    const int walkable[2][100]={    //Eigenschaften, welches Feld wie weit von einem Objekt betreten werden darf
//        //             00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99
//        /*0 = AVATAR*/{WB,WA,WA,WC,WA,WA,WA,WA,WC,WC,WC,WC,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WB,WA,WA,-1,-1,-1,-1,-1,-1,WA,WA,-1,WA,-1,-1,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,-1,WA,WA,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,WB,WB,WB,WB,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
//        /*1 = KUGEL */{WB,WA,WC,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WC,WC,WC,WC,WA,WA,WA,WA,WA,WB,WA,WA,-1,-1,-1,-1,-1,-1,WA,WA,-1,WA,-1,-1,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,-1,WA,WA,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,WB,WB,WB,WB,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
//        };  //Erster Index: Objekt-Typ (Datenwerte siehe Struktur "OBJEKT"). Inhalt: Prozentwert, wieweit das Feld betreten werden darf. 100=darf betreten werden. 0-99: nur zu x% betretbar, danach wieder zurückprallen. -1: Fehler - das Feld darf gar nicht vorkommen

    const int walkable[2][64]={    //Eigenschaften, welches Feld wie weit von einem Objekt betreten werden darf
        //             00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
        /*0 = AVATAR*/{WB,WA,WB,WA,WA,WA,WC,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WC,WC,WC,WC,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WB,WB,WB,WB,WA,WA,WA,WA,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        /*1 = KUGEL */{WB,WA,WB,WA,WA,WC,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WC,WC,WC,WC,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WA,WB,WB,WB,WB,WA,WA,WA,WA,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
        };  //Erster Index: Objekt-Typ (Datenwerte siehe Struktur "OBJEKT"). Inhalt: Prozentwert, wieweit das Feld betreten werden darf. 100=darf betreten werden. 0-99: nur zu x% betretbar, danach wieder zurückprallen. -1: Fehler - das Feld darf gar nicht vorkommen

    enum FIELDPROPERTY{
        er = -1,
        nm, /*normal*/
        up, /*up*/
        dn, /*down*/
        lt, /*left*/
        rt, /*right*/
        bm, /*beamer*/
        ic, /*rutschen*/
        tr, /*um 90° im Uhrzeigersinn weiter rutschen*/
        tl, /*um 90° gegen den Urhzeigersinn weiter rutschen*/
        dt, /*death - tödlich*/
        fx  /*fixiert - nicht beweglich*/
    };

//Werte für das alte Levelformat:
//    const FIELDPROPERTY fieldproperty[2][100]={    //Eigenschaften ob dieses Feld eine besondere Fähigkeit für ein Objekt hat, das sich auf diesem Feld befindet (zB. ein Objekt weiter transportiert)
//        //             00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55, 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99
//        /*0 = AVATAR*/{nm,nm,nm,nm,nm,nm,nm,nm,nm,nm,nm,nm,rt,lt,up,dn,rt,lt,up,dn,rt,lt,up,dn,bm,nm,ic,nm,er,er,er,er,er,er,dt,nm,er,nm,er,er,rt,lt,up,dn,nm,nm,tl,tr,tl,tr,tr,tl,nm,er,nm,ic,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,dt,dt,dt,dt,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er},
//        /*1 = KUGEL */{nm,nm,nm,nm,rt,lt,up,dn,rt,lt,up,dn,nm,nm,nm,nm,nm,nm,nm,nm,rt,lt,up,dn,bm,nm,ic,nm,er,er,er,er,er,er,dt,nm,er,fx,er,er,lt,rt,dn,up,tl,tr,nm,nm,tl,tr,tl,tr,nm,er,nm,ic,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,dt,dt,dt,dt,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er,er}
//        };  //Erster Index: Objekt-Typ (Datenwerte siehe Struktur "OBJEKT").
    const FIELDPROPERTY fieldproperty[2][64]={    //Eigenschaften ob dieses Feld eine besondere Fähigkeit für ein Objekt hat, das sich auf diesem Feld befindet (zB. ein Objekt weiter transportiert)
        //             00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63
        /*0 = AVATAR*/{nm,nm,nm,nm,nm,nm,nm,ic,up,dn,lt,rt,nm,nm,nm,nm,up,dn,lt,rt,nm,nm,nm,nm,up,dn,lt,rt,up,dn,lt,rt,tl,tr,nm,nm,tl,tr,tr,tl,nm,ic,nm,nm,nm,nm,dt,nm,bm,nm,er,er,er,er,er,er,er,er,er,er,er,er,er,er},
        /*1 = KUGEL */{nm,nm,nm,nm,fx,nm,nm,ic,nm,nm,nm,nm,up,dn,lt,rt,nm,nm,nm,nm,up,dn,lt,rt,up,dn,lt,rt,dn,up,rt,lt,nm,nm,tl,tr,tl,tr,tl,tr,nm,ic,nm,nm,nm,nm,dt,nm,bm,nm,er,er,er,er,er,er,er,er,er,er,er,er,er,er}
        };  //Erster Index: Objekt-Typ (Datenwerte siehe Struktur "OBJEKT").

/** PROTOTYPEN **/
    void cleanupFrameArray();                                                                       //Löscht den Inhalt des gesamten Framearrays
    void transformY(POS* position);                                                                 //Die Y-Koordinate wird gespiegelt (Ursprung oben/unten vertauscht)
    bool coordInside(POS coord,AREA gebiet);                                                        //Meldet, ob sich die übergebenen Koordinaten innerhalb des übergebenen Gebiets befinden (zB. für "Maus über Bild?)
    void getMousePos(POS *coord);                                                                   //Gibt die Position der Maus im Koordinatensystem zurück
    void PosSizeToArea(AREA *data);                                                                 //Wandelt eine Struktur, die aus Position+Größe besteht um, in Startposition+Endposition
    AREA PosSizeToArea(AREA data);                                                                  //Wandelt eine Struktur, die aus Position+Größe besteht um, in Startposition+Endposition
    POS getFieldCoord(POS position,DIRECTION richtung);                                             //Gibt die Koorinaten des Nachbarfeldes, abhängig von der Richtung, zurück
    bool propRicCmp(DIRECTION richtung,FIELDPROPERTY property);                                     //Vergleicht die beiden unterschiedlichen Datentypen auf Gleichheit (1=gleich)
    bool colorcmp(COLOR a,COLOR b);                                                                 //Vergleicht 2 Farben auf gleichheit (1=gleich)
    bool poscmp(POS a,POS b);                                                                       //Vergleicht 2 Positionen auf übereinstimmung
    DIRECTION turnRight(DIRECTION richtung);                                                        //Gibt die um 90° im Uhrzeigersinn gedrehte Richtung zurück
    DIRECTION turnLeft(DIRECTION richtung);                                                         //Gibt die um 90° gegen den Uhrzeigersinn gedrehte Richtung zurück


#endif // DEFINITIONS_H_INCLUDED
