//Version 0.0
//Mathias Gartner
//Levelauswahl

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "button.h"
#include "definitions.h"
#include "filesystemutility.h"
#include "globals.h"
#include "graphics.h"
#include "highscore.h"
#include "levelclass.h"
#include "levelselect.h"
#include "logger.h"

bool strEndsWith(char* string, const char* suffix);

LEVELSELECT* LEVELSELECT::_instance = NULL;

LEVELSELECT::LEVELSELECT()
{
    if (_instance != NULL)
    {
        delete _instance;
        _instance = NULL;
    }
    _instance = this;

    currentLevel = NULL;
    currentDirectory = NULL;

    char levelPath[MAX_PATH];
    GetDirectoryPath(levelPath, "");

    if (FILESYSTEMUTILITY::EnumerateDirectories(levelPath, &directoryCount, &addDirectoryListEntry))
    {
        SwitchDirectory(0);
    }
    else
    {
        error("LEVELSELECT::ctor()", "FATAL: FILESYSTEMUTILITY::EnumerateDirectories() failed");
        exit(1);
    }
}

LEVELSELECT::~LEVELSELECT()
{
    FreeLevelList(currentLevel);
    FreeDirectoryList(currentDirectory);
}

int LEVELSELECT::Select()
{
    const char noDirectoryName[] = "[kein Verzeichnis]";

    BUTTON prevDirButton ({{10, windY - 100}, {90, windY - 50}}, 0, 3, 10, CYAN, "<", 20, YELLOW);
    BUTTON nextDirButton ({{windX - 100, windY - 100}, {windX - 10, windY - 50}}, 0, 3, 10, CYAN, ">", 20, YELLOW);
    BUTTON dirCaption ({{100, windY - 100}, {windX - 110, windY - 50}}, 3, 3, 10, CYAN, "(Directory)", 20, YELLOW);
    BUTTON prevButton ({{10, windY - 180}, {90, windY - 130}}, 0, 3, 10, CYAN, "<", 20, YELLOW);
    BUTTON nextButton ({{windX - 100, windY - 180}, {windX - 10, windY - 130}}, 0, 3, 10, CYAN, ">", 20, YELLOW);
    BUTTON selectButton ({{windX - 200, 10}, {windX - 10, 60}}, 0, 3, 10, CYAN, "Spiel starten", 20, YELLOW);
    BUTTON cancelButton ({{10, 10}, {190, 60}}, 0, 3, 10, CYAN, "Zurück", 20, YELLOW);
    BUTTON levelCaption ({{100, windY - 180}, {windX - 110, windY - 130}}, 3, 3, 10, CYAN, "(Levelname)", 20, YELLOW);
    AREA highscoreOutput {{100, 140}, {windX / 2 - 100, windY - 240}};
    prevDirButton.assignKeyboardButton(0,'Q');
    nextDirButton.assignKeyboardButton(0,'E');
    prevButton   .assignKeyboardButton(0,GLFW_KEY_LEFT ,'A');
    nextButton   .assignKeyboardButton(0,GLFW_KEY_RIGHT,'D');
    levelCaption .assignKeyboardButton(0,'S');
    cancelButton .assignKeyboardButton(0,GLFW_KEY_ESC);
    selectButton .assignKeyboardButton(0,GLFW_KEY_ENTER,GLFW_KEY_SPACE);

    ReloadHighscore();

    prepare_GameLoop();

    do
    {
        prepare_graphics();

        char levelText[MAX_PATH];
        if (GetLevel() != NULL)
            sprintf(levelText, "%s (%d/%d)", GetLevel()->name, GetLevel()->index + 1, GetLevelCount());
        else
            strcpy(levelText, "[Kein Level vorhanden]");

        levelCaption.setText(levelText);

        const char* directoryName;
        if (strlen(GetDirectory()->name) > 0)
            directoryName = GetDirectory()->name;
        else
            directoryName = noDirectoryName;

        char directoryText[strlen(directoryName) + 9];

        sprintf(directoryText, "%s (%d/%d)", directoryName, GetDirectory()->index + 1, GetDirectoryCount());
        dirCaption.setText(directoryText);

        //handle clicks
        if (cancelButton.clicked())
            break;
        else if (selectButton.clicked() && isInputValid())
        {
            currentLevel->score.setTimesPlayed(currentLevel->score.getTimesPlayed() + 1);
            return true;
        }
        else if (prevButton.clicked() || levelCaption.clicked() == 2)
            SwitchLevel(-1);
        else if (nextButton.clicked() || levelCaption.clicked() == 1)
            SwitchLevel(1);
        else if (prevDirButton.clicked() || dirCaption.clicked() == 2)
            SwitchDirectory(-1);
        else if (nextDirButton.clicked() || dirCaption.clicked() == 1)
            SwitchDirectory(1);

        //output
        prevDirButton.print();
        nextDirButton.print();
        dirCaption.print();
        prevButton.print();
        nextButton.print();
        selectButton.print();
        cancelButton.print();
        levelCaption.print();

        if (currentLevel != NULL)
        {
            drawBox({{windX/2 + 80, 140}, {windX - 110, windY - 240}}, 10, 3, WHITE);
            currentLevel->level->printPreview();

            TIME time = currentLevel->score.getTime();
            drawBox(highscoreOutput,10,3,WHITE);
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 40}, taLEFT, "Highscore");
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 80}, taLEFT, "Züge: %d", currentLevel->score.getMoves());
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 120}, taLEFT, "Zeit: %02d:%02d:%05.2f", time.Hours, time.Minutes, time.Seconds);
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 160}, taLEFT, "%d mal gespielt", currentLevel->score.getTimesPlayed());
        }
    } while (complete_graphics());

    logger(true, "LEVELSELECT: Cancelled, Returning false.");
    return false;
}

bool LEVELSELECT::isInputValid()
{
    return (currentLevel != NULL);
}

bool LEVELSELECT::NextLevelAvailable()
{
    return (GetLevel() != NULL && GetLevel()->next != NULL);
}

bool LEVELSELECT::PrevLevelAvailable()
{
    return (GetLevel() != NULL && GetLevel()->prev != NULL);
}

bool LEVELSELECT::SelectLevel(int index)
{
    bool found = false;
    LEVELFILE* level = GetLevel();
    if (level == NULL)
        return false;

    //get first level
    while (level->prev != NULL)
    {
        level = level->prev;
    }

    for (LEVELFILE* p = level; p != NULL; p = p->next)
    {
        if (p->index == index)
        {
            found = true;
            currentLevel = p;
            break;
        }
    }

    ReloadHighscore();

    return found;
}

bool LEVELSELECT::SwitchLevel(int offset)
{
    bool success = true;

    if (offset<0)
    {
        for (int i=0; i>offset; i--)
        {
            if (currentLevel != NULL && currentLevel->prev != NULL)
            {
                currentLevel = currentLevel->prev;
            }
            else
            {
                success = false;
                break;
            }
        }
    }
    else if (offset>0)
    {
        for (int i=0; i<offset; i++)
        {
            if (currentLevel != NULL && currentLevel->next != NULL)
            {
                currentLevel = currentLevel->next;
            }
            else
            {
                success = false;
                break;
            }
        }
    }

    ReloadHighscore();

    return success;
}

bool LEVELSELECT::NextDirectoryAvailable()
{
    return (GetDirectory() != NULL && GetDirectory()->next != NULL);
}

bool LEVELSELECT::PrevDirectoryAvailable()
{
    return (GetDirectory() != NULL && GetDirectory()->prev != NULL);
}

bool LEVELSELECT::SelectDirectory(int index)
{
    bool found = false;
    LEVELDIRECTORY* directory = GetDirectory();
    if (directory == NULL)
        return false;

    //get first directory
    while (directory->prev != NULL)
    {
        directory = directory->prev;
    }

    for (LEVELDIRECTORY* p = directory; p != NULL; p = p->next)
    {
        if (p->index == index)
        {
            found = true;
            currentDirectory = p;
            break;
        }
    }

    if (found)
        LoadDirectory();

    return found;
}

bool LEVELSELECT::SwitchDirectory(int offset)
{
    bool success = true;

    if (offset<0)
    {
        for (int i=0; i>offset; i--)
        {
            if (currentDirectory != NULL && currentDirectory->prev != NULL)
            {
                currentDirectory = currentDirectory->prev;
            }
            else
            {
                success = false;
                break;
            }
        }
    }
    else if (offset>0)
    {
        for (int i=0; i<offset; i++)
        {
            if (currentDirectory != NULL && currentDirectory->next != NULL)
            {
                currentDirectory = currentDirectory->next;
            }
            else
            {
                success = false;
                break;
            }
        }
    }

    if (success)
    {
        LoadDirectory();
    }

    return success;
}

//Loads current directory (including all levels it contains)
void LEVELSELECT::LoadDirectory()
{
    char levelPath[MAX_PATH];
    GetDirectoryPath(&levelPath[0], "");
    if (strlen(levelPath)+strlen(currentDirectory->name)>=MAX_PATH)
    {
        error("LEVELSELECT::SwitchDirectory", "FATAL: Path too long.");
        exit (1);
    }
    strcat(levelPath, currentDirectory->name);

    levelCount = 0;
    FreeLevelList(currentLevel);
    currentLevel = NULL;
    if (FILESYSTEMUTILITY::EnumerateFiles(levelPath, &levelCount, &addLevelListEntry))
    {
        SwitchLevel(0);
    }
    else
    {
        logger(true, "WARN: No levels available (Got false from FILESYSTEMUTILITY::EnumerateFiles)");
    }
}

void LEVELSELECT::ReloadHighscore()
{
    if (currentLevel != NULL)
        currentLevel->score = (currentLevel->name);
}

void LEVELSELECT::GetLevelPath(char* levelPath, const LEVELFILE* level)
{
    if (strlen(level->directory->path) + strlen(level->name) + 4 >= MAX_PATH)
    {
        error ("LEVELSELECT::GetLevelPath", "FATAL: Path too long (longer than %d). (Path: %s%s.lvl)", level->directory->path, level->name);
        exit (1);
    }
    else
    {
        sprintf(levelPath, "%s%s.lvl", level->directory->path, level->name);
    }
}

bool LEVELSELECT::GetDirectoryPath(char* directoryPath, const char* directoryName)
{
    const char basePath[] = "\\daten\\level\\";

    if (GetCurrentDirectory(MAX_PATH, directoryPath) != 0)
    {
        if (strlen(directoryPath) + strlen(basePath) + strlen(directoryName) >= MAX_PATH)
        {
            error("LEVELSELECT::GetDirectoryName", "Path too long (longer than %d). (Path: %s%s%s)", MAX_PATH, directoryPath, basePath, directoryName);
        }
        else
        {
            strcat(directoryPath, basePath);
            if (strlen(directoryName) > 0)
            {
                strcat(directoryPath, directoryName);
                strcat(directoryPath, "\\");
            }
            return true;
        }
    }
    else
    {
        error("LEVELSELECT::GetDirectoryName", "Couldn't get CurrentDirectory");
    }

    exit (1); //Only valid paths will be returned. If anything goes wrong -> exit
    return false;
}

bool LEVELSELECT::addLevelListEntry(char* levelFileName)
{
    if (strlen(levelFileName)+1 > MAX_PATH)
    {
        error("LEVELSELECT::addLevelListEntry", "Parameter levelFileName too long. Max length is MAX_PATH (%d).", MAX_PATH);
        return false;
    }

    if (!strEndsWith(levelFileName, ".lvl"))
    {
        logger(true, "WARNING: Unknown file in level directory %s: %s", GetCurrent()->GetDirectory()->name, levelFileName);
        return false;
    }

    LEVELFILE *lastLevel = GetCurrent()->GetLevel();
    LEVELFILE *newLevel;

    //get last level
    if (lastLevel != NULL)
    {
        while (lastLevel->next != NULL)
        {
            lastLevel = lastLevel->next;
        }
    }

    newLevel = (LEVELFILE*)malloc(sizeof(LEVELFILE));
    newLevel->prev = lastLevel;
    newLevel->next = NULL;
    if (lastLevel != NULL)
    {
        lastLevel->next = newLevel;
        newLevel->index = lastLevel->index+1;
    }
    else
    {
        newLevel->index = 0;
    }

    strcpy(newLevel->name, levelFileName);
    newLevel->name[strlen(newLevel->name)-4] = '\0'; //cut off fileextension (to get levelname)
    newLevel->directory = GetCurrent()->currentDirectory;
    GetCurrent()->GetLevelPath(newLevel->path, newLevel);
    newLevel->score = (newLevel->name);
    newLevel->level = new LEVEL({(windX/2+160), 160}, 17, newLevel->path, true);

    if (GetCurrent()->GetLevel() == NULL)
        GetCurrent()->currentLevel = newLevel;
    return true;
}

bool LEVELSELECT::addDirectoryListEntry(char* directoryName)
{
    if (strlen(directoryName)+1 > MAX_PATH)
    {
        error("LEVELSELECT::addLevelListEntry", "Parameter directoryName too long. Max length is MAX_PATH (%d).", MAX_PATH);
        return false;
    }

    LEVELDIRECTORY *lastDirectory = GetCurrent()->GetDirectory();
    LEVELDIRECTORY *newDirectory;

    //get last directory
    if (lastDirectory != NULL)
    {
        while (lastDirectory->next != NULL)
        {
            lastDirectory = lastDirectory->next;
        }
    }

    newDirectory = (LEVELDIRECTORY*)malloc(sizeof(LEVELDIRECTORY));
    newDirectory->prev = lastDirectory;
    newDirectory->next = NULL;
    if (lastDirectory != NULL)
    {
        lastDirectory->next = newDirectory;
        newDirectory->index = lastDirectory->index+1;
    }
    else
    {
        newDirectory->index = 0;
    }

    if (strcmp(directoryName, "..")==0)
    {
        strcpy(newDirectory->name, "");
    }
    else
    {
        strcpy(newDirectory->name, directoryName);
    }

    GetCurrent()->GetDirectoryPath(newDirectory->path, newDirectory->name);

    if (GetCurrent()->GetDirectory() == NULL)
        GetCurrent()->currentDirectory = newDirectory;
    return true;
}

//Frees hole list. listEntry can be any element of the target list. listEntry can be NULL.
void LEVELSELECT::FreeLevelList(LEVELFILE* listEntry)
{
    if (listEntry != NULL)
    {
        LEVELFILE* level = listEntry;

        //get first level
        while (level->prev != NULL)
        {
            level = level->prev;
        }

        //delete all
        for (LEVELFILE* p = level; p != NULL; p = p->next)
        {
            p->score.Save();
            delete p->level;
            if (p->next != NULL)
                p->next->prev = NULL;
            free(p);
        }
    }
}

//Frees hole list. listEntry can be any element of the target list. listEntry can be NULL.
void LEVELSELECT::FreeDirectoryList(LEVELDIRECTORY* listEntry)
{
    if (listEntry != NULL)
    {
        LEVELDIRECTORY* directory = listEntry;

        //get first directory
        while (directory->prev != NULL)
        {
            directory = directory->prev;
        }

        //delete all
        for (LEVELDIRECTORY* p = directory; p != NULL; p = p->next)
        {
            if (p->next != NULL)
                p->next->prev = NULL;
            free(p);
        }
    }
}

bool strEndsWith(char* string, const char* suffix)
{
    int stringLength, suffixLength;
    stringLength = strlen(string);
    suffixLength = strlen(suffix);
     if(stringLength < suffixLength)
    return false;

    return strncmp(string + stringLength - suffixLength, suffix, suffixLength) == 0;
}
