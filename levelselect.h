#ifndef LEVELSELECT_H_INCLUDED
#define LEVELSELECT_H_INCLUDED
#include "highscore.h"
#include "levelclass.h"

class LEVELSELECT
{
    public:
        LEVELSELECT();
        ~LEVELSELECT();
        /*
            false: Auswahl abgebrochen, Rückkehr ins MainMenu
            true: Level ausgewählt, Name in GetLevelName
        */
        int Select();
        void SwitchLevel(int jumpWidth);
        // Objects received by GetLevel() will be destroyed when SwitchLevel() is called.
        LEVEL* GetLevel() {return currentLevel;}
        char* GetLevelName() {return levelName;}
        char* GetLevelPath() {return GetLevelPath(GetLevelName(), true);}
        HIGHSCORE* getCurrentLevelScore() {return currentLevelScore;}
    private:
        bool isInputValid();
        char* GetLevelPath(const char* levelName, bool appendExtension);
        char* levelName;
        char **levels;
        char levelPath[MAX_PATH];
        int levelCount;
        LEVEL *currentLevel;
        int currentLevelIndex;
        HIGHSCORE *currentLevelScore;
};

#endif // LEVELSELECT_H_INCLUDED
