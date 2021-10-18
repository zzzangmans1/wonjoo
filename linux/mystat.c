/*

./mystat test.txt                                       
파일명 : test.txt
inode번호 : 307031
파일의 종류 : 일반 파일
접근권한 : rw-r--r--
UID : 1000
파일 수정 시각 : 1634529476

*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]){
        struct stat buf;

        if( argc != 2) {
                printf("Usage : ./mystat [file name]\n");
                return 1;
        }

        stat(argv[1] , &buf);
        printf("파일명 : %s\n", argv[1]);
        printf("inode번호 : %d\n", (int)buf.st_ino);
        printf("파일의 종류 : ");
        switch(buf.st_mode & S_IFMT)
        {
                case S_IFREG : printf("일반 파일\n");
                               break;
                case S_IFDIR : printf("디렉 터리\n");
                               break;
                case S_IFIFO : printf("FIFO 파일\n");
                               break;
                case S_IFLNK : printf("링크 파일\n");
                               break;
        }
        printf("접근권한 : ");
        for(int i=0;i<3;i++){
                if(buf.st_mode &(S_IREAD >> i*3))
                        printf("r");
                else    printf("-");
                if(buf.st_mode &(S_IWRITE >> i*3))
                        printf("w");
                else    printf("-");
                if(buf.st_mode &(S_IEXEC >> i*3))
                        printf("x");
                else    printf("-");
        }
        printf("\nUID : %d\n", (int)buf.st_uid);
        printf("파일 수정 시각 : %d\n", (int)buf.st_mtime);


        return 0;
}
