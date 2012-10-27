//HAUPRPROGRAMM
//KEINE VERSIONSNUMMER
//jeder hat sein eigenes main() damit die Funktionen/Blöcke, die gerade programmiert werden, aufgerufen werden können
//Versionsnummer gibts erst ab dem zusamenstellen von mehreren Blöcken




//Wenn Grafiken und andere Datein nicht gefunden werden: "Execution Working Dir" in den Project-Options editieren



#include "main.h"



int main(int argc,char* argv[])
{

    init_window();  //Fenster initialisieren

///---------------------------------------------------------------------
///             Variablendefinitionen und Initialisierungen
///---------------------------------------------------------------------

    long loopStart=clock();             //für die gleichmäßige Geschwindigkeit
    GLuint textur=LoadTexture("maxi.tga");//Ladend er Textur;
    GLuint mainText=LoadTexture("daten/texturen/main.tga");//Laden der Textur;
    GLuint fontText=LoadTexture("daten/texturen/font1.tga");//Laden der Textur;
    POS mouse;
    int x,y;                            //Temp. Variablen, frei verwendbar
    AREA picArea={{500,500},{600,700}};

    setFont(&fontText);                 //Schriftart wählen

     if(DEBUG)  printf("FUER JEDE KONSOLENAUSGABE UND DEBUG-FUNKTION: \"if(DEBUG){ ... }\" verwenden!!!\n");
     if(DEBUG)  printf("FUER WICHTIGE AUSGABEN logger() VERWENDEN, BEI FEHLERN error() VERWENDEN!!!!!!!\n");
///---------------------------------------------------------------------
///---------------------------------------------------------------------

    prepare_GameLoop();                 //Für die Spiel-/Anzeigeschleife vorbereiten
    do
    {
        loopStart=clock();              //Zeitpunkt speichern (für eine gleichmäßige Spielgeschwindigkeit)
        prepare_graphics();             //Grafiken vorbereiten
        getMousePos(&mouse);            //Mausposition erhalten


///####################################################################################################################################################################################################
///             HAUPTSCHLEIFE
///####################################################################################################################################################################################################

        ///POSITION MIT TASTATUR VERÄNDERN:

        if(glfwGetKey(GLFW_KEY_RIGHT))
            picArea.a.x+=10;
        if(glfwGetKey(GLFW_KEY_LEFT))
            picArea.a.x-=10;
        if(glfwGetKey(GLFW_KEY_UP))
            picArea.a.y+=10;
        if(glfwGetKey(GLFW_KEY_DOWN))
            picArea.a.y-=10;
            //Achtung: die Funktion coordInside() unten funktioniert nur, wenn die x und y Koordinaten in a und b sortiert sind
            //          a.x <= b.x    UND   a.y <= b.y


        ///WEISSE LINIE DIE DER MAUS FOLGT
        switchGraphicMode(DRAWING);
        glColor3f(WHITE.r,WHITE.g,WHITE.b);
            glBegin(GL_LINE_STRIP);
                glVertex2f(0,0);
                glVertex2f(mouse.x,mouse.y);
                glVertex2f(windowSize.x,windowSize.y);
            glEnd();


        ///VOLLSTÄNDIGES BILD IN LINKER, UNTERER ECKE
        putImage(textur,{{0,0},{120,120}});

        ///HALBES BILD, RECHTS OBEN
        putImage(textur,picArea,{{0,0},{0.5,1}},RED);

        ///FETTES, WEISSES QUADRAT
        glLineWidth(2);
        markArea({{200,100},{400,300}});

        ///HALBES BILD UMRAHMEN (farbe abhängig, ob sich die Maus innerhalb befindet)
        COLOR farbe;
        if(coordInside(mouse,picArea))  farbe=RED;  //Maus über Bild
        else                            farbe=BLUE;
        markArea(picArea,farbe);    //Halbes Bild umrahmen

        glLineWidth(1);


        ///SPRITES AUSGEBEN:
        putSprite(mainText,{10,10},{1,1},PosSizeToArea({{8*50,9*50},{50,50}}));
        putSprite(mainText,{10,10},{1,0},PosSizeToArea({{9*50,9*50},{50,50}}));
        putSprite(mainText,{10,10},{7,2},PosSizeToArea({{8*50,8*50},{50,50}}));
        putSprite(mainText,{10,10},{6,2},PosSizeToArea({{9*50,8*50},{50,50}}));


        ///GELBE LINIE, DIE DER MAUS FOLGT (wird als letztes ausgegeben --> Über allen anderen Elementen
        switchGraphicMode(DRAWING);
        glColor3f(YELLOW.r,YELLOW.g,YELLOW.b);
            glBegin(GL_LINE_STRIP);
                glVertex2f(0,windY);
                glVertex2f(mouse.x,mouse.y);
                glVertex2f(windX,0);
            glEnd();



    putLetter('A',{0,0},80,WHITE);
    putString("abcdefghijklmnopqrstuvwxyz",{0,windY-100},50,BLUE);
    putString("ABCDEFGHIJKLMNOPQRSTUVWXYZ",{0,windY-150},50,RED);
    putString("und das o! l? er. k, k:u; --> \"und ' (nicht) test! &%@\"",{0,windY-200},50,{1,0.5,0});
    putString("<>\\//@€+*~'[]()|^?_,0123456789",{0,windY-250},50,BLUE);

    putString("Bei einem Fehler \"error()\" aufrufen!",{0,windY-540},30,RED);
    putString("Bei einer Satus-Ausgabe in der Konsole: \"logger()\" verwenden!",{0,windY-570},30,RED);

    if(glfwGetMouseButton(0))   putString("Linke Maustaste"    ,{0      ,windY-50},50,YELLOW,taLEFT);
    if(glfwGetMouseButton(2))   putString("Mittlere Maustaste" ,{windX/2,windY-50},50,YELLOW,taCENTER);
    if(glfwGetMouseButton(1))   putString("Rechte Maustaste"   ,{windX  ,windY-50},50,YELLOW,taRIGHT);

    gprintf({windX/2,(windY-25)/2},100,WHITE,taCENTER,"(%d | %d)",mouse.x,mouse.y);
    printf("(%d | %d)            \r",mouse.x,mouse.y);
///####################################################################################################################################################################################################
///####################################################################################################################################################################################################

    }while(complete_graphics(loopStart,10));    //Abschlussarbeiten und Abbruch-Überprüfung

    cleanup();  //Abschlussarbeiten vor dem Programmende
    return 0;

}
