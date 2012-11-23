#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

//enum BUTTONID                               //Eine eindeutige ID für jeden Button, der im Spiel irgendwo irgendwann vorkommt
//{   BUTTON_NO_BUTTON,
//
//
//    BUTTON_GAMELOGHIDE,                     //Gamelogger: Button zum ausblenden
//    BUTTON_GAMELOGSAVE                      //Gamelogger: Button zum speichern im Infobereich
//
//
//
//};



void getMousePos(POS *coord);      //Gibt die Position der Maus im Koordinatensystem zurück


//enum MOUSEBUTTON                            //Maustasten
//{   MBLEFT=0,
//    MBRIGHT=1,
//    MBMIDDLE=2
//};



//COLOR getButtonOverlayColor(BUTTONID buttonID,AREA gebiet,BUTTONID mousebutton);         //Gibt eine Overlay-Farbe für einen Button zurück


//class MOUSE
//{
//    private:
//        POS position;                       //Mausposition
//
//        BUTTONID buttonID[3];                 //Wenn auf einen Button geklickt wurde
//        BUTTONID clickedButtonID[3];          //In diesem Schleifenzyklus gedrückte Buttons
//
//
//        void updateMousePos(POS *coord);    //Gibt die Position der Maus im Koordinatensystem zurück
//    public:
//        MOUSE();                            //Konstruktor
//        POS getMousePos();                  //Gibt die Mausposition zurück
//        void run();                         //Muss am Anfang der Schleife aufgerufen werden, wird von prepare_graphics(); automatisch erledigt
//
//        int getButtonState(BUTTONID _buttonID,const AREA bereich,MOUSEBUTTON mousebutton);
//            ///Return-Wert:
//            //0: nichts
//            //1: hover
//            //2: active
//            //3: clicked
//
//        bool isHover(BUTTONID _buttonID,const AREA bereich);                              //Gibt zurück, ob sich die Maus über einem Button befindet
//        bool isActive(BUTTONID _buttonID,const AREA bereich,MOUSEBUTTON mousebutton);     //Gibt zurück, ob sich die Maus MIT GEDRÜCKTER MAUSTASTE über einem Button befindet
//        bool isClicked(BUTTONID _buttonID,MOUSEBUTTON mousebutton);                       //Gibt zurück, ob der Button geklickt wurde (Maustaste losgelassen)
//
//};





















class BUTTON//Ein Button, den man drücken kann
{
    private:
        ///Position:
            AREA output;

        ///Zustand:
            int active;                            //Wenn 1: Die Maus hat die Taste gedrückt. Und das über dem button
                //0: nein
                //1: links
                //2: rechts
                //3: mitte

        ///Eigenschaften:

            char clickable;                         //Für welche Maustasten dieser Button verwendet werden kann
                //0: nur links
                //1: nur rechts
                //2: nur mitte
                //3: links + rechts
                //4: links + mitte
                //5: rechts + mitte
                //6: links + rechts + mitte

            int type;                               //Wie der Button ausgegeben werden soll
                //0: Keine Ausgabe
                //1: Textur
                //2: Textur + Text
                //3: Box
                //4: Box + Text


            //type==1 || type==2:
                class TEXTURE *textur;                   //Textur, wenn eine verwendet wird
                POS spritePos;                          //Teil der Textur, die ausgegeben werden soll
            //type==3 || type==4
                int boxtype;                            //Welche Boxart verwendet wird
                COLOR color;                                    //(Typ==2: Overlay, nicht ganz weiß)
                int borderWidth;                        //Rahmendicke
            //type==2 || type==4
                char buttonText[128];                   //Text, der Ausgegeben wird
                int fontSize;
                COLOR fontcolor;                        //Schriftfarbe


    public:

        BUTTON(AREA _output,char _clickable);                                                                //Typ 0 initialisieren
        BUTTON(AREA _output,char _clickable,TEXTURE *_textur,POS _spritePos);                                //Typ 1 initialisieren
        BUTTON(AREA _output,char _clickable,TEXTURE *_textur,POS _spritePos,const char* const value,int _fontSize,COLOR _fontcolor);   //Typ 2 initialisieren
        BUTTON(AREA _output,char _clickable,int _boxtype,int _borderWidth,COLOR _color);                                      //Typ 3 initialisieren
        BUTTON(AREA _output,char _clickable,int _boxtype,int _borderWidth,COLOR _color,const char* const value,int _fontSize,COLOR _fontcolor);         //Typ 4 initialisieren

        char clicked();     //Gibt zurück ob, und welche Maustatste diesen Button geklickt hat (muss VOR pint() aufgerufen werden)
        void print();       //Button ausgeben




};





#endif // BUTTON_H_INCLUDED
