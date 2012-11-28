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
#include "levelselect.h"
#include "logger.h"

LEVELSELECT::LEVELSELECT()
{
    currentLevelIndex = 0;
    levels = FILESYSTEMUTILITY::EnumarateFiles(GetLevelPath("", false), &levelCount);
    if (levels == NULL)
    {
        logger(true, "WARN: No Levels available (Got NULL from FILESYSTEMUTILITY::EnumarateFiles)");
    }
    else
    {
        switchLevel(0);
    }
}

LEVELSELECT::~LEVELSELECT()
{
    if (levels != NULL)
    {
        for (int i=0; i<levelCount; i++)
        {
            free(&levels[i][0]);
        }
        free(levels);
    }

    if (currentLevel != NULL)
        delete currentLevel;
    if (currentLevelScore != NULL)
        delete currentLevelScore;
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

        levelCaption.setText(GetLevelName());

        //handle clicks
        if (cancelButton.clicked())
            break;
        else if (selectButton.clicked() && isInputValid())
        {
            currentLevelScore->setTimesPlayed(currentLevelScore->getTimesPlayed() + 1);
            return true;
        }
        else if (prevButton.clicked() || levelCaption.clicked() == 2)
            switchLevel(-1);
        else if (nextButton.clicked() || levelCaption.clicked() == 1)
            switchLevel(1);

        //output
        prevButton.print();
        nextButton.print();
        selectButton.print();
        cancelButton.print();
        levelCaption.print();

        if (currentLevel != NULL)
        {
            drawBox({{windX/2 + 80, 140}, {windX - 110, windY - 240}}, 10, 3, WHITE);
            currentLevel->printPreview();
        }
        if (currentLevelScore != NULL)
        {
            TIME time = currentLevelScore->getTime();
            drawBox(highscoreOutput,10,3,WHITE);
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 40}, taLEFT, "Highscore");
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 80}, taLEFT, "Züge: %d", currentLevelScore->getMoves());
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 120}, taLEFT, "Zeit: %2d:%2d:%2d", time.Hours, time.Minutes, time.Seconds);
            normalFont.printf({highscoreOutput.a.x + 20, highscoreOutput.b.y - 160}, taLEFT, "%d mal gespielt", currentLevelScore->getTimesPlayed());
        }
    } while (complete_graphics(loopStart, GAMESPEED));

    logger(true, "LEVELSELECT: Got false from complete_graphics(..). Returning -1.");
    return false;
}

bool LEVELSELECT::isInputValid()
{
    return (levels != NULL && GetLevelName() != NULL);
}

void LEVELSELECT::switchLevel(int jumpWidth)
{
    currentLevelIndex = currentLevelIndex+jumpWidth;
    logger(true, "LEVELSELECT: Switching level, increasing %d, new index %d", jumpWidth, currentLevelIndex);

    if (currentLevelIndex < 0)
        currentLevelIndex = 0;
    if (currentLevelIndex >= levelCount)
        currentLevelIndex = levelCount - 1;

    levelName = levels[currentLevelIndex];

    if (currentLevel != NULL)
        delete currentLevel;
    currentLevel = new LEVEL({windX/2+160, 160}, 17, GetLevelPath(), true);
    if (currentLevelScore != NULL)
        delete currentLevelScore;
    currentLevelScore = new HIGHSCORE(GetLevelName());
}

char* LEVELSELECT::GetLevelPath(const char* levelName, bool appendExtension)
{
    if (GetCurrentDirectory(MAX_PATH, &LEVELSELECT::levelPath[0]) != 0)
    {
        if (strlen(LEVELSELECT::levelPath)+strlen(levelName) > MAX_PATH)
        {
            error("LEVELSELECT::GetLevelName", "Path too long (longer than %d). (Path: %s%s)", MAX_PATH, LEVELSELECT::levelPath, levelName);
        }
        else
        {
            strcat(LEVELSELECT::levelPath, "\\daten\\level\\");
            strcat(LEVELSELECT::levelPath, levelName);
            if (appendExtension)
              strcat(LEVELSELECT::levelPath, ".lvl");
            return LEVELSELECT::levelPath;
        }
    }
    else
    {
        error ("LEVELSELECT::GetLevelName", "Couldn't get CurrentDirectory");
    }

    exit (1); //Only valid filepaths will be returned. If anything goes wrong -> exit
    return NULL;
}
