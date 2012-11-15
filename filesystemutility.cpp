#include "filesystemutility.h"
#include "main.h"

bool FILESYSTEMUTILITY::DirectoryExists(char* path)
{
#ifdef _WINDOWS_H
    DWORD attribs = GetFileAttributesA(path);
    if (attribs == INVALID_FILE_ATTRIBUTES)
      return false;
    return (attribs & FILE_ATTRIBUTE_DIRECTORY);
#else  //_WINDOWS_H
    //TODO: Linux implementation
    error("FILESYSTEMUTILITY::DirectoryExists not implemented!");
#endif //_WINDOWS_H
}
