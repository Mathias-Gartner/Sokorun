//Version 0.1
//Jakob Maier
//Grafikfunktionen

#define M_PI		3.14159265358979323846

extern TEXTURE boxTextures;
extern POS mouse;
//extern MOUSE mouse;


void init_window()//Initialisiert das Spiel
{
    glfwInit();                     //Initialisiert alles
    if( !glfwOpenWindow(windowSize.x*coordPixel,windowSize.y*coordPixel, 0, 0, 0, 0, 0, 0, GLFW_WINDOW ) ) //Fenster öffnen
    {   glfwTerminate();            //Beenden
        return;
    }
    glfwSetWindowTitle("SokoRun");          //Fenstername
    if(DEBUG)                                                       //Ändern in "main.h"
    {   system("title SokoRun Debug Window");                       //Konsolenfenstername
        logger(1,"Das Programm wurde im Debug-Modus gestartet");
        logger(1,"Versionsnummer: %s",VERSION);                       //Versionsnummer (Ändern in "main.h")
    }
}


void prepare_GameLoop()//Wird vor dem betreten der Spiele- und Anzeigeschleife aufgerufen
{   //ALPHA-Werte Aktivieren (Transparenz):
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   //Definition für das Überlagern von Bilddaten
    glEnable(GL_BLEND);                                 //Blend aktivieren
    glAlphaFunc(GL_GREATER,0.1f);
    glEnable(GL_ALPHA_TEST);

    /*int width, height;
    glfwGetWindowSize(&width,&height);
    glViewport( 0, 0, width, height );              //set Origin (außerhalb kann nicht gezeichnet werden)
    glOrtho(0,windowSize.x,0,windowSize.y,0,128);   //2D-Modus; z-Koordinate wird nicht verwendet
    */

    graphicMode=DRAWING;
}

void prepare_graphics()//Wird zu beginn jedes Durchgangs in der Spiele- und Anzeigeschleife aufgerufen
{
    static int width, height;       //Tatsächliche Fentergröße in Pixel

    if(AutoAdjustWindowSize)        //Seitenverhältnisse kontrollieren
    {   glfwGetWindowSize(&width,&height);
        if((float)width/(float)height != ((float)windowSize.x / (float)windowSize.y))
        {   width = ((float)windowSize.x / (float)windowSize.y) * height;
            glfwSetWindowSize(width,height);
            coordPixel=(float)width/windowSize.x;
            glViewport( 0, 0, width, height );              //set Origin (außerhalb kann nicht gezeichnet werden)
        }

    }
    height = height > 0 ? height : 1;

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );         //Hintergrundfarbe
    glClear(GL_COLOR_BUFFER_BIT);
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,windowSize.x,0,windowSize.y,0,128);   //2D-Modus; z-Koordinate wird nicht verwendet
    glMatrixMode(GL_MODELVIEW);                     //auf Standard-Matrix umschalten

    getMousePos(&mouse);                            //Mauskoordinaten erhalten
    //mouse.run();                                    //Mausdaten aktualisieren

}


//Übergabeparameter:
//  1.) loopStart           [time]  Zeitpunkt, zu dem die Spiele- und Anzeigeschleife in diesen Durchgang gestartet ist
//  (2) loopSpeed           [ms]    Wielange ein Schleifendurchgang dauern muss
//Rückgabewert:
//  -)  Abbruchbedingung    [bool]  Ob die Schleife abgebrochen werden soll (false) oder weiterlaufen darf (true)
int complete_graphics(long loopStart,unsigned int loopSpeed=10)//Wird am Ende jedes Durchgangs in der Spiele- und Anzeigeschleife aufgerufen
{   if(kbhit()){getch();system("CLS");}             //Ein Tastendruck in der Konsole führt zum löschen des Inhalts

    glfwSwapBuffers();                              //erzeugte Grafikdaten ausgeben
    static int td;
    td=loopSpeed-(clock()-loopStart);

    if(TIMEDEBUGOUTPUT) printf("%4dms -> ",(int)(clock()-loopStart));

    if(td-1 > 0){Sleep(td-1);}                      //Grob, Blockiert das Programm --> Statt 100% CPU-Auslasten nur noch 10% (Stand: Ver. 13.11.2012)
    while(clock()-loopStart<(int)loopSpeed);        //genaue, gleichmäßige Geschwindigkeit garantieren

    if(TIMEDEBUGOUTPUT) printf("%4dms (soll: %4dms)\n",(int)(clock()-loopStart),loopSpeed);

    return (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED));//Abbruchbedingung
}

void cleanup()//Wird beim beenden des Programms aufgerufen
{
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
    glfwTerminate();            //Fensters schließen
}


void winkelKorr(int& gamma)  //Winkelwert in einen gültigen Wert (0-359) umwandeln
{
    while(gamma<0)      gamma+=360;
    while(gamma>=360)   gamma-=360;
}


///KLASSE TEXTURE
/*private*/
void TEXTURE::loadTexture()//TGA-Datei laden
{
    TGAImg Img;        // Image loader
    // Load our Texture
    if(Img.Load(path)!=IMG_OK)
    {   error("TEXTURE::loadTexture()","Eine Bilddatei konnte nicht geladen werden. Pfad: \"%s\". Es wird stattdessen eine weisse Textur verwendet",path);
        textur=-1;
        loaded=1;
        return;
    }

    glGenTextures(1,&textur);            // Allocate space for texture
    bindTexture();                      // Set our Tex handle as current

    // Create the texture
    if(Img.GetBPP()==24)
        glTexImage2D(GL_TEXTURE_2D,0,3,Img.GetWidth(),Img.GetHeight(),0,
                    GL_RGB,GL_UNSIGNED_BYTE,Img.GetImg());
    else if(Img.GetBPP()==32)
        glTexImage2D(GL_TEXTURE_2D,0,4,Img.GetWidth(),Img.GetHeight(),0,
                    GL_RGBA,GL_UNSIGNED_BYTE,Img.GetImg());
    else
    {   error("TEXTURE::loadTexture()","Eine Bilddatei konnte nicht geladen werden. Pfad: \"%s\". Es wird stattdessen eine weisse Textur verwendet",path);
        textur=-1;
        loaded=1;
        return;
    }

    // Specify filtering and edge actions
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//Wenn das Image > als der Screen ist (mehrere Image-Pixel = ein Screen-Pixel)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//ein Image-Pixel = mehrere Screen-Pixel
    if(allowTextureRepeat)//Die Textur wird bei Texturkoordinaten > 1 wiederholt
    {   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    }else
    {   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    }

    loaded=1;
    return;
}


TEXTURE::TEXTURE(const char* const imgPath, POS imgSize=POS{0,0}, POS imgSprites=POS{1,1},bool _allowTextureRepeat=0)
{   loaded=0;
    strcpy(path,imgPath);
    sprites=imgSprites;
    allowTextureRepeat=_allowTextureRepeat; //Ob das Bild wiederholt werden soll, wenn die Texturkoordinaten >1 sind (zB. Hintergrundbild). Wenn nicht, sit 0 empfohlen.

    if((imgSize.x<1 || imgSize.y<1) && (imgSprites.x>1 || imgSprites.y>1))
    {   error("TEXTURE::TEXTURE()","Schwerer Fehler: Es wurde eine Texture initialisiert, die weniger als 1 Pixel breit und/oder hoch ist. Bildpfad: \"%s\", imgSize: (%dx%d), Sprites: (%dx%d)",imgPath,imgSize.x,imgSize.y,imgSprites.x,imgSprites.y);
        exit(1);
    }
    if(imgSprites.x<1 || imgSprites.y<1)
    {   error("TEXTURE::TEXTURE()","Schwerer Fehler: Es wurde eine Texture initialisiert, die weniger als 1 Sprite in x und/oder y Richtung beinhaltet. Bildpfad: \"%s\", imgSize: (%dx%d), Sprites: (%dx%d)",imgPath,imgSize.x,imgSize.y,imgSprites.x,imgSprites.y);
        exit(1);
    }
    halfTexelSize={(0.5f/imgSize.x),(0.5f/imgSize.y)};
    spriteSize={(1.0f/sprites.x),(1.0f/sprites.y)};
}

void TEXTURE::print(AREA display,fAREA textArea=stdTextArea,COLOR overlay=WHITE,float alpha)             //Ausgabe des gesamten Bildes
{   if(!loaded) loadTexture();   //Grafik laden, wenn das noch nicht geschehen ist

    switchGraphicMode(TEXTURES);
    glColor4f(overlay.r,overlay.g,overlay.b,alpha);
    bindTexture();
    glBegin(GL_QUADS);
        glTexCoord2f(textArea.a.x,textArea.b.y);glVertex2f(display.a.x,display.a.y);
        glTexCoord2f(textArea.a.x,textArea.a.y);glVertex2f(display.a.x,display.b.y);
        glTexCoord2f(textArea.b.x,textArea.a.y);glVertex2f(display.b.x,display.b.y);
        glTexCoord2f(textArea.b.x,textArea.b.y);glVertex2f(display.b.x,display.a.y);
    glEnd();
}

void TEXTURE::bindTexture() //Bindet eine Textur
{
    static GLint act;
    glGetIntegerv(GL_TEXTURE_BINDING_2D,&act);
    if(act!=(int)textur)
        glBindTexture(GL_TEXTURE_2D,textur);
}

void TEXTURE::print(AREA display,POS spritePos,COLOR overlay=WHITE,float alpha)                          //Ausgabe eines Sprites
{   if(!loaded) loadTexture();   //Grafik laden, wenn das noch nicht geschehen ist
    switchGraphicMode(TEXTURES);
    glColor4f(overlay.r,overlay.g,overlay.b,alpha);
    bindTexture();
    glBegin(GL_QUADS);
        glTexCoord2f(spriteSize.x* spritePos.x   +halfTexelSize.x,spriteSize.y*(spritePos.y+1)-halfTexelSize.y);   glVertex2f(display.a.x,display.a.y);
        glTexCoord2f(spriteSize.x* spritePos.x   +halfTexelSize.x,spriteSize.y* spritePos.y   +halfTexelSize.y);   glVertex2f(display.a.x,display.b.y);
        glTexCoord2f(spriteSize.x*(spritePos.x+1)-halfTexelSize.x,spriteSize.y* spritePos.y   +halfTexelSize.y);   glVertex2f(display.b.x,display.b.y);
        glTexCoord2f(spriteSize.x*(spritePos.x+1)-halfTexelSize.x,spriteSize.y*(spritePos.y+1)-halfTexelSize.y);   glVertex2f(display.b.x,display.a.y);
    glEnd();
}

//void TEXTURE::print(AREA display,POS spritePos,fAREA spriteArea,COLOR overlay,float alpha)      //Ausgabe eines Sprite-Teiles (zum drehen und spiegeln)
//{   if(!loaded) loadTexture();   //Grafik laden, wenn das noch nicht geschehen ist
//
//    switchGraphicMode(TEXTURES);
//    glColor4f(overlay.r,overlay.g,overlay.b,alpha);
//    bindTexture();
//
//    glBegin(GL_QUADS);
//        glTexCoord2f(spriteSize.x*(spritePos.x+spriteArea.a.x)  +halfTexelSize.x,spriteSize.y*(spritePos.y+spriteArea.b.y)  -halfTexelSize.y);   glVertex2f(display.a.x,display.a.y);
//        glTexCoord2f(spriteSize.x*(spritePos.x+spriteArea.a.x)  +halfTexelSize.x,spriteSize.y*(spritePos.y+spriteArea.a.y)  +halfTexelSize.y);   glVertex2f(display.a.x,display.b.y);
//        glTexCoord2f(spriteSize.x*(spritePos.x+spriteArea.b.x)  -halfTexelSize.x,spriteSize.y*(spritePos.y+spriteArea.a.y)  +halfTexelSize.y);   glVertex2f(display.b.x,display.b.y);
//        glTexCoord2f(spriteSize.x*(spritePos.x+spriteArea.b.x)  -halfTexelSize.x,spriteSize.y*(spritePos.y+spriteArea.b.y)  -halfTexelSize.y);   glVertex2f(display.b.x,display.a.y);
//    glEnd();
//}





void TEXTURE::print(AREA display,POS spritePos,COLOR overlay,float alo,float aro, float alu,float aru)               //Ausgabe eines Sprites; Jede Texturkoordinate hat einen andren Alpha-Wert
{   if(!loaded) loadTexture();   //Grafik laden, wenn das noch nicht geschehen ist

    switchGraphicMode(TEXTURES);
    bindTexture();

    glBegin(GL_QUADS);
        glColor4f(overlay.r,overlay.g,overlay.b,alo);
        glTexCoord2f(spriteSize.x* spritePos.x   +halfTexelSize.x,spriteSize.y*(spritePos.y+1)-halfTexelSize.y);   glVertex2f(display.a.x,display.a.y);
        glColor4f(overlay.r,overlay.g,overlay.b,alu);
        glTexCoord2f(spriteSize.x* spritePos.x   +halfTexelSize.x,spriteSize.y* spritePos.y   +halfTexelSize.y);   glVertex2f(display.a.x,display.b.y);
        glColor4f(overlay.r,overlay.g,overlay.b,aru);
        glTexCoord2f(spriteSize.x*(spritePos.x+1)-halfTexelSize.x,spriteSize.y* spritePos.y   +halfTexelSize.y);   glVertex2f(display.b.x,display.b.y);
        glColor4f(overlay.r,overlay.g,overlay.b,aro);
        glTexCoord2f(spriteSize.x*(spritePos.x+1)-halfTexelSize.x,spriteSize.y*(spritePos.y+1)-halfTexelSize.y);   glVertex2f(display.b.x,display.a.y);
    glEnd();
}


void TEXTURE::print(POS position,int size,POS spritePos,int angle,COLOR overlay=WHITE,float alpha)                   //Ausgabe eines Sprites mit einer rotation
{   if(!loaded) loadTexture();   //Grafik laden, wenn das noch nicht geschehen ist

    switchGraphicMode(TEXTURES);
    glColor4f(overlay.r,overlay.g,overlay.b,alpha);
    bindTexture();



    //POS size={display.b.x-display.a.x,display.b.y-display.a.y};     //Bildgröße ermitteln
    //POS position={(position.x+size/2),(position.y+size/2)};   //Bildmittelpunkt ermitteln

    POS ecke[4];        //Eckpunkte des neuen, gedrehten Bildes

    int x,y;
    double s=sqrt(size*size*2)/2;                   //Abstand (Außenradius) zu jeder Ecke

    angle+=135;

    for(int i=0;i<4;i++)                                            //Für jede Ecke durchgehen
    {   winkelKorr(angle);

        x=cos((double)angle/180*M_PI)*s;                            //Umrechnung auf Radiant + x-Abweichung vom Mittelpunkt zur neuen Position berrechnen
        y=sin((double)angle/180*M_PI)*s;                            //Umrechnung auf Radiant + y-Abweichung vom Mittelpunkt zur neuen Position berrechnen

        ecke[i]={position.x+x,position.y+y};                        //Position abspeichern
        angle+=90;                                                  //Nächste Ecke um 90° verschoben
    }

//printf("%d x %d   %f--- (%dx%d) (%dx%d) (%dx%d) (%dx%d)\n",position.x,position.y,s,ecke[0].x,ecke[0].y,ecke[1].x,ecke[1].y,ecke[2].x,ecke[2].y,ecke[3].x,ecke[3].y);
//markArea({ecke[0],ecke[1]},WHITE);
    ///Ausgabe:
    glBegin(GL_QUADS);
        glTexCoord2f(spriteSize.x* spritePos.x   +halfTexelSize.x,spriteSize.y*(spritePos.y+1)-halfTexelSize.y);   glVertex2f(ecke[1].x,ecke[1].y);
        glTexCoord2f(spriteSize.x* spritePos.x   +halfTexelSize.x,spriteSize.y* spritePos.y   +halfTexelSize.y);   glVertex2f(ecke[0].x,ecke[0].y);
        glTexCoord2f(spriteSize.x*(spritePos.x+1)-halfTexelSize.x,spriteSize.y* spritePos.y   +halfTexelSize.y);   glVertex2f(ecke[3].x,ecke[3].y);
        glTexCoord2f(spriteSize.x*(spritePos.x+1)-halfTexelSize.x,spriteSize.y*(spritePos.y+1)-halfTexelSize.y);   glVertex2f(ecke[2].x,ecke[2].y);
    glEnd();
}


int TEXTURE::getSpriteAnz()     //Gibt die Anzahl der Sprites zurück die sich im Bild befinden
{   return (sprites.x*sprites.y);
}

POS TEXTURE::getSprites()       //Die Anzahl der Sprites wird zurückgegeben
{   return sprites;
}

///KLASSE FONT (abgeerbt von TEXTURE)

FONT::FONT(int fontFamilyNum)
{   loaded=0;

    if(fontFamilyNum < 0 || fontFamilyNum >= fontFamilyAnzahl)    //fontFamilyAnzahl: definiert in definitions.h
    {   error("FONT::FONT()","Die uebergene Schriftart ist ungueltig - Verwende Schriftart 0. fontFamily=%d",fontFamilyNum);
        fontFamily=0;
    }else
        fontFamily=fontFamilyNum;

    sprintf(path,"daten/texturen/font%d.tga",fontFamily);

    sprites=fontTextureSpriteAnz;                                                   //Definition in definitions.h
    halfTexelSize={(0.5f/fontTextureImageSize.x),(0.5f/fontTextureImageSize.y)};      //Definition in definitions.h
    spriteSize={(1.0f/sprites.x),(1.0f/sprites.y)};
    ratio=(fontTextureImageSize.y/sprites.y) / (fontTextureImageSize.x/sprites.x);

    size=50;                    //Default-Schriftgröße
    color={1.0,1.0,1.0};        //Default-Farbe = weiß
}

void FONT::setFontSize(const int newSize)//Schriftgröße ändern
{   if(newSize>0)
        size=newSize;
    else
        error("FONT::setFontSize()","Die Schriftgroesse ist ungueltig. newSize=%d",newSize);
}

void FONT::setFontColor(COLOR newColor)//Schriftfarbe ändern
{   color=newColor;
}

int FONT::putLetter(char letter,POS position)//Gibt ein Zeichen am Bildschirm aus (A-Z, a-z, 0-9, Sonderzeichen)
{   POS mat={-1,-1};//Position in der font-Tabelle

    for(int y=0;y<11;y++)
    {   for(int x=0;x<10;x++)
        {   if(font[fontFamily][y][x].symbol==letter)    //gefunden
            {   mat.x=x; mat.y=y;
                y=11;
                break;
            }
        }
    }

    if(mat.x>=0&&mat.y>=0)//Existiert
    {   print(PosSizeToArea({position,{(int)(size*ratio),size}}),mat,color);
        return position.x+size*ratio*(font[fontFamily][mat.y][mat.x].width+0.05); //Nächste Buchstabenposition
    }
    else
    {   error("FONT::putLetter()","Unallowed Symbol '%c' - no output",letter);
        return position.x;
    }
}

int FONT::getFontWidth(const char *text)//Gibt zurück, wieviel Platz ein Text in x-Richtung am Bildschirm brauchen würde (zB. für Zentrierte und Rechtsbündige Texte)
{   int width=0;
    while(*text!=0)
    {   for(int y=0;y<11;y++)
        {   for(int x=0;x<10;x++)
            {   if(font[fontFamily][y][x].symbol==*text)    //gefunden
                {   width+=size*ratio*(font[fontFamily][y][x].width+0.05);
                    y=11;
                    break;
                }
            }
        }
        text++;
    }
    return width;
}

int FONT::putString(const char *text,POS position,TEXTALIGN ausrichtung=taLEFT)           //Gibt einen Text aus
{   if(ausrichtung==taCENTER)   position.x-=getFontWidth(text)/2;
    if(ausrichtung==taRIGHT)    position.x-=getFontWidth(text);

    while(*text!=0)
    {   position.x=putLetter(*text,position);
        text++;
    }
    return position.x;
}

int FONT::printf(POS position,TEXTALIGN ausrichtung,const char *format,...)              //Printf für Grafikfenster - ACHTUNG: max. Textlänge!
{
    static char buffer[GPRINTF_BUFFER+1];											//Buffer
    va_list  argptr;																//Argument-Liste
    va_start( argptr, format );
    vsprintf( buffer, format, argptr );												//Mit sprintf-Funktion in Buffer übertragen
    va_end  ( argptr );
    buffer[GPRINTF_BUFFER]=0;														//Zur Sicherheit
    return putString(buffer,position,ausrichtung);
}
////Übergabeparameter:
////  1.) textur          [GLuint]    Bereits zuvor geladene Textur
////  2.) display         [AREA]      Koordinate a = linke,untere Position des Bildes
////                                  Koordinate b = rechte, obere Position des Bildes
////  (3) TexCoord        [AREA]      Zum zuschneiden des Bildes (Wertebereich 0.0f - 1.0f)
////  (4) overlay         [COLOR]     zum einfärben der Textur
//void putImage(GLuint textur,AREA display,fAREA TexCoord=stdTextArea,COLOR overlay=WHITE)//Textur ausgeben
//{
//    switchGraphicMode(TEXTURES);
//    glColor3f(overlay.r,overlay.g,overlay.b);
//    glBindTexture(GL_TEXTURE_2D,textur);
//    glBegin(GL_QUADS);
//        glTexCoord2f(TexCoord.a.x,TexCoord.b.y);glVertex2f(display.a.x,display.a.y);
//        glTexCoord2f(TexCoord.a.x,TexCoord.a.y);glVertex2f(display.a.x,display.b.y);
//        glTexCoord2f(TexCoord.b.x,TexCoord.a.y);glVertex2f(display.b.x,display.b.y);
//        glTexCoord2f(TexCoord.b.x,TexCoord.b.y);glVertex2f(display.b.x,display.a.y);
//    glEnd();
//}
//
//
////Übergabeparameter:
////  1.) textur          [GLuint]    Bereits zuvor geladene Textur, die aus mehreren Teilelementen besteht
////  2.) texSize         [POS]       Aus wievielen Teilelementen die Textur besteht (X,Y)
////  3.) texPos          [POS]       An welcher Position sich das gewünschte Element befindet (ACHTUNG: es wird bei 0 losgezählt)
////  4.) display         [AREA]      Bereich, wo das Bild ausgegeben wird
////  (5) overlay         [COLOR]     zum einfärben der Textur
//void putSprite(GLuint textur,POS texSize,POS texPos,AREA display,COLOR overlay=WHITE)//Teil einer Textur ausgeben (Wenn in einer Bilddatei mehrere Teillemente vorhanden sind)
//{
//    switchGraphicMode(TEXTURES);
//    glColor3f(overlay.r,overlay.g,overlay.b);
//    glBindTexture(GL_TEXTURE_2D,textur);
//
//    fPOS partSize={1.0f/texSize.x,1.0f/texSize.y};//Bildbereichsgröße eines Elements
//    glBegin(GL_QUADS);
//double x=0.5/2200,y=0.5/2200;
//        glTexCoord2f(partSize.x*texPos.x    +x ,partSize.y*(texPos.y+1)-y);   glVertex2f(display.a.x,display.a.y);
//        glTexCoord2f(partSize.x*texPos.x    +x ,partSize.y*texPos.y+y);       glVertex2f(display.a.x,display.b.y);
//        glTexCoord2f(partSize.x*(texPos.x+1)-x ,partSize.y*texPos.y+y);       glVertex2f(display.b.x,display.b.y);
//        glTexCoord2f(partSize.x*(texPos.x+1)-x ,partSize.y*(texPos.y+1)-y);   glVertex2f(display.b.x,display.a.y);
//    glEnd();
//}
//
////Übergabeparameter:
////  1.) letter          [char]      Buchtstabe, der ausgegeben werden soll
////  2.) position        [POS]       Position, wo der Buchstabe hingeschrieben werden soll
////  3.) fontSize        [int]       Schriftgröße
////  (4) fontColor       [COLOR]     Schriftfarbe
////Rückgabewert:
////  -)  neue x-Position für das nächste Zeichen
//int putLetter(char letter,POS position,int fontSize,COLOR fontColor=WHITE)//Gibt ein Zeichen am Bildschirm aus (A-Z, a-z, 0-9, Sonderzeichen)
//{   POS mat={-1,-1};//Position in der font-Tabelle
//    for(int y=0;y<11;y++)
//    {   for(int x=0;x<10;x++)
//        {   if(font[y][x].symbol==letter)    //gefunden
//            {   mat.x=x; mat.y=y;
//                y=11;
//                break;
//            }
//        }
//    }
//
//    if(mat.x>=0&&mat.y>=0)//Existiert
//    {   putSprite(*fontTextur,{10,11},mat,{position,{position.x+fontSize*0.775,position.y+fontSize}},fontColor);
//        return position.x+fontSize*0.775*(font[mat.y][mat.x].width+0.05); //Nächste Buchstabenposition
//    }
//    else
//    {   error("graphics.cpp / putLetter()","Unallowed Symbol '%c' - no output\n",letter);
//        return position.x;
//    }
//}





//Übergabeparameter:
//  1.) target              Ob ab jetzt gezeichnet (DRAWING) werden soll, oder ob Texturem (TEXTURES) verwendet werden sollen
void switchGraphicMode(GRAPHICMODES target)//Schaltet den Grafikmodus um (Zeichnen/Texturen)
{
    if(target != graphicMode)//Umschalten notwendig
    {   if(target==DRAWING)
        {   glDisable(GL_TEXTURE_2D);
            graphicMode=DRAWING;
        }else
        {   glEnable(GL_TEXTURE_2D);
            graphicMode=TEXTURES;
        }
    }
}

//Übergabeparameter:
//  1.) gebiet          [AREA]      Gibt das Gebiet an, das umrandet werden soll
//  (2) farbe           [COLOR]     Farbe, die der Strich haben soll
void markArea(AREA gebiet,COLOR farbe=WHITE)//Umrandet ein Gebiet
{   switchGraphicMode(DRAWING);
    glColor3f(farbe.r,farbe.g,farbe.b);
    glBegin(GL_LINE_LOOP);
        glVertex2f(gebiet.a.x,gebiet.a.y);//Links Unten
        glVertex2f(gebiet.b.x,gebiet.a.y);//Rechts Unten
        glVertex2f(gebiet.b.x,gebiet.b.y);//Rechts Oben
        glVertex2f(gebiet.a.x,gebiet.b.y);//Links Oben
    glEnd();
}







void drawBox(AREA gebiet,int borderWidth,int type,COLOR farbe=WHITE)        //Gibt eine Box aus
{   if(ImgDebug)    markArea(gebiet,RED);

    boxTextures.print({{gebiet.a.x,gebiet.b.y-borderWidth},{gebiet.a.x+borderWidth,gebiet.b.y}},{0,type},farbe);  //Ecke links oben
    boxTextures.print({{gebiet.a.x,gebiet.a.y+borderWidth},{gebiet.a.x+borderWidth,gebiet.a.y}},{0,type},farbe);  //Ecke links unten
    boxTextures.print({{gebiet.b.x,gebiet.b.y-borderWidth},{gebiet.b.x-borderWidth,gebiet.b.y}},{0,type},farbe);  //Ecke rechts oben
    boxTextures.print({{gebiet.b.x,gebiet.a.y+borderWidth},{gebiet.b.x-borderWidth,gebiet.a.y}},{0,type},farbe);  //Ecke rechts unten

    boxTextures.print({{gebiet.b.x-borderWidth,gebiet.b.y-borderWidth},{gebiet.a.x+borderWidth,gebiet.b.y}},{1,type},farbe);  //Rand oben
    boxTextures.print({{gebiet.b.x-borderWidth,gebiet.a.y+borderWidth},{gebiet.a.x+borderWidth,gebiet.a.y}},{1,type},farbe);  //Rand unten
    boxTextures.print({{gebiet.a.x,gebiet.a.y+borderWidth},{gebiet.a.x+borderWidth,gebiet.b.y-borderWidth}},{2,type},farbe);  //Rand links
    boxTextures.print({{gebiet.b.x,gebiet.a.y+borderWidth},{gebiet.b.x-borderWidth,gebiet.b.y-borderWidth}},{2,type},farbe);  //Rand rechts

    boxTextures.print({{gebiet.b.x-borderWidth,gebiet.b.y-borderWidth},{gebiet.a.x+borderWidth,gebiet.a.y+borderWidth}},{3,type},farbe);  //Inhalt
}












