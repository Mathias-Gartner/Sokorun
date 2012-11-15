#ifndef HIGHSCORE_H_INCLUDED
#define HIGHSCORE_H_INCLUDED

typedef struct
{
    int Hours;
    int Minutes;
    int Seconds;
} TIME;

class HIGHSCORE
{
    public:
        HIGHSCORE(char* map);
        virtual ~HIGHSCORE();
        void Load();
        void Save();
        unsigned int Getmoves() { return m_moves; }
        void Setmoves(unsigned int val) { m_moves = val; }
        TIME Gettime() { return m_time; }
        void Settime(TIME val) { m_time = val; }
        unsigned int Gettimesplayed() { return m_timesplayed; }
        void Settimesplayed(unsigned int val) { m_timesplayed = val; }
    protected:
    private:
        bool DirectoryExists(char* dir);
        unsigned int m_moves;
        TIME m_time;
        unsigned int m_timesplayed;
        char* map;
        bool isLoaded;
};

#endif // HIGHSCORE_H_INCLUDED
