#ifndef PESCANNER_PESCANNER_H
#define PESCANNER_PESCANNER_H
#include <windows.h>
typedef struct _map_file_struct{
    HANDLE hFile;
    HANDLE hMapping;
    LPVOID ImageBase;
}MAP_FILE_STRUCT, *PMAP_FILE_STRUCT;
int LoadFile(LPTSTR lpFileName, PMAP_FILE_STRUCT pstMapFile);
void UnLoadFile(PMAP_FILE_STRUCT pstMapFile);
int IsPEFile(LPVOID ImageBase);
void InitPEScanner(PMAP_FILE_STRUCT pmapFileStruct, LPVOID filePath);
void ShowFileHeaderInfo(PMAP_FILE_STRUCT pstMapFile);
void ShowDataDirectoryInfo(PMAP_FILE_STRUCT pstMapFile);
void ShowSectionHeaderInfo(PMAP_FILE_STRUCT pstMapFile);
#endif //PESCANNER_PESCANNER_H
