//Version 0.0
//
//Highscore-Anzeige nach dem beenden des Spiels (nur, wenn das Spiel nicht vom Editor aufgerufen wurde)

#include "main.h"
#include "highscore.h"
#include "filesystemutility.h"

HIGHSCORE::HIGHSCORE(char* mapname)
{
    isLoaded = false;
    map = mapname;
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
        FILE* file = fopen(map, "r");
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
    char mapPath[strlen("highscore\\") + strlen(map) + 1];
    strcpy(mapPath, "highscore\\");
    strcat(mapPath, map);

    if (!DirectoryExists("highscore"))
        CreateDirectory("highscore", NULL);

    /*if (!isLoaded)
        Load();*/

    FILE* file = fopen(strcat("highscore\\", map), "w");
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
