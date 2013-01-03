//Version 0.0
//
//Highscore-Anzeige nach dem beenden des Spiels (nur, wenn das Spiel nicht vom Editor aufgerufen wurde)

#include <math.h>
#include <stdio.h>
#include <time.h>
#include "filesystemutility.h"
#include "highscore.h"
#include "logger.h"

const char HIGHSCORE::path[] = "daten\\highscore\\";

HIGHSCORE::HIGHSCORE(char* levelname, char* directoryname)
{
    m_moves = 0;
    m_time = {0, 0, 0};
    m_timesplayed = 0;
    isLoaded = false;

    if (strlen(levelname)<MAX_PATH)
    {
        strcpy(level, levelname);
    }
    else
    {
        error("HIGHSCORE::HIGHSCORE", "FATAL: Parameter levelname is too long.");
        exit(1);
    }
    if (strlen(directoryname)<MAX_PATH)
    {
        strcpy(directory, directoryname);
    }
    else
    {
        error("HIGHSCORE::HIGHSCORE", "FATAL: Parameter directoryname is too long.");
        exit(1);
    }

    Load();
}

HIGHSCORE::~HIGHSCORE()
{
    //dtor
}

void HIGHSCORE::Load()
{
    char filename[strlen(path) + strlen(directory) + strlen(level) + 8];
    sprintf(filename, "%s%s^%s.score", path, directory, level);
    isLoaded = false;

    if (FILESYSTEMUTILITY::DirectoryExists(path))
    {
        FILE* file = fopen(filename, "r");
        if (file != NULL)
        {
            fscanf(file, "%d", &m_timesplayed);
            fscanf(file, "%d", &m_moves);
            //fscanf(file, "%2d:%2d:%2d", &m_time.Hours, &m_time.Minutes, &m_time.Seconds);
            fscanf(file, "%ld", &m_time.clockTicks);
            fclose(file);
            m_time = clockTicksToScoreTime(m_time.clockTicks);
            logger(true, "Higscore of level %s loaded.", level);
            isLoaded = true;
        }
    }
}

void HIGHSCORE::Save()
{
    char levelPath[strlen(path) + strlen(directory) + strlen(level) + 8];
    sprintf(levelPath, "%s%s^%s.score", path, directory, level);

    if (!FILESYSTEMUTILITY::DirectoryExists(path))
        CreateDirectory(path, NULL);

    FILE* file = fopen(levelPath, "w");
    if (file != NULL)
    {
        fprintf(file, "%d\n", m_timesplayed);
        fprintf(file, "%d\n", m_moves);
        //fprintf(file, "%2d:%2d:%2d\n", m_time.Hours, m_time.Minutes, m_time.Seconds);
        fprintf(file, "%ld", m_time.clockTicks);
        fclose(file);
    }
}

void HIGHSCORE::setScoreFromGameLog(GAMELOG* gamelog)
{
    TIME time = clockTicksToScoreTime(gamelog->getPlayTime());
    TIME currentTime = getTime();

    if ((time.Hours < currentTime.Hours) ||
        (time.Hours == currentTime.Hours && time.Minutes < currentTime.Minutes) ||
        (time.Hours == currentTime.Hours && time.Minutes == currentTime.Minutes && time.Seconds < currentTime.Seconds) ||
        (currentTime.Hours == 0 && currentTime.Minutes == 0 && currentTime.Seconds == 0))
    {
        setTime(time);
    }
    if (getMoves() == 0 || getMoves() > gamelog->getUserAvatarMoves())
    {
        setMoves(gamelog->getUserAvatarMoves());
    }
    Save();
}

TIME HIGHSCORE::clockTicksToScoreTime(long clockTicks)
{
    TIME time = {0, 0, 0, clockTicks};
    if(clockTicks<60000)
    {
        time.Seconds = (double)clockTicks/(double)CLOCKS_PER_SEC;
    }
    else
    {
        double tmpseconds = clockTicks/CLOCKS_PER_SEC;

        if(tmpseconds<3600)
        {
            time.Minutes = (int)tmpseconds/60;
            time.Seconds = (int)tmpseconds%60;
        }
        else
        {
            time.Hours = (int)tmpseconds/3600;
            double lastHour = fmod(tmpseconds, 3600.0);
            time.Minutes = (int)lastHour/60;
            time.Seconds = fmod(lastHour, 60.0);
        }
    }
    return time;
}
