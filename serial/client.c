#include <arpa/inet.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <zconf.h>

char *client(char *tar){
    struct sockaddr_in remote_addr;
    char *buf = (char *)malloc(1024);
    memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr("123.56.218.81");
    remote_addr.sin_port = htons(8848);
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(client_fd < 0){
        printf("socket error\n");
        exit(0);
    }
    int connect_fd = connect(client_fd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));
    if(connect_fd < 0){
        printf("connect error\n");
        exit(0);
    }
    send(client_fd, tar, strlen(tar), 0);
    int len = recv(client_fd, buf, 1024, 0);
    buf[len] = '\0';
    close(client_fd);
    return buf;
}