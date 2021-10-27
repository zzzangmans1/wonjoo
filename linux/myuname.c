#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
        struct utsname uts;

        if(argc != 2){
                printf("Usage : ./myuname [ option ]\n");
                return 1;
        }

        if(uname(&uts) == -1){
                printf("uname error!\n");
                return 1;
        }

        if(!strcmp(argv[1], "-a")){
                printf("%s ", uts.sysname);
                printf("%s ", uts.nodename);
                printf("%s ", uts.release);
                printf("%s ", uts.version);
                printf("%s\n", uts.machine);
        }
        else if(!strcmp(argv[1], "-s")){
                printf("OS : %s\n", uts.sysname);
        }
        else if(!strcmp(argv[1], "-n")){
                printf("host : %s\n", uts.nodename);
        }
        else if(!strcmp(argv[1], "-r")){
                printf("release : %s\n", uts.release);
        }
        else if(!strcmp(argv[1], "-v")){
                printf("version : %s\n", uts.version);
        }
        else if(!strcmp(argv[1], "-m")){
                printf("Hardware : %s\n", uts.machine);
        }
        return 0;
}
