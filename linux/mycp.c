#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        int wp, rp, n;
        char buf[256] ={0,};

        if(argc != 3) {
                printf("Usage : ./mycp [file name] [copy name]\n");
                return 1;
        }

        rp = open(argv[1], O_RDONLY);
        if(rp == -1) {
                printf("read open error!\n");
                return 1;
        }

        wp = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(wp == -1) {
                printf("wirte open error!\n");
                return 1;
        }

        while((n = read(rp, buf, sizeof(buf))) > 0)
                if(write(wp, buf, n) != n) printf("write error!\n");

        close(rp);
        close(wp);
        return 0;
}
