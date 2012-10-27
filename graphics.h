//Version 0.1
//Jakob Maier

#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED


/** EINSTELLUNGEN **/
	#define GPRINTF_BUFFER 1024 //max. Stringlänge für die Funktion gprintf()


/** TYPE DEFINITIONS **/
     //akt. Anzeigemodus
    enum GRAPHICMODES{
        DRAWING,    //Zeichnen (Linien, Pixel, Kreise, Quadrate,...)
        TEXTURES    //Einbinden von Texturen
    };

    struct fontSymbol{  //Einzelnes Zeichen eines fonts
        const char symbol;      //Zeichen (zB. 'a', 'b', '0', '@', ...)
        const float width;      //Zeichenbreite (0.0 - 1.0)
    };

    enum TEXTALIGN{     //Für die Ausrichtung von Text
        taLEFT,
        taCENTER,
        taRIGHT
    };


/** GLOBALE VARIABLEN **/

    GRAPHICMODES graphicMode=DRAWING;

    static const fontSymbol font[11][10]={
                                        {{'A',0.95}, {'B',0.75}, {'C',0.8 }, {'D',0.84}, {'E',0.72}, {'F',0.7 }, {'G',0.84}, {'H',0.73}, {'I',0.45}, {'J',0.68}},
                                        {{'K',0.79}, {'L',0.76}, {'M',0.9 }, {'N',0.8 }, {'O',0.85}, {'P',0.75}, {'Q',0.95}, {'R',1.0 }, {'S',0.6 }, {'T',0.83}},
                                        {{'U',0.8 }, {'V',0.9 }, {'W',1.0 }, {'X',0.78}, {'Y',0.77}, {'Z',0.95}, {' ',0.4 }, {'!',0.25}, {'?',0.42}, {'.',0.24}},
                                        {{'a',0.5 }, {'b',0.6 }, {'c',0.6 }, {'d',0.6 }, {'e',0.55}, {'f',0.6 }, {'g',0.6 }, {'h',0.55}, {'i',0.23}, {'j',0.4 }},
                                        {{'k',0.53}, {'l',0.24}, {'m',0.73}, {'n',0.53}, {'o',0.62}, {'p',0.76}, {'q',0.6 }, {'r',0.45}, {'s',0.48}, {'t',0.53}},
                                        {{'u',0.55}, {'v',0.57}, {'w',0.75}, {'x',0.62}, {'y',0.46}, {'z',0.6 }, {',',0.25}, {':',0.27}, {';',0.29}, {'#',0.72}},
                                        {{'_',0.63}, {'+',0.52}, {'-',0.34}, {'*',0.46}, {':',0.26}, {'\"',0.35},{'<',0.43}, {'>',0.47}, {'~',0.62}, {'/',0.4 }},
                                        {{'0',0.6 }, {'1',0.46}, {'2',0.56}, {'3',0.49}, {'4',0.5 }, {'5',0.5 }, {'6',0.47}, {'7',0.58}, {'8',0.5 }, {'9',0.51}},
                                        {{'\\',0.42},{'=',0.51}, {'(',0.35}, {')',0.25}, {'\'',0.2 },{'%',0.77}, {'©',0.64}, {'€',0.63}, {'@',0.92}, {' ',0.0 }},
                                        {{'&',0.62}, {'|',0.2 }, {'^',0.5 }, {' ',0.0 }, {' ',0.0 }, {' ',0.0 }, {'[',0.39}, {']',0.38}, {' ',0.0 }, {' ',0.0 }},
                                        {{'Ä',0.96}, {'Ö',0.85}, {'Ü',0.8 }, {'ä',0.49}, {'ö',0.61}, {'ü',0.55}, {'ß',0.5 }, {' ',0.0 }, {' ',0.0 }, {' ',0.0 }}
                                    };

    GLuint *fontTextur=NULL;    //Zeiger auf die Textur, die die Bilddaten der Font-Datei enthält (zum ändern der Schriftart diesen Zeiger ändern)


/** PROTOTYPEN **/

void init_window();                                                                                 //Initialisiert das Spiel
void prepare_GameLoop();                                                                            //Wird vor dem betreten der Spiele- und Anzeigeschleife aufgerufen
void prepare_graphics();                                                                            //Wird zu beginn jedes Durchgangs in der Spiele- und Anzeigeschleife aufgerufen
int complete_graphics(long loopStart,unsigned int loopSpeed);                                       //Wird am Ende jedes Durchgangs in der Spiele- und Anzeigeschleife aufgerufen
void cleanup();                                                                                     //Wird beim beenden des Programms aufgerufen
void switchGraphicMode(GRAPHICMODES target);                                                        //Schaltet den Grafikmodus um (Zeichnen/Texturen)
void markArea(AREA gebiet,COLOR farbe);                                                             //Umrandet ein Gebiet
GLuint LoadTexture(char *TexName);                                                                  //UP für das laden von Tga datein
void putImage(AREA display,fAREA TexCoord,COLOR overlay);                                           //Textur ausgeben
void putSprite(POS texSize,POS textPos,AREA display,COLOR overlay);                                 //Teil einer Textur ausgeben (Wenn in einer Bilddatei mehrere Teillemente vorhanden sind)
int putLetter(char letter,POS position,int fontSize,COLOR fontColor);                               //Gibt ein Zeichen am Bildschirm aus (A-Z, a-z, 0-9, Sonderzeichen)
int getFontWidth(const char *text,int fontSize);                                                    //Gibt zurück, wieviel Platz ein Text in x-Richtung am Bildschirm brauchen würde (zB. für Zentrierte und Rechtsbündige Texte)
int putString(const char *text,POS position,int fontSize,COLOR fontColor,TEXTALIGN ausrichtung);    //Gibt einen Text aus
int gprintf(POS position,int fontSize,COLOR fontColor,TEXTALIGN ausrichtung,const char *format,...);//printf für Grafikfenster - gibt einen formatierten String aus - ACHTUNG: max. Textlänge!
void setFont(GLuint *textur);                                                                       //Stellt die Schriftart ein






#endif // GRAPHICS_H_INCLUDED
