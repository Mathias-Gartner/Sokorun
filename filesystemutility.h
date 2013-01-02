#ifndef FILESYSTEMUTILITY_H
#define FILESYSTEMUTILITY_H


class FILESYSTEMUTILITY
{
    public:
        typedef bool (*AddFunction)(char*);

        static bool DirectoryExists(const char* path);
        static bool EnumerateFiles(const char* directory, int* count, AddFunction addFunction);
        static bool EnumerateDirectories(const char* directory, int* count, AddFunction addFunction);
    private:
        static bool EnumarateFileSystem(const char* directory, int* count, AddFunction addFunction, bool directories);
        static char** appendToArray(char** filesArray, int* position, char* newEntry);
};

#endif // FILESYSTEMUTILITY_H
