/*

$ ./mvdir test55                                                                                       
 pwd : /home/kali/source/mid_ex3/test55
                                                                                                                    
$ ls                                                                                                        
dirclear    mvdir    mychmod   mychmod1.c  mystat    test    test.txt
dirclear.c  mvdir.c  mychmod1  mychmod.c   mystat.c  test55
   

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]){
        char *cwd;
        char wd[BUFSIZ];
        if(argc != 2) {
                printf("Usage : ./mvdir [dir name]\n");
                return 1;
        }

        if(mkdir(argv[1], 0755) == -1){
                printf("mkdir error!\n");
                return 1;
        }

        cwd = getcwd(NULL, BUFSIZ);
        chdir(argv[1]);
        getcwd(wd, BUFSIZ);
        printf(" pwd : %s\n", wd);

        return 0;
}
