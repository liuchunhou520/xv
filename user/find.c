#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void
find(char *path, char *tar)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strcmp(path, tar) == 0) {
        printf("%s\n",path);
    }

    switch(st.type){
        case T_FILE:
            printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
            break;

        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0){
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{

    if (argc == 1) {
        printf("usage: find [path] [target]\n");
        exit(0);
    }
    int i;

    if(argc == 2){
        find(".", argv[1]);
        exit(0);
    }

    if (argc == 3) {
        find(argv[1], argv[2]);
        exit(0);
    }

    exit(0);
}
