#include <stdio.h>
#include <time.h>
#include "PEScanner.h"
void InitPEScanner(PMAP_FILE_STRUCT pmapFileStruct, LPVOID filePath){
    if(!LoadFile(filePath, pmapFileStruct)){
        fprintf(stderr, "load error\n");
        return;
    }
    if(!IsPEFile(pmapFileStruct->ImageBase)){
        UnLoadFile(pmapFileStruct);
        fprintf(stderr, "not a pe file\n");
        return;
    }
}
int LoadFile(LPTSTR lpFileName, PMAP_FILE_STRUCT pstMapFile){
    if(lpFileName == NULL){
        return 0;
    }
    HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
    if(!hFile){
        fprintf(stderr, "create file error\n");
        return 0;
    }
    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, 0);
    if(!hMapping){
        CloseHandle(hFile);
        fprintf(stderr, "create file mapping error\n");
        return 0;
    }
    LPVOID ImageBase = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
    if(!ImageBase){
        CloseHandle(hFile);
        CloseHandle(hMapping);
        fprintf(stderr, "map view of file error\n");
        return 0;
    }
    memset(pstMapFile, 0, sizeof(MAP_FILE_STRUCT));
    pstMapFile->hFile = hFile;
    pstMapFile->hMapping = hMapping;
    pstMapFile->ImageBase = ImageBase;
}
void UnLoadFile(PMAP_FILE_STRUCT pstMapFile){
    if(pstMapFile->hFile){
        CloseHandle(pstMapFile->hFile);
    }
    if(pstMapFile->hMapping){
        CloseHandle(pstMapFile->hMapping);
    }
    if(pstMapFile->ImageBase){
        UnmapViewOfFile(pstMapFile->ImageBase);
    }
}
int IsPEFile(LPVOID ImageBase){
    if(!ImageBase){
        return 0;
    }
    PIMAGE_DOS_HEADER pimageDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
    if(pimageDosHeader->e_magic != IMAGE_DOS_SIGNATURE){
        fprintf(stderr, "not found a dos header\n");
        return 0;
    }
    PIMAGE_NT_HEADERS pimageNtHeaders = (PIMAGE_NT_HEADERS)((DWORD)pimageDosHeader + pimageDosHeader->e_lfanew);
    if(pimageNtHeaders->Signature != IMAGE_NT_SIGNATURE){
        fprintf(stderr, "not found a nt header\n");
        return 0;
    }
    return 1;
}
PIMAGE_NT_HEADERS GetNtHeaders(LPVOID ImageBase){
    if(!IsPEFile(ImageBase)){
        return NULL;
    }
    PIMAGE_DOS_HEADER pimageDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
    PIMAGE_NT_HEADERS pimageNtHeaders = (PIMAGE_NT_HEADERS)((DWORD)pimageDosHeader + pimageDosHeader->e_lfanew);
    return pimageNtHeaders;
}
PIMAGE_FILE_HEADER GetFileHeader(LPVOID ImageBase){
    PIMAGE_NT_HEADERS pimageNtHeaders = GetNtHeaders(ImageBase);
    if(!pimageNtHeaders){
        return NULL;
    }
    return &(pimageNtHeaders->FileHeader);
}
PIMAGE_OPTIONAL_HEADER GetOptionHeader(LPVOID ImageBase){
    PIMAGE_NT_HEADERS pimageNtHeaders = GetNtHeaders(ImageBase);
    if(!pimageNtHeaders){
        return NULL;
    }
    return &(pimageNtHeaders->OptionalHeader);
}
void ShowFileHeaderInfo(PMAP_FILE_STRUCT pstMapFile){
    PIMAGE_FILE_HEADER pimageFileHeader = GetFileHeader(pstMapFile->ImageBase);
    if(!pimageFileHeader){
        fprintf(stderr, "get file header failed!\n");
        return;
    }
    PIMAGE_OPTIONAL_HEADER pimageOptionalHeader = GetOptionHeader(pstMapFile->ImageBase);
    if(!pimageOptionalHeader){
        fprintf(stderr, "get file optional header failed!\n");
        return;
    }
    time_t t = pimageFileHeader->TimeDateStamp;
    struct tm* ptmBegin = localtime(&t);
    char strTime[20];
    strftime(strTime, 20, "%Y/%m/%d %H:%M:%S", ptmBegin);
    char *strFileHeaderFormat = "[IMAGE_FILE_HEADER]\n\
Machine:        \t\t%04lX\n\
NumberOfSection:\t\t%04lX\n\
TimeDateStamp:    \t\t%s(%04lX)\n\
PointerToSymbolTable:\t\t%08lX\n\
SizeOfOptionalHeader:\t\t%04lX\n\
Characteristics:\t\t%04lX\n";
    char strTmp[1024];
    sprintf(strTmp, strFileHeaderFormat, pimageFileHeader->Machine, pimageFileHeader->NumberOfSections, strTime,
    pimageFileHeader->TimeDateStamp, pimageFileHeader->PointerToSymbolTable, pimageFileHeader->SizeOfOptionalHeader,
    pimageFileHeader->Characteristics);
    puts(strTmp);
    memset(strTmp, 0, sizeof(strTmp));
    char *strFileOptionalHeaderFormat = "[IMAGE_OPTIONAL_HEADER]\n\
Entry Point:\t\t\t%08lX\n\
Image Base:\t\t\t%08lX\n\
Code Base:\t\t\t%08lX\n\
Data Base:\t\t\t%08lX\n\
Image Size:\t\t\t%08lX\n\
Headers Size:\t\t\t%08lX\n\
Section Alignment:\t\t%08lX\n\
File Alignment:\t\t\t%08lX\n\
Subsystem:\t\t\t%08lX\n\
Check Sum:\t\t\t%04lX\n\
Dll Flags:\t\t\t%04lX\n";
    sprintf(strTmp, strFileOptionalHeaderFormat, pimageOptionalHeader->AddressOfEntryPoint, pimageOptionalHeader->ImageBase,
    pimageOptionalHeader->BaseOfCode, pimageOptionalHeader->BaseOfData, pimageOptionalHeader->SizeOfImage,
    pimageOptionalHeader->SizeOfHeaders, pimageOptionalHeader->SectionAlignment, pimageOptionalHeader->FileAlignment,
    pimageOptionalHeader->Subsystem, pimageOptionalHeader->CheckSum, pimageOptionalHeader->DllCharacteristics);
    puts(strTmp);
}
void ShowDataDirectoryInfo(PMAP_FILE_STRUCT pstMapFile){
    PIMAGE_OPTIONAL_HEADER pimageOptionalHeader = GetOptionHeader(pstMapFile->ImageBase);
    if(!pimageOptionalHeader){
        fprintf(stderr, "get file optional header failed!\n");
        return;
    }
    char data[16][20] = {
            "Export Table:",
            "Import Table",
            "Resource:",
            "Exception:",
            "Certificate:",
            "Relocation:",
            "Debug:     ",
            "Copyright:",
            "Global Pointer:",
            "TLS Table:",
            "Load Config:",
            "Bound Import:",
            "Import Address:",
            "Delay Import:",
            "CLR Header:",
            "Reserved:   "
    };
    printf("[Date Directory]\n");
    printf("NAME\t\tRVA\t\tSize\n");
    for(int i = 0; i < 16; i++){
        printf("%s\t%08lX\t%08lX\n", data[i], pimageOptionalHeader->DataDirectory[i].VirtualAddress,
        pimageOptionalHeader->DataDirectory[i].Size);
    }
    putchar('\n');
}
PIMAGE_SECTION_HEADER GetFirstSectionHeader(LPVOID ImageBase){
    if(!ImageBase){
        return NULL;
    }
    PIMAGE_NT_HEADERS pimageNtHeaders = GetNtHeaders(ImageBase);
    if(!pimageNtHeaders){
        return NULL;
    }
    PIMAGE_SECTION_HEADER pimageSectionHeader = IMAGE_FIRST_SECTION(pimageNtHeaders);
    return pimageSectionHeader;
}
void ShowSectionHeaderInfo(PMAP_FILE_STRUCT pstMapFile){
    PIMAGE_FILE_HEADER pimageFileHeader = GetFileHeader(pstMapFile->ImageBase);
    if(!pimageFileHeader){
        fprintf(stderr, "get file header failed!\n");
        return;
    }
    printf("[Section Table]:\n");
    printf("Name\tVAddress\tMisc    \tRAddress\tRSize    \tFlags\n");
    PIMAGE_SECTION_HEADER pimageSectionHeader = GetFirstSectionHeader(pstMapFile->ImageBase);
    for(int i = 0; i < pimageFileHeader->NumberOfSections; i++){
        printf("%s\t%08lX\t%08lX\t%08lX\t%08lX\t%08lX\n", pimageSectionHeader->Name, pimageSectionHeader->VirtualAddress,
        pimageSectionHeader->Misc, pimageSectionHeader->PointerToRawData, pimageSectionHeader->SizeOfRawData,
        pimageSectionHeader->Characteristics);
        pimageSectionHeader += 1;
    }
}