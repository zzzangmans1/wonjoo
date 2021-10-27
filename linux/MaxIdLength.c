#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){

        if(argc != 2){
                printf("Usage : ./MaxIdLength : [ id ]\n");
                return 1;
        }

        int len = strlen(argv[1]);
        if(len > 256) {
                printf("MAX Login Name Length : %ld\n", sysconf(_SC_LOGIN_NAME_MAX));
                printf("Login Name Length : %d\n", len);
                printf("로그인 닉네임 길이가 너무 깁니다. \n");
        }
        else {
                printf("Max Login Name Length : %ld\n", sysconf(_SC_LOGIN_NAME_MAX));
                printf("Login Name Length : %d\n", len);
        }
        return 0;
}
