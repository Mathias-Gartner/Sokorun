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

    char levelPath[MAX_PATH];
    GetLevelPath(levelPath, "", false);
    if (FILESYSTEMUTILITY::EnumarateFiles(levelPath, &levelCount, &addLevelListEntry))
    {
        SwitchLevel(0);
    }
    else
    {
        logger(true, "WARN: No levels available (Got false from FILESYSTEMUTILITY::EnumerateFiles)");
    }
}

LEVELSELECT::~LEVELSELECT()
{
    if (currentLevel != NULL)
    {
        LEVELFILE* level = currentLevel;

        //get first level
        while (level->prev != NULL)
        {
            level = level->prev;
        }

        //delete all
        while (level->next != NULL)
        {
            level->score.Save();
            delete level->level;
            level = level->next;
            free(level->prev);
            level->prev = NULL;
        }
    }
}

int LEVELSELECT::Select()
{
    BUTTON prevButton ({{10, windY - 180}, {90, windY-130}}, 0, 3, 10, CYAN, "<", 20, YELLOW);
    BUTTON nextButton ({{windX - 100, windY - 180}, {windX - 10, windY - 130}}, 0, 3, 10, CYAN, ">", 20, YELLOW);
    BUTTON selectButton ({{windX - 200, 10}, {windX - 10, 60}}, 0, 3, 10, CYAN, "Spiel starten", 20, YELLOW);
    BUTTON cancelButton ({{10, 10}, {190, 60}}, 0, 3, 10, CYAN, "Zurück", 20, YELLOW);
    BUTTON levelCaption ({{100, windY - 180}, {windX - 110, windY - 130}}, 3, 3, 10, CYAN, "(Levelname)", 20, YELLOW);
    AREA highscoreOutput {{100, 140}, {windX / 2 - 100, windY - 240}};
    prevButton  .assignKeyboardButton(0,GLFW_KEY_LEFT ,'A');
    nextButton  .assignKeyboardButton(0,GLFW_KEY_RIGHT,'D');
    levelCaption.assignKeyboardButton(0,'S');
    cancelButton.assignKeyboardButton(0,GLFW_KEY_ESC);
    selectButton.assignKeyboardButton(0,GLFW_KEY_ENTER,GLFW_KEY_SPACE);

    prepare_GameLoop();

    do
    {
        prepare_graphics();

        char levelCount[3];
        char levelText[strlen(GetLevel()->name) + 8];
        strcpy(levelText, GetLevel()->name);
        strcat(levelText, " (");
        sprintf(levelCount, "%d", GetLevel()->index + 1);
        strcat(levelText, levelCount);
        strcat(levelText, "/");
        sprintf(levelCount, "%d", GetLevelCount());
        strcat(levelText, levelCount);
        strcat(levelText, ")");
        levelCaption.setText(levelText);

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

        //output
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
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 120}, taLEFT, "Zeit: %2d:%2d:%2d", time.Hours, time.Minutes, time.Seconds);
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

int LEVELSELECT::GetLevelCount()
{
    int count = 1;
    LEVELFILE* level = GetLevel();
    if (level == NULL)
        return 0;

    //get first level
    while (level->prev != NULL)
    {
        level = level->prev;
    }
    //count
    while (level->next != NULL)
    {
        level = level->next;
        count++;
    }

    return count;
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

    return found;
}

bool LEVELSELECT::SwitchLevel(int jumpWidth)
{
    bool success = true;

    if (jumpWidth<0)
    {
        for (int i=0; i>jumpWidth; i--)
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
    else if (jumpWidth>0)
    {
        for (int i=0; i<jumpWidth; i++)
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

    //reload highscore
    currentLevel->score = (GetLevel()->name);

    return success;
}

bool LEVELSELECT::GetLevelPath(char* levelPath, const char* levelName, bool appendExtension)
{
    const char basePath[] = "\\daten\\level\\";

    if (GetCurrentDirectory(MAX_PATH, levelPath) != 0)
    {
        if (strlen(levelPath)+strlen(basePath)+strlen(levelName) > MAX_PATH)
        {
            error("LEVELSELECT::GetLevelName", "Path too long (longer than %d). (Path: %s%s)", MAX_PATH, levelPath, levelName);
        }
        else
        {
            strcat(levelPath, basePath);
            strcat(levelPath, levelName);
            if (appendExtension)
              strcat(levelPath, ".lvl");
            return true;
        }
    }
    else
    {
        error ("LEVELSELECT::GetLevelName", "Couldn't get CurrentDirectory");
    }

    exit (1); //Only valid filepaths will be returned. If anything goes wrong -> exit
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
        logger(true, "WARNING: Unknown file in level directory: %s", levelFileName);
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
    GetCurrent()->GetLevelPath(newLevel->path, newLevel->name, true);
    newLevel->score = (newLevel->name);
    newLevel->level = new LEVEL({(windX/2+160), 160}, 17, newLevel->path, true);

    if (GetCurrent()->GetLevel() == NULL)
        GetCurrent()->currentLevel = newLevel;
    return true;
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
