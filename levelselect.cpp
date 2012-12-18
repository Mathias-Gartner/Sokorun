//Version 0.0
//Mathias Gartner
//Levelauswahl

#include <stdio.h>
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
            delete level->level;
            level = level->next;
            free(level->prev);
            level->prev = NULL;
        }
    }
}

int LEVELSELECT::Select()
{
    long loopStart;
    BUTTON prevButton ({{10, windY - 180}, {90, windY-130}}, 0, 3, 10, CYAN, "<", 20, YELLOW);
    BUTTON nextButton ({{windX - 100, windY - 180}, {windX - 10, windY - 130}}, 0, 3, 10, CYAN, ">", 20, YELLOW);
    BUTTON selectButton ({{windX - 200, 10}, {windX - 10, 60}}, 0, 3, 10, CYAN, "Spiel starten", 20, YELLOW);
    BUTTON cancelButton ({{10, 10}, {190, 60}}, 0, 3, 10, CYAN, "Zurück", 20, YELLOW);
    BUTTON levelCaption ({{100, windY - 180}, {windX - 110, windY - 130}}, 3, 3, 10, CYAN, "(Levelname)", 20, YELLOW);
    AREA highscoreOutput {{100, 140}, {windX / 2 - 100, windY - 240}};

    prepare_GameLoop();

    do
    {
        loopStart = clock();
        prepare_graphics();

        levelCaption.setText(GetLevel()->name);

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

    logger(true, "LEVELSELECT: Got false from complete_graphics(..). Returning -1.");
    return false;
}

bool LEVELSELECT::isInputValid()
{
    return (currentLevel != NULL);
}

void LEVELSELECT::SwitchLevel(int jumpWidth)
{
    if (jumpWidth<0)
    {
        for (int i=0; i>jumpWidth; i--)
        {
            if (currentLevel != NULL && currentLevel->prev != NULL)
                currentLevel = currentLevel->prev;
            else
                break;
        }
    }
    else if (jumpWidth>0)
    {
        for (int i=0; i<jumpWidth; i++)
        {
            if (currentLevel != NULL && currentLevel->next != NULL)
                currentLevel = currentLevel->next;
            else
                break;
        }
    }
    /*currentLevelIndex = currentLevelIndex+jumpWidth;
    logger(true, "LEVELSELECT: Switching level, increasing %d, new index %d", jumpWidth, currentLevelIndex);

    if (currentLevelIndex < 0)
        currentLevelIndex = 0;
    if (currentLevelIndex >= levelCount)
        currentLevelIndex = levelCount - 1;

    levelName = levels[currentLevelIndex];*/

    /*if (currentLevel != NULL)
        delete currentLevel;
    currentLevel = new LEVEL({windX/2+160, 160}, 17, GetLevelPath(), true);*/

    //reload highscore
    currentLevel->score = (GetLevel()->name);
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
        lastLevel->next = newLevel;
    strcpy(newLevel->name, levelFileName);
    newLevel->name[strlen(newLevel->name)-4] = '\0'; //cut off fileextension
    GetCurrent()->GetLevelPath(newLevel->path, newLevel->name, true);
    newLevel->score = (newLevel->name);
    newLevel->level = new LEVEL({(windX/2+160), 160}, 17, newLevel->path, true);

    if (GetCurrent()->GetLevel() == NULL)
        GetCurrent()->currentLevel = newLevel;
    return true;
}
