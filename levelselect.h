#ifndef LEVELSELECT_H_INCLUDED
#define LEVELSELECT_H_INCLUDED
#include "highscore.h"
#include "levelclass.h"

class LEVELSELECT
{
    public:
        typedef struct str_levelfile
        {
            LEVEL* level;
            int index;
            char name[MAX_PATH];
            char path[MAX_PATH];
            HIGHSCORE score;

            struct str_levelfile *prev;
            struct str_levelfile *next;
        } LEVELFILE;

        LEVELSELECT();
        ~LEVELSELECT();
        static LEVELSELECT* GetCurrent() {if (_instance == NULL) new LEVELSELECT(); return _instance;}

        /*
            false: Auswahl abgebrochen, Rückkehr ins MainMenu
            true: Level ausgewählt, Name in GetLevelName
        */
        int Select();

        //functions for calling levelselect programmaticly
        int GetLevelCount();
        bool SwitchLevel(int jumpWidth);
        bool SelectLevel(int index);
        bool NextLevelAvailable();
        bool PrevLevelAvailable();

        LEVELFILE* GetLevel() {return currentLevel;}
    private:
        bool isInputValid();
        bool GetLevelPath(char* levelPath, const char* levelName, bool appendExtension);
        static bool addLevelListEntry(char* levelFileName);
        static LEVELSELECT* _instance;
        char* levelName;
        LEVELFILE* currentLevel;
        int levelCount;
};

#endif // LEVELSELECT_H_INCLUDED
