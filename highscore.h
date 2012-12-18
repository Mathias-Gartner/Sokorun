#ifndef HIGHSCORE_H_INCLUDED
#define HIGHSCORE_H_INCLUDED

#include <windows.h>

typedef struct
{
    int Hours;
    int Minutes;
    int Seconds;
} TIME;

class HIGHSCORE
{
    public:
        HIGHSCORE(char* level);
        virtual ~HIGHSCORE();
        void Load();
        void Save();
        int getMoves() { return m_moves; }
        void setMoves(int val) { m_moves = val; }
        TIME getTime() { return m_time; }
        void setTime(TIME val) { m_time = val; }
        int getTimesPlayed() { return m_timesplayed; }
        void setTimesPlayed(int val) { m_timesplayed = val; }
    protected:
    private:
        bool DirectoryExists(char* dir);
        int m_moves;
        TIME m_time;
        int m_timesplayed;
        char level[MAX_PATH];
        bool isLoaded;
};

#endif // HIGHSCORE_H_INCLUDED
