#include <stdlib.h>
#include "animation.h"
#include "definitions.h"
#include "display.h"
#include "graphics.h"

ANIMATIONGROUP animationHandler;                                                    //Verwaltet alle Animationen

GRAPHICMODES graphicMode=DRAWING;

TEXTURE boxTextures ("daten/texturen/box.tga", {101,125}, {4,5});                                           //Elemente zur Ausgabe von Boxen
TEXTURE gamelogIcons ("daten/texturen/gamelog.tga", {128,128}, {4,4});                                      //Elemente zur Ausgabe von Boxen
TEXTURE leveltiles ("daten/texturen/leveltiles.tga", {512,512}, {8,8}, false);                              //Dieses Bild ist 512 512 groß und besteht aus 64 Teilbildern (8x8 Teilbilder) --> Jedes Teilbild ist 64x64 Pixel groß
TEXTURE levelanimations     ("daten/texturen/animation/levelanimations.tga"     ,{256,768},{4,12});         //
TEXTURE crashAnimationX     ("daten/texturen/animation/crashAnimationX.tga"     ,{13,128},{1,4});           //Animationsdatei: Falls ein Objekt gegen en Hinderniss stößt (links/rechts)
TEXTURE crashAnimationY     ("daten/texturen/animation/crashAnimationY.tga"     ,{128,13},{4,1});           //Animationsdatei: Falls ein Objekt gegen en Hinderniss stößt (oben/unten)
TEXTURE lavafall            ("daten/texturen/animation/lavafall.tga"            ,{512,205},{5,2});          //Animationsdatei: Wenn ein Objekt in die Lava fällt
TEXTURE gamebackground      ("daten/texturen/gamebg.tga"                        ,{256,256},{1,1},true);     //Hintergrund für das Spiel
TEXTURE shine               ("daten/texturen/shine.tga"                         ,{64,64},{1,1});            //Shein/Leuchten

FONT normalFont(0);            //Ausgabe für Texte: normale Schriftart

POS mouse;                  //Mausposition. Wird automatisch aktualisern (von prepare_graphics())

float coordPixel=1;         //Wieviele Pixel eine Koordinateneinheit am Bildschirm bekommt

int __COLOR = ccLIGHTGRAY | (ccBLACK<<4);

/*const*/ char* const VERSION={"Ver. 0.0"};

/*const*/ POS windowSize={1200,680};      //Fenstergröße - Anzahl der Koordinatenpunkte die erreicht werden können
