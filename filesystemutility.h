#ifndef FILESYSTEMUTILITY_H
#define FILESYSTEMUTILITY_H


class FILESYSTEMUTILITY
{
    public:
        static bool DirectoryExists(char* path);
        static char** EnumarateFiles(const char* directory, int* count);
    private:
        static char** appendToArray(char** filesArray, int* position, char* newEntry);
};

#endif // FILESYSTEMUTILITY_H
