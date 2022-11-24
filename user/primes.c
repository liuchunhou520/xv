#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define MSGSIZE 36


void prime(int pipe_read, int pipe_write) {
    char buff[MSGSIZE];
    int val = 0;
    read(pipe_read, buff, MSGSIZE);
//    printf("%s\n", buff);
    for (int i = 0; i < MSGSIZE; ++i) {
        if (buff[i] == '1') {
            val = i;
            break;
        }
    }
    if (val == 0) {
        exit(0);
    }
    printf("prime number: %d\n", val);
    buff[val] = '0';

    for (int i = 0; i < MSGSIZE; ++ i) {
        if (i % val == 0) {
            buff[i] = '0';
        }
    }
    int pid = fork();
    if (pid > 0) {
        write(pipe_write, buff, MSGSIZE);
    } else if (pid == 0) {
        prime(pipe_read, pipe_write);
    }

}


int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);
    char numbers[MSGSIZE];
    for (int i = 0; i < MSGSIZE; i ++) {
        numbers[i] = '1';
    }
    int pid = fork();
    if (pid > 0) {
        numbers[0] = '0';
        numbers[1] = '0';
        write(fd[1], numbers, MSGSIZE);
        wait(0);

    } else if (pid == 0){
        prime(fd[0], fd[1]);
        wait(0);
    }

    exit(0);
}
