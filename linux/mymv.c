#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        int wfp, rfp, n;

        char buf[256] ={0,};

        if(argc != 3) {
                printf("Usage : ./mymv [file name] [change name]\n");
                return 1;
        }

        rfp = open(argv[1] , O_RDONLY);
        if(rfp == -1){
                printf("read error!\n");
                return 1;
        }

        wfp = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(wfp == -1){
                printf("write error!\n");
                return 1;
        }

        while((n = read(rfp, buf, sizeof(buf))) > 0)
                if(write(wfp, buf, n)) break;

        if((remove(argv[1])) == 0) printf("del Success!\n");
        else printf("del fail!\n");

        close(rfp);
        close(wfp);
        return 0;
}
