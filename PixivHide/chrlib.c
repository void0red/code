//
// Created by void0red on 18-2-26.
//

#include "chrlib.h"
int _pow(int num, int n){
    int ret = 1;
    for(int i = 0; i < n; i++){
        ret *= num;
    }
    return ret;
}
void dec2bin(int src, char *tar){
    tar[8] = '\0';
    int i = 7;
    while(src){
        tar[i--] = (char)(src % 2 + '0');
        src /= 2;
    }
    while(i >= 0){
        tar[i--] = '0';
    }
}
char *txt2bin(char *src){
    char res[2];
    char *ret = (char *)malloc(8 * strlen(src) + 1);
    int i = 0, tmp;
    while(src[i]){
        sprintf(res, "%02x", (unsigned char)src[i]);
        tmp = (int)strtol(res, 0, 16);
//        sscanf(res, "%x", &tmp);
        dec2bin(tmp, ret + i * 8);
        i++;
    }
    return ret;
}
char *bin2txt(char *src){
    int x = (int)(strlen(src) / 8);
    char *ret = (char *)malloc(sizeof(char)*(x + 1));
    ret[x + 1] = '\0';
    int cnt = 0;
    for(int i = 0; i < strlen(src); i += 8, cnt++){
        int tmp = 0;
        for(int j = 0; j < 8; j++){
            tmp += (src[i+j] - '0') * _pow(2, 7-j);
        }
        sprintf(ret + cnt, "%c", tmp);
    }
    return ret;
}