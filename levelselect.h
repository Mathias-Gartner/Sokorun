#ifndef LEVELSELECT_H_INCLUDED
#define LEVELSELECT_H_INCLUDED
#include "highscore.h"
#include "levelclass.h"

class LEVELSELECT
{
    public:
        typedef struct str_leveldirectory
        {
            int index;
            char name[MAX_PATH];
            char path[MAX_PATH];

            struct str_leveldirectory *prev;
            struct str_leveldirectory *next;
        } LEVELDIRECTORY;

        typedef struct str_levelfile
        {
            LEVEL* level;
            LEVELDIRECTORY* directory;
            int index;
            char name[MAX_PATH];
            char path[MAX_PATH];
            HIGHSCORE score;

            struct str_levelfile *prev;
            struct str_levelfile *next;
        } LEVELFILE;

        LEVELSELECT();
        ~LEVELSELECT();
        static LEVELSELECT* GetCurrent() { if (_instance == NULL) new LEVELSELECT(); return _instance; }

        /*
            Sets Levelselect interactive. Blocking call.
            false: Selection canceled, return to MainMenu
            true: Level selected, name in GetLevelName
        */
        int Select();

        //functions for calling levelselect programmaticly
        bool SwitchLevel(int offset);
        bool SelectLevel(int index);
        bool NextLevelAvailable();
        bool PrevLevelAvailable();
        bool SwitchDirectory(int offset);
        bool SelectDirectory(int index);
        bool NextDirectoryAvailable();
        bool PrevDirectoryAvailable();

        LEVELFILE* GetLevel() { return currentLevel; }
        LEVELDIRECTORY* GetDirectory() { return currentDirectory; }
        int GetLevelCount() { return levelCount; }
        int GetDirectoryCount() { return directoryCount; }
    private:
        static bool addLevelListEntry(char* levelFileName);
        static bool addDirectoryListEntry(char* directoryName);
        static LEVELSELECT* _instance;

        bool isInputValid();
        void GetLevelPath(char* levelPath, const LEVELFILE* level);
        bool GetDirectoryPath(char* directoryPath, const char* directoryName);
        void LoadDirectory();
        void FreeLevelList(LEVELFILE* listEntry);
        void FreeDirectoryList(LEVELDIRECTORY* listEntry);

        char* levelName;
        LEVELFILE* currentLevel;
        LEVELDIRECTORY* currentDirectory;
        int levelCount;
        int directoryCount;
};

#endif // LEVELSELECT_H_INCLUDED
