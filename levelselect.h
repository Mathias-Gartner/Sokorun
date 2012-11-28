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
        int GetStatus() {return status;}
        char* GetLevelName() {return levelName;}
        char* GetLevelPath() {return GetLevelPath(GetLevelName(), true);}
        HIGHSCORE* getCurrentLevelScore() {return currentLevelScore;}
    private:
        bool isInputValid();
        void switchLevel(int jumpWidth);
        char* GetLevelPath(const char* levelName, bool appendExtension);
        char* levelName;
        char **levels;
        char levelPath[MAX_PATH];
        int levelCount;
        LEVEL *currentLevel;
        int currentLevelIndex;
        HIGHSCORE *currentLevelScore;
        int status;

};

#endif // LEVELSELECT_H_INCLUDED
