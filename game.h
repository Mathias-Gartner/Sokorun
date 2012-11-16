#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED


extern TEXTURE gamebackground,shine;



class GAMEBACKGROUND
{
    private:
        AREA area;              //Soll-Ausgabefläche


        float alpha;

        int colorProgress;      //Zu wieviel % die neue Farbe g=nommen wird

        COLOR old;              //Letze Farbe
        COLOR final;            //Soll-Farbe

        COLOR splash;           //Kurzzeitige Soll-Farbe
        //bool splashEnabled;     //1: Die Splashfarbe=Zielfarbe
        int splashProgress;     //1-100% - zu wieviel % die Splash-Farbe eingeblendet werden soll; 0=keine Splahfarbe
        bool splashDirection;   //0: Die Splashfarbe wird eingeblendet; 1: Die Splashfarbe wird ausgeblendet

        //bool colorVariation;     //0: Ziel = target-Farbe    1: Ziel = abgedunkelte Target-Farbe


    public:
        GAMEBACKGROUND(AREA _area);

        void print(bool printshine=1);
        void setColor(COLOR _target);           //Neue, endgültige Farbe wählen
        void setSplashColor(COLOR _splash);     //Farbe einstellen, die kurzfristig verwendet werden soll
};

#endif // GAME_H_INCLUDED
