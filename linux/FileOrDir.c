/*

$ ls -al ./test                               
-rw-r--r-- 1 kali kali    0 Oct 18 15:59 mvtest.txt
drwxr-xr-x 2 kali kali 4096 Oct 18 16:23 test123
-rw-r--r-- 1 kali kali   10 Oct 18 14:57 test.txt
                                                                                                                    
$ ./FileOrDir test                                                                                      
mvtest.txt는 일반파일
test.txt는 일반파일
test123는 디렉터리

*/
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        DIR *dp;
        struct dirent *dent;

        struct stat sbuf;

        if(argc != 2){
                printf("Usage : ./FileOrDir [dir name]\n");
                return 1;
        }

        if((dp =opendir(argv[1])) == NULL){
                printf("opendir error!\n");
                return 1;
        }
        chdir(argv[1]);
        while((dent = readdir(dp))) {
                if(dent->d_name[0] == '.') continue;
                stat(dent->d_name, &sbuf);
                switch(sbuf.st_mode & S_IFMT){
                        case S_IFREG : printf("%s는 일반파일\n",dent->d_name);
                                       break;
                        case S_IFDIR : printf("%s는 디렉터리\n",dent->d_name);
                                       break;
                }
        }
        closedir(dp);
        return 0;
}
