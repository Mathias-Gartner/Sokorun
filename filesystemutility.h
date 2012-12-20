#ifndef FILESYSTEMUTILITY_H
#define FILESYSTEMUTILITY_H


class FILESYSTEMUTILITY
{
    public:
        typedef bool (*AddFunction)(char*);

        static bool DirectoryExists(const char* path);
        static bool EnumarateFiles(const char* directory, int* count, AddFunction addFunction);
    private:
        static char** appendToArray(char** filesArray, int* position, char* newEntry);
};

#endif // FILESYSTEMUTILITY_H
