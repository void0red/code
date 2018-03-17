#include <stdio.h>
#include <stdlib.h>                                              
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
int main(){
    char *argv[101] = {0};
    for(int i = 0; i < 101; i++)argv[i] = "a"; 
    argv[0] = "/home/input2/input";
    argv['A'] = "\x00";
    argv['B'] = "\x20\x0a\x0d";
    argv['C'] = "2333";
    argv[100] = NULL;
    char *envp[2] = {"\xde\xad\xbe\xef=\xca\xfe\xba\xbe", NULL};
    FILE *fp = fopen("\x0a", "wb");\
    fwrite("\x00\x00\x00\x00", 4, 1, fp);
    fclose(fp);
    int myin[2], myerr[2];
    pipe(myin);
    pipe(myerr);
    pid_t pid = fork();
    if(pid == 0){
            close(myin[0]);
            close(myerr[0]);
            write(myin[1], "\x00\x0a\x00\xff", 4);
            write(myerr[1], "\x00\x0a\x02\xff", 4);
    }else if(pid > 0){
            close(myin[1]);
            close(myerr[1]);
            dup2(myin[0], 0);
            dup2(myerr[0], 2);
            execve(argv[0], argv, envp);
    }
    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(2333);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    connect(sock, &serv, sizeof(serv));
    char *buf = "\xde\xad\xbe\xef\n";
    send(sock, buf, strlen(buf), 0);
    close(sock);
    return 0;
}