//Version 0.1
//Jakob Maier
//Grafikfunktionen

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
        }
        coordPixel=(float)width/windowSize.x;
    }

    height = height > 0 ? height : 1;
    glViewport( 0, 0, width, height );              //set Origin (außerhalb kann nicht gezeichnet werden)
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );         //Hintergrundfarbe
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();                               //auf Standard-Matrix umschalten
    glOrtho(0,windowSize.x,0,windowSize.y,0,128);   //2D-Modus; z-Koordinate wird nicht verwendet
    glMatrixMode(GL_MODELVIEW);
}


//Übergabeparameter:
//  1.) loopStart           [time]  Zeitpunkt, zu dem die Spiele- und Anzeigeschleife in diesen Durchgang gestartet ist
//  (2) loopSpeed           [ms]    Wielange ein Schleifendurchgang dauern muss
//Rückgabewert:
//  -)  Abbruchbedingung    [bool]  Ob die Schleife abgebrochen werden soll (false) oder weiterlaufen darf (true)
int complete_graphics(long loopStart,unsigned int loopSpeed=10)//Wird am Ende jedes Durchgangs in der Spiele- und Anzeigeschleife aufgerufen
{   if(kbhit()){getch();system("CLS");}             //Ein Tastendruck in der Konsole führt zum löschen des Inhalts

    while(clock()-loopStart<loopSpeed){}            //gleichmäßige Geschwindigkeit
    glfwSwapBuffers();                              //erzeugte Grafikdaten ausgeben

    return (!glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED));//Abbruchbedingung
}

void cleanup()//Wird beim beenden des Programms aufgerufen
{
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE_2D);
    glfwTerminate();            //Fensters schließen
}

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


//Übergabeparameter:
//  1.) *TexName            [string]    Pfad für die Bilddatei (.tga)
GLuint LoadTexture(char *TexName)//UP für das laden von Tga datein
{
    TGAImg Img;        // Image loader
    GLuint Texture;

    // Load our Texture
    if(Img.Load(TexName)!=IMG_OK)
    return -1;

    glGenTextures(1,&Texture);            // Allocate space for texture
    glBindTexture(GL_TEXTURE_2D,Texture); // Set our Tex handle as current

    // Create the texture
    if(Img.GetBPP()==24)
        glTexImage2D(GL_TEXTURE_2D,0,3,Img.GetWidth(),Img.GetHeight(),0,
                    GL_RGB,GL_UNSIGNED_BYTE,Img.GetImg());
    else if(Img.GetBPP()==32)
        glTexImage2D(GL_TEXTURE_2D,0,4,Img.GetWidth(),Img.GetHeight(),0,
                    GL_RGBA,GL_UNSIGNED_BYTE,Img.GetImg());
    else
        return -1;

    // Specify filtering and edge actions
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//Wenn das Image > als der Screen ist (mehrere Image-Pixel = ein Screen-Pixel)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//ein Image-Pixel = mehrere Screen-Pixel
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

    return Texture;
}


//Übergabeparameter:
//  1.) textur          [GLuint]    Bereits zuvor geladene Textur
//  2.) display         [AREA]      Koordinate a = linke,untere Position des Bildes
//                                  Koordinate b = rechte, obere Position des Bildes
//  (3) TexCoord        [AREA]      Zum zuschneiden des Bildes (Wertebereich 0.0f - 1.0f)
//  (4) overlay         [COLOR]     zum einfärben der Textur
void putImage(GLuint textur,AREA display,fAREA TexCoord=stdTextArea,COLOR overlay=WHITE)//Textur ausgeben
{
    switchGraphicMode(TEXTURES);
    glColor3f(overlay.r,overlay.g,overlay.b);
    glBindTexture(GL_TEXTURE_2D,textur);
    glBegin(GL_QUADS);
        glTexCoord2f(TexCoord.a.x,TexCoord.b.y);glVertex2f(display.a.x,display.a.y);
        glTexCoord2f(TexCoord.a.x,TexCoord.a.y);glVertex2f(display.a.x,display.b.y);
        glTexCoord2f(TexCoord.b.x,TexCoord.a.y);glVertex2f(display.b.x,display.b.y);
        glTexCoord2f(TexCoord.b.x,TexCoord.b.y);glVertex2f(display.b.x,display.a.y);
    glEnd();
}


//Übergabeparameter:
//  1.) textur          [GLuint]    Bereits zuvor geladene Textur, die aus mehreren Teilelementen besteht
//  2.) texSize         [POS]       Aus wievielen Teilelementen die Textur besteht (X,Y)
//  3.) texPos          [POS]       An welcher Position sich das gewünschte Element befindet (ACHTUNG: es wird bei 0 losgezählt)
//  4.) display         [AREA]      Bereich, wo das Bild ausgegeben wird
//  (5) overlay         [COLOR]     zum einfärben der Textur
void putSprite(GLuint textur,POS texSize,POS texPos,AREA display,COLOR overlay=WHITE)//Teil einer Textur ausgeben (Wenn in einer Bilddatei mehrere Teillemente vorhanden sind)
{
    switchGraphicMode(TEXTURES);
    glColor3f(overlay.r,overlay.g,overlay.b);
    glBindTexture(GL_TEXTURE_2D,textur);

    fPOS partSize={1.0f/texSize.x,1.0f/texSize.y};//Bildbereichsgröße eines Elements
    glBegin(GL_QUADS);
        glTexCoord2f(partSize.x*texPos.x     ,partSize.y*(texPos.y+1));   glVertex2f(display.a.x,display.a.y);
        glTexCoord2f(partSize.x*texPos.x     ,partSize.y*texPos.y);       glVertex2f(display.a.x,display.b.y);
        glTexCoord2f(partSize.x*(texPos.x+1) ,partSize.y*texPos.y);       glVertex2f(display.b.x,display.b.y);
        glTexCoord2f(partSize.x*(texPos.x+1) ,partSize.y*(texPos.y+1));   glVertex2f(display.b.x,display.a.y);
    glEnd();
}

//Übergabeparameter:
//  1.) letter          [char]      Buchtstabe, der ausgegeben werden soll
//  2.) position        [POS]       Position, wo der Buchstabe hingeschrieben werden soll
//  3.) fontSize        [int]       Schriftgröße
//  (4) fontColor       [COLOR]     Schriftfarbe
//Rückgabewert:
//  -)  neue x-Position für das nächste Zeichen
int putLetter(char letter,POS position,int fontSize,COLOR fontColor=WHITE)//Gibt ein Zeichen am Bildschirm aus (A-Z, a-z, 0-9, Sonderzeichen)
{   POS mat={-1,-1};//Position in der font-Tabelle
    for(int y=0;y<11;y++)
    {   for(int x=0;x<10;x++)
        {   if(font[y][x].symbol==letter)    //gefunden
            {   mat.x=x; mat.y=y;
                y=11;
                break;
            }
        }
    }

    if(mat.x>=0&&mat.y>=0)//Existiert
    {   putSprite(*fontTextur,{10,11},mat,{position,{position.x+fontSize*0.775,position.y+fontSize}},fontColor);
        return position.x+fontSize*0.775*(font[mat.y][mat.x].width+0.05); //Nächste Buchstabenposition
    }
    else
    {   error("graphics.cpp / putLetter()","Unallowed Symbol '%c' - no output\n",letter);
        return position.x;
    }
}


//Übergabeparameter:
//  1.) text            [string]    Text, dessen Platzverbrauch berechnet werden soll
//  2.) fontSize        [int]       Schriftgröße
//Rückgabewert:
//  -)  Platzverbrauch in x-Richtung
int getFontWidth(const char *text,int fontSize)//Gibt zurück, wieviel Platz ein Text in x-Richtung am Bildschirm brauchen würde (zB. für Zentrierte und Rechtsbündige Texte)
{   int width=0;
    while(*text!=0)
    {   for(int y=0;y<11;y++)
        {   for(int x=0;x<10;x++)
            {   if(font[y][x].symbol==*text)    //gefunden
                {   width+=fontSize*0.775*(font[y][x].width+0.05);
                    y=11;
                    break;
                }
            }
        }
        text++;
    }
    return width;
}


//Übergabeparameter:
//  1.) text            [string]    Text, der ausgegeben werden soll
//  2.) position        [POS]       Position, wo der Text hingeschrieben werden soll
//  3.) fontSize        [int]       Schriftgröße
//  (4) fontColor       [COLOR]     Schriftfarbe
//  (5) ausrichtung     [ALIGN]     Ausrichtung (taLEFT, taCENTER, taRIGHT)     (ta=text-align)
//Rückgabewert:
//  -)  neue x-Position für das nächste Zeichen/den nächsten Text
int putString(const char *text,POS position,int fontSize,COLOR fontColor=WHITE,TEXTALIGN ausrichtung=taLEFT)//Gibt einen Text aus
{
    //markArea({position,{(position.x+getFontWidth(text,fontSize)),position.y+2}},MAGENTA);

    if(ausrichtung==taCENTER)   position.x-=getFontWidth(text,fontSize)/2;
    if(ausrichtung==taRIGHT)    position.x-=getFontWidth(text,fontSize);

    while(*text!=0)
    {   position.x=putLetter(*text,position,fontSize,fontColor);
        text++;
    }
    return position.x;
}


//Übergabeparameter:
//  1.) position        [POS]       Position, wo der Text hingeschrieben werden soll
//  2.) fontSize        [int]       Schriftgröße
//  3.) fontColor       [COLOR]     Schriftfarbe
//  4.) ausrichtung     [ALIGN]     Ausrichtung (taLEFT, taCENTER, taRIGHT)     (ta=text-align)
//  5.) Formatierter String
//  6 - n)  Zusätzliche Daten
//Rückgabewert:
//  -)  neue x-Position für das nächste Zeichen/den nächsten Text
int gprintf(POS position,int fontSize,COLOR fontColor,TEXTALIGN ausrichtung,const char *format,...)//Printf für Grafikfenster - ACHTUNG: max. Textlänge!
{
	static char buffer[GPRINTF_BUFFER+1];											//Buffer
	va_list  argptr;																//Argument-Liste
	va_start( argptr, format );
	vsprintf( buffer, format, argptr );												//Mit sprintf-Funktion in Buffer übertragen
	va_end  ( argptr );
	buffer[GPRINTF_BUFFER]=0;														//Zur Sicherheit
	return putString(buffer,position,fontSize,fontColor,ausrichtung);
}
//Übergabeparameter:
//  1.) textur        [GLuint*]       Adresse der Textur, in der sich die Font-Bilddaten befinden (muss bereits geladen sein)
void setFont(GLuint *textur)                                                        //Stellt die Schriftart ein
{   fontTextur=textur;
}
