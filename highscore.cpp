//Version 0.0
//
//Highscore-Anzeige nach dem beenden des Spiels (nur, wenn das Spiel nicht vom Editor aufgerufen wurde)

#include "main.h"
#include "highscore.h"
#include "filesystemutility.h"

HIGHSCORE::HIGHSCORE(char* levelname)
{
    isLoaded = false;
    level = levelname;
    Load();
}

HIGHSCORE::~HIGHSCORE()
{
    //dtor
}

void HIGHSCORE::Load()
{
    isLoaded = false;
    if (FILESYSTEMUTILITY::DirectoryExists("highscore"))
    {
        FILE* file = fopen(level, "r");
        if (file != NULL)
        {
            fscanf(file, "%d", m_timesplayed);
            fscanf(file, "%d", m_moves);
            fscanf(file, "%2d:%2d:%2d", m_time.Hours, m_time.Minutes, m_time.Seconds);
            fclose(file);
            isLoaded = true;
        }
    }
}

void HIGHSCORE::Save()
{
    char levelPath[strlen("highscore\\") + strlen(level) + 1];
    strcpy(levelPath, "highscore\\");
    strcat(levelPath, level);

    if (!DirectoryExists("highscore"))
        CreateDirectory("highscore", NULL);

    /*if (!isLoaded)
        Load();*/

    FILE* file = fopen(strcat("highscore\\", level), "w");
    if (file != NULL)
    {
        fprintf(file, "%d", m_timesplayed);
        fprintf(file, "%d", m_moves);
        fprintf(file, "%2d:%2d:%2d", m_time.Hours, m_time.Minutes, m_time.Seconds);
        fclose(file);
    }
}

bool HIGHSCORE::DirectoryExists(char* dir)
{
    DWORD attribs = GetFileAttributesA(dir);
    if (attribs == INVALID_FILE_ATTRIBUTES)
      return false;
    return (attribs & FILE_ATTRIBUTE_DIRECTORY);
}
