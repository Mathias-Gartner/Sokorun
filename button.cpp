//Zur Verwendung von Buttons
//Version 1.0
//Jakob Maier

#include <GL/glfw.h>
#include <stdio.h>
#include <strings.h>
#include "button.h"
#include "definitions.h"
#include "graphics.h"
#include "globals.h"
#include "logger.h"


void getMousePos(POS *coord)    //Gibt die Position der Maus im Koordinatensystem zurück
{   glfwGetMousePos(&(coord->x),&(coord->y));
    //Koordinatensystem umrechnen:
    if(coordPixel==0)           //Fenster=minimiert
    {   *coord={-1,-1};
        return;
    }
    coord->x/=coordPixel;
    coord->y/=coordPixel;
    transformY(coord);
}

BUTTON::BUTTON(AREA _output,char _clickable)              //Typ 0 initialisieren
{
    clickable=_clickable;
    for(int mt=0;mt<3;mt++)                                 //Keine Tastaturtasten zuweisen
    {   keyboardButton[0][mt]=0;
        keyboardButton[1][mt]=0;
    }
    type=0;                                                             //Keine Ausgabe
}

BUTTON::BUTTON(AREA _output,char _clickable,TEXTURE *_textur,POS _spritePos)    //Typ 1 initialisieren
{
    active=0;
    output=_output;
    clickable=_clickable;
    for(int mt=0;mt<3;mt++)                                 //Keine Tastaturtasten zuweisen
    {   keyboardButton[0][mt]=0;
        keyboardButton[1][mt]=0;
    }
    textur=_textur;
    color={0.7,0.7,0.7};
    spritePos=_spritePos;
    type=1;                                                             //Textur
}

BUTTON::BUTTON(AREA _output,char _clickable,TEXTURE *_textur,POS _spritePos,const char* const value,int _fontSize,COLOR _fontcolor)    //Typ 2 initialisieren
{
    active=0;
    output=_output;
    clickable=_clickable;
    for(int mt=0;mt<3;mt++)                                 //Keine Tastaturtasten zuweisen
    {   keyboardButton[0][mt]=0;
        keyboardButton[1][mt]=0;
    }
    textur=_textur;
    color={0.7,0.7,0.7};
    spritePos=_spritePos;
    if(strlen(value)>127)//bufoverflow vermeiden
            error("BUTTON::BUTTON()","Der uebergebene String ist zu lange. Max. Laenge=126+1 Zeichen");
    else    strcpy(buttonText,value);
    fontcolor=_fontcolor;
    fontSize=_fontSize;
    type=2;                                                             //Textur+Text
}

BUTTON::BUTTON(AREA _output,char _clickable,int _boxtype,int _borderWidth,COLOR _color)              //Typ 3 initialisieren
{
    active=0;
    output=_output;
    clickable=_clickable;
    for(int mt=0;mt<3;mt++)                                 //Keine Tastaturtasten zuweisen
    {   keyboardButton[0][mt]=0;
        keyboardButton[1][mt]=0;
    }
    boxtype=_boxtype;
    borderWidth=_borderWidth;
    color=_color;
    type=3;                                                             //Box
}

BUTTON::BUTTON(AREA _output,char _clickable,int _boxtype,int _borderWidth,COLOR _color,const char* const value,int _fontSize,COLOR _fontcolor)              //Typ 4 initialisieren
{
    active=0;
    output=_output;
    clickable=_clickable;
    for(int mt=0;mt<3;mt++)                                 //Keine Tastaturtasten zuweisen
    {   keyboardButton[0][mt]=0;
        keyboardButton[1][mt]=0;
    }
    boxtype=_boxtype;
    borderWidth=_borderWidth;
    color=_color;
    if(strlen(value)>127)//bufoverflow vermeiden
            error("BUTTON::BUTTON()","Der uebergebene String ist zu lange. Max. Laenge=126+1 Zeichen");
    else    strcpy(buttonText,value);
    fontcolor=_fontcolor;
    fontSize=_fontSize;
    type=4;                                                             //Box+Text
}

void BUTTON::assignKeyboardButton(int mousebutton,int keyboard1,int keyboard2)  //Weist Tastaturtasten zu einem Button zu
{
    if(mousebutton<0||mousebutton>2)
    {   error("BUTTON::assignKeyboardButton()","Es wurde ein ungueltiger Wert fuer die Maustaste uebergeben. Der Aufruf wird ignoriert. mousbutton: %d (erlaubt: 0-2)",mousebutton);
        return;
    }

    keyboardButton[0][mousebutton]=keyboard1;
    keyboardButton[1][mousebutton]=keyboard2;
}

char BUTTON::clicked()                                                   //Gibt zurück ob, und welche Maustatste diesen Button geklickt hat (muss VOR pint() aufgerufen werden)
{
    if(!glfwGetMouseButton(0) && !glfwGetKey(keyboardButton[0][0]) && !glfwGetKey(keyboardButton[1][0]) && active==1)//Links
        return 1;
    if(!glfwGetMouseButton(1) && !glfwGetKey(keyboardButton[0][1]) && !glfwGetKey(keyboardButton[1][1]) && active==2)//Rechts
        return 2;
    if(!glfwGetMouseButton(2) && !glfwGetKey(keyboardButton[0][2]) && !glfwGetKey(keyboardButton[1][2]) && active==3)//Mitte
        return 3;
    return 0;


    /*for(int i=0;i<3;i++)//Links - Mitte - Rechts
    {   if(!(glfwGetMouseButton(i)||glfwGetKey(keyboardButton[0][i])||glfwGetKey(keyboardButton[1][i])) && active==i)
        {printf("CLICKED\n");
             return i;
        }
    }*/

    return 0;
}

void BUTTON::print(bool SimulateOnly)                                                    //Ausgabe
{
    COLOR farbe=color;
    active=0;

    if(coordInside(mouse,output) && !SimulateOnly)
    {   if(glfwGetMouseButton(0) && (clickable==0 || clickable == 3 ||clickable==4 ||clickable==6))//Links
        {   farbe=GREEN;    active=1;
        }else
        if(glfwGetMouseButton(1) && (clickable==1 || clickable == 3 ||clickable==5 ||clickable==6))//Rechts
        {   farbe=BLUE;     active=2;
        }else
        if(glfwGetMouseButton(2) && (clickable==2 || clickable == 4 ||clickable==5 ||clickable==6))//Mitte
        {   farbe=CYAN;     active=3;
        }else
        {   farbe=WHITE;
        }
    }
    if(active==0)
    {   if((glfwGetKey(keyboardButton[0][0]) || glfwGetKey(keyboardButton[1][0])) && (clickable==0 || clickable == 3 ||clickable==4 ||clickable==6))
        {   farbe=GREEN;   active=1;
        }else
        if((glfwGetKey(keyboardButton[0][1]) || glfwGetKey(keyboardButton[1][1])) && (clickable==1 || clickable == 3 ||clickable==5 ||clickable==6))
        {   farbe=BLUE;    active=2;
        }else
        if((glfwGetKey(keyboardButton[0][2]) || glfwGetKey(keyboardButton[1][2])) && (clickable==2 || clickable == 4 ||clickable==5 ||clickable==6))
        {   farbe=CYAN;    active=3;
        }
    }

    if(SimulateOnly)    return; //Den Button nicht ausgeben

    switch(type)
    {   case 0: return;
        case 1: textur->print(output,spritePos,farbe); break;
        case 2: normalFont.setFontColor(fontcolor);
                normalFont.setFontSize(fontSize);
                textur->print(output,spritePos,farbe);
                normalFont.putString(buttonText,{output.a.x+(output.b.x-output.a.x)/2 , output.a.y + (output.b.y-output.a.y-fontSize)/2},taCENTER); break;
        case 3: drawBox(output,borderWidth,boxtype,farbe); break;
        case 4: drawBox(output,borderWidth,boxtype,farbe);
                normalFont.setFontColor(fontcolor);
                normalFont.setFontSize(fontSize);
                normalFont.putString(buttonText,{output.a.x+(output.b.x-output.a.x)/2 , output.a.y + (output.b.y-output.a.y-fontSize)/2},taCENTER); break;

    }
}

