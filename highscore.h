#ifndef HIGHSCORE_H_INCLUDED
#define HIGHSCORE_H_INCLUDED

#include <windows.h>
#include "gamelog.h"

typedef struct
{
    int Hours;
    int Minutes;
    double Seconds;
    long clockTicks;
} TIME;

class HIGHSCORE
{
    public:
        HIGHSCORE(char* level);
        virtual ~HIGHSCORE();
        void Load();
        void Save();
        void setScoreFromGameLog(GAMELOG* gamelog);
        unsigned int getMoves() { return m_moves; }
        void setMoves(unsigned int val) { m_moves = val; }
        TIME getTime() { return m_time; }
        void setTime(TIME val) { m_time = val; }
        unsigned int getTimesPlayed() { return m_timesplayed; }
        void setTimesPlayed(unsigned int val) { m_timesplayed = val; }
    protected:
    private:
        TIME clockTicksToScoreTime(long clockTicks);
        unsigned int m_moves;
        TIME m_time;
        unsigned int m_timesplayed;
        char level[MAX_PATH];
        bool isLoaded;
        static const char path[];
};

#endif // HIGHSCORE_H_INCLUDED
