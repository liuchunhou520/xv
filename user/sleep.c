#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argv, char *argv[])
{
    if (argv <= 1) {
        fprintf(2, "usage: sleep seconds\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}