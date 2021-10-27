#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        FILE *rfp = NULL, *wfp = NULL;

        int n =0;

        char rbuf[256] = {0,};
        char wbuf[256] = {0,};

        if(argc != 3) {
                printf("Usage : ./lseek [file name]\n");
                return 1;
        }
        rfp = fopen(argv[1], "r");
        if( rfp == NULL) {
                printf("open error!\n");
                return 1;
        }
        printf("데이터 파일 내용 : ");
        while(feof(rfp) == 0)
        {
                fread(rbuf, sizeof(char), 256, rfp);
                printf("%s", rbuf);
        }

        printf("\n");

        wfp = fopen(argv[2], "w");
        if(wfp == NULL) {
                printf("write error!\n");
                return 1;
        }

        fseek(rfp, 0, SEEK_SET);
        printf("출력 형태 : ");
        while( 0 < (n = fread(wbuf, sizeof(char), 1, rfp))){
                fwrite(wbuf, sizeof(char), 1, wfp);
                printf("%c", wbuf[0]);
                fseek(rfp, 1, SEEK_CUR);
        }


        fclose(rfp);
        fclose(wfp);

        return 0;
}
