#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MSGSIZE 16

int main() {
    int fd[2];
    char buff[MSGSIZE];
    pipe(fd);

    int pid = fork();

    if (pid > 0) {
        printf("I am the father\n");
        write(fd[1], "ping", MSGSIZE);
        wait(NULL);
        read(fd[0], buff, MSGSIZE);
        printf("%d received from child: %s\n", pid, buff);
    } else {
        printf("I am the child\n");
        read(fd[0], buff, MSGSIZE);
        printf("%d received from father: %s\n",pid, buff);
        write(fd[1], "pong", MSGSIZE);


    }
    return 0;

}