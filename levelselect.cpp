//Version 0.0
//Mathias Gartner
//Levelauswahl

#include <ctime>
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
    {   switchLevel(0);
    }
}

int LEVELSELECT::Select()
{
    long loopStart;
    BUTTON *prevButton = new BUTTON({{10, windY - 180}, {90, windY-130}}, 0, 3, 10, CYAN, "<", 20, YELLOW);
    BUTTON *nextButton = new BUTTON({{windX - 100, windY - 180}, {windX - 10, windY - 130}}, 0, 3, 10, CYAN, ">", 20, YELLOW);
    BUTTON *selectButton = new BUTTON({{windX - 200, 10}, {windX - 10, 60}}, 0, 3, 10, CYAN, "Spiel starten", 20, YELLOW);
    BUTTON *cancelButton = new BUTTON({{10, 10}, {190, 60}}, 0, 3, 10, CYAN, "Zurück", 20, YELLOW);
    BUTTON *levelCaption = new BUTTON({{100, windY - 180}, {windX - 110, windY - 130}}, 3, 3, 10, CYAN, "(Levelname)", 20, YELLOW);
    AREA highscoreOutput = {{20, 20}, {windX / 2, windY - 200}};

    //Tastaturtasten zu den Buttons hinzufügen:
        prevButton  ->assignKeyboardButton(0,GLFW_KEY_LEFT ,'A');
        nextButton  ->assignKeyboardButton(0,GLFW_KEY_RIGHT,'D');
        levelCaption->assignKeyboardButton(0,'S');
        cancelButton->assignKeyboardButton(0,GLFW_KEY_ESC);
        selectButton->assignKeyboardButton(0,GLFW_KEY_ENTER,GLFW_KEY_SPACE);

    prepare_GameLoop();

    do
    {
        loopStart = clock();
        prepare_graphics();

        levelCaption->setText(GetLevelName());

        //handle clicks
        if (cancelButton->clicked())
            return false;
        else if (selectButton->clicked() && isInputValid())
            return true;
        else if (prevButton->clicked() || levelCaption->clicked() == 2)
            switchLevel(-1);
        else if (nextButton->clicked() || levelCaption->clicked() == 1)
            switchLevel(1);

        //output
        prevButton->print();
        nextButton->print();
        selectButton->print();
        cancelButton->print();
        levelCaption->print();

        /*if (currentLevelScore != NULL)
        {
            drawBox(highscoreOutput,10,3,CYAN);
            normalFont.setFontColor(YELLOW);
            normalFont.setFontSize(20);
            normalFont.putString("Highscore",{highscoreOutput.a.x+(highscoreOutput.b.x-highscoreOutput.a.x)/2 , highscoreOutput.a.y + (highscoreOutput.b.y-highscoreOutput.a.y-20)/2},taLEFT); break;
        }*/
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

    free(currentLevelScore);
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
