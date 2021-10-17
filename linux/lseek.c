/*
데이터 파일 내용 : abcdefghijklmnopqrstuvwxyz

출력 형태 : acegikmoqsuwy
*/
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        int fp, n;
        char buf[256] = {0,};

        if(argc != 2) {
                printf("Usage : ./lseek [file name]\n");
                return 1;
        }
        fp = open(argv[1], O_RDONLY);
        if( fp == -1) {
                printf("open error!\n");
                return 1;
        }
        printf("데이터 파일 내용 : ");
        while( (n = read(fp, buf, sizeof(buf))) > 0 )
                printf("%s", buf);
        printf("\n");

        lseek(fp, 0, SEEK_SET);
        printf("출력 형태 : ");
        while( (n = read(fp, buf, 1)) > 0 ){
                printf("%c", buf[0]);
                lseek(fp, 1, SEEK_CUR);
        }


        close(fp);

        return 0;
}
