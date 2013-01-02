#include <stdio.h>
#include <windows.h>
#include "filesystemutility.h"
#include "logger.h"

bool FILESYSTEMUTILITY::DirectoryExists(const char* path)
{
#ifdef _WINDOWS_H
    DWORD attribs = GetFileAttributesA(path);
    if (attribs == INVALID_FILE_ATTRIBUTES)
      return false;
    return (attribs & FILE_ATTRIBUTE_DIRECTORY);
#else  //_WINDOWS_H
    //TODO: Linux implementation
    error("FILESYSTEMUTILITY::DirectoryExists", "Not implemented!");
    return false;
#endif //_WINDOWS_H
}

bool FILESYSTEMUTILITY::EnumerateFiles(const char* directory, int* count, AddFunction addFunction)
{
    return EnumarateFileSystem(directory, count, addFunction, false);
}

bool FILESYSTEMUTILITY::EnumerateDirectories(const char* directory, int* count, AddFunction addFunction)
{
    return EnumarateFileSystem(directory, count, addFunction, true);
}

//Calls addFunction for every file or directory in a directory
//In an errornous case the function returns false
bool FILESYSTEMUTILITY::EnumarateFileSystem(const char* directory, int* count, AddFunction addFunction, bool directories)
{
    *count = 0; //set default value
#ifdef _WINDOWS_H
    int i=0;
    char dirWithWildcard[MAX_PATH];
    WIN32_FIND_DATA lpFindFileData;
    HANDLE findHandle;

    if (strlen(directory) > MAX_PATH - 3)
    {
        error ("EnurmerateFiles: Path %s is too long!", directory);
        return false;
    }

    //append wildcard
    strcpy(dirWithWildcard, directory);
    strcat(dirWithWildcard, "\\*");

    logger(true, "DEBUG: EnumerateFiles: Calling FindFirstFile with parameter %s", dirWithWildcard);
    findHandle = FindFirstFile(dirWithWildcard, &lpFindFileData);
    if (findHandle == INVALID_HANDLE_VALUE)
        return false;

    if (!(lpFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        addFunction(lpFindFileData.cFileName);
        i++;
    }

    while (FindNextFile(findHandle, &lpFindFileData))
    {
        if (findHandle != INVALID_HANDLE_VALUE)
        {
            if ((directories && (lpFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
               || (!directories && !(lpFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)))
            {
                if (addFunction(lpFindFileData.cFileName))
                    i++;
            }
        }
    }

    FindClose(findHandle);

    *count = i;
    return true;
#else //_WINDOWS_H
    //TODO: Linux implementation
    error("FILESYSTEMUTILITY::EnumerateFiles", "Not implemented!");
    return false;
#endif //_WINDOWS_H
}


char** FILESYSTEMUTILITY::appendToArray(char** filesArray, int* position, char* newEntry)
{
    char** newArray = (char**) malloc((*position)+1);
    char* newEntryInstance = (char*) malloc(strlen(newEntry)+1);
    strcpy(newEntryInstance, newEntry);

    if (newArray == NULL)
    {
        error("FILESYSTEMUTILITY::appendToArray", "FATAL: Cannot allocate memory");
        exit (1);
    }

    if (filesArray != NULL)
    {
        for (int i=0; i<*position; i++)
        {
            newArray[i] = filesArray[i];
        }

        free(filesArray);
    }

    newArray[*position] = newEntryInstance;
    newArray[*position][strlen(newEntryInstance)-4] = '\0'; //cut off fileextension (for nicer name)
    (*position)++;
    return newArray;
}
