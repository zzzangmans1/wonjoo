#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        int fp=0;
        char buf[256]= {0,};
        int n;

        if(argc != 2) {
                printf("Usage : ./mycat [file name]\n");
                return 1;
        }

        fp = open(argv[1], O_RDONLY);

        if(fp == -1){
                printf("open error!\n");
                return 1;
        }

        while((n = read(fp, buf, sizeof(buf))) > 0)
                        printf("%s", buf);

        close(fp);

        return 0;
}
