#include <stdio.h>
#include <shadow.h>
#include <time.h>

int main(int argc, char *argv[]){
        struct spwd *spw;

        struct tm *tm;
        time_t t;


        char date[] = {
                "%G년 %m월 %d일"
        };
        char buf[256];

        if(argc != 2){
                printf("Usage : sudo ./PasswdChangeDate [User name]\n");
                return 1;
        }

        spw = getspnam(argv[1]);

        t = (time_t)spw->sp_lstchg;
        time(&t);
        tm = localtime(&t);

        strftime(buf, sizeof(buf), date, tm);
        printf("%s\n", buf);
        return 0;
}
