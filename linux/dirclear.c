/*

$ ls -al                                                                                                      

drwxr-xr-x 2 kali kali  4096 Oct 18 14:57 test
drwxr-xr-x 2 kali kali  4096 Oct 18 14:56 test1

                                                                                                                    
$ ./dirclear test1                                                                                  
디렉토리 안의 파일이 없습니다！
                                                                                                                    
$ ls -al                                                                                         

drwxr-xr-x 2 kali kali  4096 Oct 18 14:57 test

                                                                                                                    
$ ./dirclear test                                                                                       
디렉토리　안의 파일이 있습니다！        

$ ls -al
drwxr-xr-x 2 kali kali  4096 Oct 18 14:57 test

*/

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
        DIR *dp;
        struct dirent *dent;

        if(argc != 2){
                printf("Usage : ./dirclear [dir name]\n");
                return 1;
        }

        if( (dp = opendir(argv[1])) == NULL){
                printf("opendir error!\n");
                return 1;
        }

        dent = readdir(dp);
        if(((int)dent->d_off) > 0) printf("디렉토리　안의 파일이 있습니다！\n");
        else {
                printf("디렉토리 안의 파일이 없습니다！\n");
                rmdir(argv[1]);
        }
        closedir(dp);
        return 0;
}
