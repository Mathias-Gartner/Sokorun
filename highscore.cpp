//Version 0.0
//
//Highscore-Anzeige nach dem beenden des Spiels (nur, wenn das Spiel nicht vom Editor aufgerufen wurde)

#include <stdio.h>
#include "filesystemutility.h"
#include "highscore.h"
#include "logger.h"

const char HIGHSCORE::path[] = "daten\\highscore\\";

HIGHSCORE::HIGHSCORE(char* levelname)
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
    Load();
}

HIGHSCORE::~HIGHSCORE()
{
    //dtor
}

void HIGHSCORE::Load()
{
    char filename[strlen(path) + strlen(level) + 7];
    strcpy(filename, path);
    strcat(filename, level);
    strcat(filename, ".score");
    isLoaded = false;
    if (FILESYSTEMUTILITY::DirectoryExists(path))
    {
        FILE* file = fopen(filename, "r");
        if (file != NULL)
        {
            fscanf(file, "%d", &m_timesplayed);
            fscanf(file, "%d", &m_moves);
            fscanf(file, "%2d:%2d:%2d", &m_time.Hours, &m_time.Minutes, &m_time.Seconds);
            fclose(file);
            logger(true, "Higscore of level %s loaded.", level);
            isLoaded = true;
        }
    }
}

void HIGHSCORE::Save()
{
    char levelPath[strlen(path) + strlen(level) + 7];
    strcpy(levelPath, path);
    strcat(levelPath, level);
    strcat(levelPath, ".score");

    if (!FILESYSTEMUTILITY::DirectoryExists(path))
        CreateDirectory(path, NULL);

    /*if (!isLoaded)
        Load();*/

    FILE* file = fopen(levelPath, "w");
    if (file != NULL)
    {
        fprintf(file, "%d", m_timesplayed);
        fprintf(file, "%d", m_moves);
        fprintf(file, "%2d:%2d:%2d", m_time.Hours, m_time.Minutes, m_time.Seconds);
        fclose(file);
        logger(true, "Highscore of level %s saved.", level);
    }
}
