#include <stdio.h>
#include "PEScanner.h"
#include <unistd.h>
int main(int argc, char *argv[]){
    char *usage = "Usage: %s\n-f bin (must require)\n-h show file header\n-d show data directory\n-s show section\n-a show all\n";
    LPSTR filePath = NULL;
    MAP_FILE_STRUCT stMapFile = {NULL, NULL, NULL};
    int opt;
    while((opt = getopt(argc, argv, "f:hdsa?")) != -1){
        switch(opt){
            case '?':
                fprintf(stderr, usage, argv[0]);
                return 0;
            case 'f':
                filePath = TEXT(optarg);
                InitPEScanner(&stMapFile, filePath);
                break;
            case 'h':
                if(!filePath){
                    fprintf(stderr, usage, argv[0]);
                    UnLoadFile(&stMapFile);
                    return -1;
                }
                ShowFileHeaderInfo(&stMapFile);
                break;
            case 'd':
                if(!filePath){
                    fprintf(stderr, usage, argv[0]);
                    UnLoadFile(&stMapFile);
                    return -1;
                }
                ShowDataDirectoryInfo(&stMapFile);
                break;
            case 's':
                if(!filePath){
                    fprintf(stderr, usage, argv[0]);
                    UnLoadFile(&stMapFile);
                    return -1;
                }
                ShowSectionHeaderInfo(&stMapFile);
                break;
            case 'a':
                if(!filePath){
                    fprintf(stderr, usage, argv[0]);
                    UnLoadFile(&stMapFile);
                    return -1;
                }
                ShowFileHeaderInfo(&stMapFile);
                ShowDataDirectoryInfo(&stMapFile);
                ShowSectionHeaderInfo(&stMapFile);
                break;
        }
    }
    if(!filePath){
        fprintf(stderr, usage, argv[0]);
        UnLoadFile(&stMapFile);
        return -1;
    }
    UnLoadFile(&stMapFile);
    return 0;
}