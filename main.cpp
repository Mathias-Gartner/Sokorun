//HAUPRPROGRAMM
//KEINE VERSIONSNUMMER
//jeder hat sein eigenes main() damit die Funktionen/Blöcke, die gerade programmiert werden, aufgerufen werden können
//Versionsnummer gibts erst ab dem zusamenstellen von mehreren Blöcken




//Wenn Grafiken und andere Datein nicht gefunden werden: "Execution Working Dir" in den Project-Options editieren



#include "main.h"



TEXTURE leveltiles          ("daten/texturen/leveltiles.tga"                    ,{512,512},{8,8});          //Dieses Bild ist 512 512 groß und besteht aus 64 Teilbildern (8x8 Teilbilder) --> Jedes Teilbild ist 64x64 Pixel groß
TEXTURE levelanimations     ("daten/texturen/animation/levelanimations.tga"     ,{256,768},{4,12});         //
TEXTURE crashAnimationX     ("daten/texturen/animation/crashAnimationX.tga"     ,{13,128},{1,4});           //Animationsdatei: Falls ein Objekt gegen en Hinderniss stößt (links/rechts)
TEXTURE crashAnimationY     ("daten/texturen/animation/crashAnimationY.tga"     ,{128,13},{4,1});           //Animationsdatei: Falls ein Objekt gegen en Hinderniss stößt (oben/unten)
TEXTURE lavafall            ("daten/texturen/animation/lavafall.tga"            ,{512,205},{5,2});          //Animationsdatei: Wenn ein Objekt in die Lava fällt

TEXTURE boxTextures         ("daten/texturen/box.tga"                           ,{103,128},{4,5});          //Elemente zur Ausgabe von Boxen

TEXTURE gamelogIcons        ("daten/texturen/gamelog.tga"                       ,{128,128},{4,4});          //Elemente zur Ausgabe von Boxen

TEXTURE gamebackground      ("daten/texturen/gamebg.tga"                        ,{256,256},{1,1},true);     //Hintergrund für das Spiel
TEXTURE shine               ("daten/texturen/shine.tga"                         ,{64,64},{1,1});            //Shein/Leuchten




//TEXTURE coldAnimation       ("daten/texturen/animation/coldAnimation.tga"       ,{900,272},{5,2});          //Animationsdatei
//TEXTURE lavaAnimation       ("daten/texturen/animation/lavaAnimation.tga"       ,{256,256},{4,4});          //Animationsdatei
//TEXTURE lava                ("daten/texturen/animation/lava.tga"                ,{16,64},{1,4});            //Animationsdatei



///OBJEKTE, DIE NUR EINMAL EXISTIEREN UND ÜBERALL VERWENDET WERDEN:
ANIMATIONGROUP animationHandler;                                                    //Verwaltet alle Animationen
FONT normalFont(0);                                                                 //Ausgabe für Texte: normale Schriftart
POS mouse;                                                                          //Mausposition. Wird automatisch aktualisern (von prepare_graphics())

                                                                       //Zur Verwendung mit der Maus

int main(int argc,char* argv[])
{

    init_window();  //Fenster initialisieren





///VERTEX BUFFER ARRAY - TEST (funktioniert nicht weil keine Extension verwendet wird)
///http://sdickinson.com/wordpress/?p=122
//    #define X_POS 0
//    #define Y_POS 1
//
//    #define U_POS 0
//    #define V_POS 1
//
//    #define R_POS 0
//    #define G_POS 1
//    #define B_POS 2
//
//
//    typedef struct
//    {
//        GLfloat location[2];    //fenster
//        GLfloat tex[2];         //textur
//        //GLFloat normal[3];
//        GLfloat colour[3];      //farbe
//
//        //GLfloat á 4Byte
//
//        GLubyte padding[36];    // Pads the struct out to 64 bytes for performance increase
//    } Vertex;
//
//
//    Vertex verts[4]; // We're making a cube, 1 face * 4 verticies per face
//    GLubyte index[6]; // 2 Triangles per face (possible to use quads, but they're being phased out of OpenGL3, so we're using triangles instead)
//
//
//    verts[0].location[X_POS] = 50; verts[0].location[Y_POS] = 50;
//	verts[0].tex[U_POS] = 0; verts[0].tex[V_POS] = 0;
//
//	verts[1].location[X_POS] = 50; verts[1].location[Y_POS] = 500;
//	verts[1].tex[U_POS] = 0; verts[1].tex[V_POS] = 1;
//
//	verts[2].location[X_POS] = 500;  verts[2].location[Y_POS] = 500;
//	verts[2].tex[U_POS] = 1; verts[2].tex[V_POS] = 1;
//
//	verts[3].location[X_POS] = 500;  verts[3].location[Y_POS] = 50;
//	verts[3].tex[U_POS] = 1; verts[0].tex[V_POS] = 0;
//
//    for (int i = 0; i < 24; i++)
//	{
//		verts[i].colour[R_POS] = 1.0;
//		verts[i].colour[G_POS] = 1.0;
//		verts[i].colour[B_POS] = 1.0;
//	}
//    // 1      2
//	// +------+
//	// |      |
//	// |      |
//	// +------+
//	// 0      3
//
//
//
//	//Triangles:
//        index[0] = 0; index[1] = 1; index[2] = 2;
//        index[3] = 2; index[4] = 3; index[5] = 0; // Repeated number 2 & 0 as they're shared
//
//    /// ... we have to repeat some of the index numbers. This is a good thing as it means that we have some shared verticies, and have to upload less data to the graphics card.
//
//
//
//    // A helper macro to get a position
//    #define BUFFER_OFFSET(i) ((char *)NULL + (i))
//
//    GLuint vboID; // Vertex Buffer, this needs to be accessable wherever we draw from, so in C++, this would be a class member, in regular C, it would probably be a global variable;
//
//    glGenBuffers(1, &vboID); // Create the buffer ID, this is basically the same as generating texture ID's
//    glBindBuffer(GL_ARRAY_BUFFER, vboID); // Bind the buffer (vertex array data)
//
//    // Allocate space.  We could pass the mesh in here (where the NULL is), but it's actually faster to do it as a
//    // seperate step.  We also define it as GL_STATIC_DRAW which means we set the data once, and never
//    // update it.  This is not a strict rule code wise, but gives hints to the driver as to where to store the data
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, NULL, GL_STATIC_DRAW);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, verts); // Actually upload the data
//
//    // Set the pointers to our data.  Except for the normal value (which always has a size of 3), we must pass
//    // the size of the individual component.  ie. A vertex has 3 points (x, y, z), texture coordinates have 2 (u, v) etc.
//    // Basically the arguments are (ignore the first one for the normal pointer), Size (many components to
//    // read), Type (what data type is it), Stride (how far to move forward - in bytes - per vertex) and Offset
//    // (where in the buffer to start reading the data - in bytes)
//
//    // Make sure you put glVertexPointer at the end as there is a lot of work that goes on behind the scenes
//    // with it, and if it's set at the start, it has to do all that work for each gl*Pointer call, rather than once at
//    // the end.
//    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(2*4));
//
//    //glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20));
//    glColorPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(16));
//    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
//
//    // When we get here, all the vertex data is effectively on the card
//
//    // Our Index Buffer, same as above, the variable needs to be accessible wherever we draw
//    GLuint indexVBOID;
//    glGenBuffers(1, &indexVBOID); // Generate buffer
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID); // Bind the element array buffer
//    // Upload the index array, this can be done the same way as above (with NULL as the data, then a
//    // glBufferSubData call, but doing it all at once for convenience)
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLubyte), index, GL_STATIC_DRAW);
//
//
//    ///DATA IS ON THE CARD
//    prepare_GameLoop();                 //Für die Spiel-/Anzeigeschleife vorbereiten
//
//    do
//    {   prepare_graphics();             //Grafiken vorbereiten
//
//
//
//
//        // Bind our buffers much like we would for texturing
//        glBindBuffer(GL_ARRAY_BUFFER, vboID);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);
//
//        // Set the state of what we are drawing (I don't think order matters here, but I like to do it in the same
//        // order I set the pointers
//        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//        glEnableClientState(GL_COLOR_ARRAY);
//        //glEnableClientState(GL_NORMAL_ARRAY);
//        glEnableClientState(GL_VERTEX_ARRAY);
//
//        // Resetup our pointers.  This doesn't reinitialise any data, only how we walk through it
//        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(2*4));
//        //glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20));
//        glColorPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(16));
//        glVertexPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
//
//        // Actually do our drawing, parameters are Primative (Triangles, Quads, Triangle Fans etc), Elements to
//        // draw, Type of each element, Start Offset
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
//
//        // Disable our client state back to normal drawing
//        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//        glDisableClientState(GL_COLOR_ARRAY);
//        //glDisableClientState(GL_NORMAL_ARRAY);
//        glDisableClientState(GL_VERTEX_ARRAY);
//
//
//    }while(complete_graphics(0,10));









///BUTTON-BEISPIELE:
//
// Entweder in gamlog.h und gamelog.cpp nachschauen oder Jakob fragen

////ANIMATIONS-BEISPIELE:
//    animationHandler.add(BASIC,/*T*/0,0, 10 , 90 ,3, 1 ,&leveltiles,{1,1}, {{0,0},{300,300}},BLACK);//Einfärben
//    animationHandler.add(BASIC,1,0,  0,20  ,1,0.3, &leveltiles,{1,1}, {{450,150},{300,0}});//Drehen
//    //animationHandler.add(BASIC,2,0,  0,9  ,0,5, &coldAnimation,{-1,-1}, {{700,0},{700+50*1.323529,50}});//Texturen
//
//    //animationHandler.add(BASIC,2,0,  0,15  ,2,0.3, &lavaAnimation,{-1,-1}, {{800,0},{880,80}});//Texturen
//    //animationHandler.add(BASIC,2,0,  0,3  ,2,1.5, &lava,{-1,-1}, {{880,0},{960,80}});//Texturen
//
//    int id=animationHandler.add(LEVELEFFECT,2,0,  0,3  ,0,4, &crashAnimationX,{-1,-1}, {{1040,0},{1055,80}});//Texturen
//
//
//    int exit=0;
//    prepare_GameLoop();                     //Für die Spiel-/Anzeigeschleife vorbereiten
//    do
//    {   prepare_graphics();                 //Grafiken vorbereiten
//
//
//        animationHandler.run(BASIC);
//
//        if(animationHandler.run(id)) //Fertig
//        {   int x=rand()%windX,y=rand()%windY;
//        srand(clock());
//            id=animationHandler.add(LEVELEFFECT,2,0,  0,3  ,0,4, &crashAnimationX,{-1,-1}, {{x,y},{x+15,y+80}});
////            2. Bild in x-Richtung gespiegelt ausgeben:
//            animationHandler.setSpriteArea(animationHandler.add(BASIC,2,0,  0,3  ,0,4, &crashAnimationX,{-1,-1}, {{x+50,y},{x+65,y+80}}),TextAreaXmirror);
//
//        }
//
//
//        animationHandler.print(0);
//
//
//
//    }while(complete_graphics(0,10) && !exit);    //Abschlussarbeiten und Abbruch-Überprüfung
//    exit=0;






    GAME *game=new GAME({50,50},40,"daten/level/transporter.lvl");
    if(game->getStatus() != 0)
    {   char *dateP,*timeP;
        game->getMetaData(&dateP,&timeP);
        logger(1,"Das Level wurde erfolgreich geladen. Levelpfad: %s, Erstelldatum: %s, Erstellzeitpunkt: %s",game->getLevelPath(),dateP,timeP);
    }

    gameMain(game);

for(;;);
    cleanup();  //Abschlussarbeiten vor dem Programmende
    return 0;

}
