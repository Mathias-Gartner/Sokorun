#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "animation.h"
#include "graphics.h"

/** TYPE DEFINITIONS **/

    extern double FPS;                              //Die FPS des letzen Intervalls
    extern long frameArray[frameArraySize+1];       //Ringbuffer; Beinhaltet die Abschlusszeiten der letzten "frameArraySize" Frames
    extern int frameArrayIndex;                     //Zeigt auf den neusten Eintrag
    extern int lastFrameArrayIndex;                 //Zeigt auf den ältesten Eintrag




extern ANIMATIONGROUP animationHandler;         //Verwaltet alle Animationen

extern GRAPHICMODES graphicMode;

extern TEXTURE boxTextures;                     //Elemente zur Ausgabe von Boxen
extern TEXTURE gamelogIcons;                    //Elemente zur Ausgabe von Boxen
extern TEXTURE leveltiles;                      //Dieses Bild ist 512 512 groß und besteht aus 64 Teilbildern (8x8 Teilbilder) --> Jedes Teilbild ist 64x64 Pixel groß
extern TEXTURE levelanimations;                 //
extern TEXTURE crashAnimationX;                 //Animationsdatei: Falls ein Objekt gegen en Hinderniss stößt (links/rechts)
extern TEXTURE crashAnimationY;                 //Animationsdatei: Falls ein Objekt gegen en Hinderniss stößt (oben/unten)
extern TEXTURE lavafall;                        //Animationsdatei: Wenn ein Objekt in die Lava fällt
extern TEXTURE gamebackground;                  //Hintergrund für das Spiel
extern TEXTURE shine;                           //Shein/Leuchten
extern TEXTURE lavaflow;                        //Animationsdatei: Bewegung von Lava
extern TEXTURE lavablubb;                       //Animationsdatei: Lavablase

extern FONT normalFont;            //Ausgabe für Texte: normale Schriftart

extern POS mouse;                  //Mausposition. Wird automatisch aktualisern (von prepare_graphics())

extern float coordPixel;         //Wieviele Pixel eine Koordinateneinheit am Bildschirm bekommt

extern int __COLOR;

extern /*const*/ char* const VERSION;

extern /*const*/ POS windowSize;      //Fenstergröße - Anzahl der Koordinatenpunkte die erreicht werden können

#endif // GLOBALS_H_INCLUDED
