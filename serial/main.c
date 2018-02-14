#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <zconf.h>
#include "base64.h"
#include "md5.h"
#include "client.h"
static void *func(char *in, int n);
int main() {
    char buf[1024];
    getcwd(buf, sizeof(buf));
    char *out = base64_encode(buf, sizeof(buf));
    func(out, 15);
    printf("SN: %s\nKEY: ", out);
    char *key = client(out);
    free(out);
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf), stdin);
    buf[strlen(buf)-1] = '\0';
    char *in = md5(buf);
    if(!strcmp(key, in)){
        printf("you got it\n");
    }else{
        printf("Wrong\n");
    }
    return 0;
}
static void *func(char *in, int n){
    int len = strlen(in);
    if(len > n){
        in[n] = '\0';
    }
}