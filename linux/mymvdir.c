/*
  $ ls -al                                                                                                      9 ⚙
-rw-r--r-- 1 kali kali     0 Oct 18 15:58 mvtest.txt

                                                                                                                    
$ ./mymv mvtest.txt test                                                                                      9 ⚙
작업디렉토리　: /home/kali/source/mid_ex3/test
현재　디렉토리 : /home/kali/source/mid_ex3
mvtest.txt 삭제 Sucess!
                                                                                                                    
$ ls -al                                                                                                      9 ⚙

$ cd test                                                                                                     9 ⚙
                                                                                                                    
$ ls -al                                                                                                      9 ⚙

-rw-r--r-- 1 kali kali    0 Oct 18 15:59 mvtest.txt

*/

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
        int wfp, rfp, n;

        char *cwd;
        char wd[BUFSIZ];

        char buf[BUFSIZ];

        if(argc != 3){
                printf("Usage : ./mymv [file name] [dir name]\n");
                return 1;
        }

        rfp = open(argv[1], O_RDONLY);
        if(rfp == -1) {
                printf("read open error!\n");
                return 1;
        }

        cwd = getcwd(NULL, BUFSIZ);
        chdir(argv[2]);
        getcwd(wd, BUFSIZ);
        printf("작업디렉토리　: %s\n", wd);

        wfp = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(wfp == -1){
                printf("write open error!\n");
                return 1;
        }

        while((n = read(rfp, buf, sizeof(buf))) > 0)
                if(write(wfp, buf, sizeof(buf)) != n) printf("write error!\n");

        chdir(cwd);
        cwd = getcwd(NULL, BUFSIZ);
        printf("현재　디렉토리 : %s\n", cwd);
        if(!remove(argv[1])) printf("%s 삭제 Sucess!\n", argv[1]);
        close(rfp);
        close(wfp);
        return 0;
}
