//Zur Verwendung von Buttons
//Version 1.0
//Jakob Maier

#include <GL/glfw.h>
#include <strings.h>
#include "button.h"
#include "definitions.h"
#include "graphics.h"
#include "globals.h"
#include "logger.h"


void getMousePos(POS *coord)      //Gibt die Position der Maus im Koordinatensystem zurück
{   glfwGetMousePos(&(coord->x),&(coord->y));
    //Koordinatensystem umrechnen:
    coord->x/=coordPixel;
    coord->y/=coordPixel;
    transformY(coord);
}

//MOUSE::MOUSE()
//{   buttonID[0]=BUTTON_NO_BUTTON;   //links
//    buttonID[1]=BUTTON_NO_BUTTON;   //rechts
//    buttonID[2]=BUTTON_NO_BUTTON;   //mitte
//    clickedButtonID[0]=BUTTON_NO_BUTTON;
//    clickedButtonID[1]=BUTTON_NO_BUTTON;
//    clickedButtonID[2]=BUTTON_NO_BUTTON;
//
//    updateMousePos(&position);            //Mausposition erhalten
//}
//
//void MOUSE::updateMousePos(POS *coord)      //Gibt die Position der Maus im Koordinatensystem zurück
//{   glfwGetMousePos(&(coord->x),&(coord->y));
//    //Koordinatensystem umrechnen:
//    coord->x/=coordPixel;
//    coord->y/=coordPixel;
//    transformY(coord);
//}
//
//POS MOUSE::getMousePos()                    //Gibt die Mausposition zurück
//{
//    return position;
//}
//
//void MOUSE::run()                           //Muss am Anfang der Schleife aufgerufen werden, wird von prepare_graphics(); automatisch erledigt
//{   updateMousePos(&position);              //Mausposition erhalten
//    if(!glfwGetMouseButton(MBLEFT))     {   clickedButtonID[0]=buttonID[0];  buttonID[0]=BUTTON_NO_BUTTON; }   else clickedButtonID[0]=BUTTON_NO_BUTTON;
//    if(!glfwGetMouseButton(MBRIGHT))    {   clickedButtonID[1]=buttonID[1];  buttonID[1]=BUTTON_NO_BUTTON; }   else clickedButtonID[1]=BUTTON_NO_BUTTON;
//    if(!glfwGetMouseButton(MBMIDDLE))   {   clickedButtonID[2]=buttonID[2];  buttonID[2]=BUTTON_NO_BUTTON; }   else clickedButtonID[2]=BUTTON_NO_BUTTON;
//}
//
//bool MOUSE::isHover(BUTTONID _buttonID,const AREA bereich)      //Gibt zurück, ob sich die Maus über einem Button befindet
//{   if(coordInside(position,bereich))   return 1;
//    return 0;
//}
//
//bool MOUSE::isActive(BUTTONID _buttonID,const AREA bereich,MOUSEBUTTON mousebutton)     //Gibt zurück, ob sich die Maus MIT GEDRÜCKTER MAUSTASTE über einem Button befindet
//{   if(coordInside(position,bereich) && glfwGetMouseButton(mousebutton))
//    {   if(buttonID[mousebutton]==BUTTON_NO_BUTTON)//kein Button
//            buttonID[mousebutton]=_buttonID;
//        return 1;
//    }
//    return 0;
//}
//bool MOUSE::isClicked(BUTTONID _buttonID,MOUSEBUTTON mousebutton)    //Gibt zurück, ob der Button geklickt wurde (Maustaste losgelassen)
//{   if(clickedButtonID[mousebutton]==_buttonID)
//        return 1;
//    return 0;
//}
//
//int MOUSE::getButtonState(BUTTONID _buttonID,const AREA bereich,MOUSEBUTTON mousebutton)
//{
//    if(isClicked(_buttonID,mousebutton))  return 3;
//    if(isActive(_buttonID,bereich,mousebutton))  return 2;
//    if(isHover(_buttonID,bereich))  return 1;
//    return 0;
//}
//
//
//COLOR getButtonOverlayColor(BUTTONID buttonID,AREA gebiet,BUTTONID mousebutton)         //Gibt eine Overlay-Farbe für einen Button zurück
//{   switch(mousebutton)
//    {   case MBLEFT:        if(mouse.isClicked(buttonID,MBLEFT)==2)
//                            {   return {1.0,0.5,0.2}; break;            //tastenclick
//                            }break;
//
//        case MBRIGHT:       if(mouse.isClicked(buttonID,MBRIGHT)==2)
//                            {   return {1.0,0.5,0.2}; break;            //tastenclick
//                            }break;
//
//        case MBMIDDLE:      if(mouse.isClicked(buttonID,MBRIGHT)==2)
//                            {   return {0.2,0.5,1.0}; break;            //tastenclick
//                            }break;
//
//        default: error("getButtonOverlayColor()","Unbekannter Button-Typ übergeben. mousebutton=%d",mousebutton);
//    }
//    if(mouse.isHover(buttonID,gebiet))                                  //Hover
//        return {1,1,1};
//
//    return {0.7,0.7,0.7};                                               //nichts
//}
//
//
//
//


BUTTON::BUTTON(AREA _output,char _clickable)              //Typ 0 initialisieren
{
    clickable=_clickable;
    type=0;                                                             //Keine Ausgabe
}

BUTTON::BUTTON(AREA _output,char _clickable,TEXTURE *_textur,POS _spritePos)    //Typ 1 initialisieren
{
    active=0;
    output=_output;
    clickable=_clickable;
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

char BUTTON::clicked()                                                   //Gibt zurück ob, und welche Maustatste diesen Button geklickt hat (muss VOR pint() aufgerufen werden)
{   if(!glfwGetMouseButton(0) && active==1)//Links
        return 1;
    if(!glfwGetMouseButton(1) && active==2)//Rechts
        return 2;
    if(!glfwGetMouseButton(2) && active==3)//Mitte
        return 3;
    return 0;
}

void BUTTON::print()                                                    //Ausgabe
{
    COLOR farbe=color;

    if(coordInside(mouse,output))
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
            active=0;
        }
    }else
    {   active=0;
    }
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

