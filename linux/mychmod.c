/*
-rwxrwxrwx 1 kali kali    11 Oct 18 12:57 test.txt
                                                                                                                    
$ ./mychmod1 0111 test.txt                                                                                    3 ⚙
                                                                                                            
$ ls -al                                                                                                      3 ⚙

---x--x--x 1 kali kali    11 Oct 18 12:57 test.txt
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
 
int main(int argc, char *argv[]){
        struct stat buf;

        if(argc != 3){
                printf("Usage : ./mychmod [mode] [file name]\n");
                return 1;
        }

        stat(argv[2], &buf);

        buf.st_mode |= S_IRWXU;
        if(argv[1][1] == '6'){
                buf.st_mode &= ~(S_IXUSR);
        }
        else if(argv[1][2] == '5'){
                buf.st_mode &= ~(S_IWUSR);
        }
        else if(argv[1][2] == '4'){
                buf.st_mode &= ~(S_IWUSR);
                buf.st_mode &= ~(S_IXUSR);
        }
        else if(argv[1][2] == '3'){
                buf.st_mode &= ~(S_IRUSR);
        }
        else if(argv[1][2] == '2'){
                buf.st_mode &= ~(S_IRUSR);
                buf.st_mode &= ~(S_IXUSR);
        }
        else if(argv[1][2] == '1'){
                buf.st_mode &= ~(S_IRUSR);
                buf.st_mode &= ~(S_IWUSR);
        }
        else if(argv[1][2] == '0'){
                buf.st_mode &= ~(S_IRUSR);
                buf.st_mode &= ~(S_IWUSR);
                buf.st_mode &= ~(S_IXUSR);
        }

        buf.st_mode |= S_IRWXG;
        if(argv[1][2] == '6'){
                buf.st_mode &= ~(S_IXGRP);
        }
        else if(argv[1][2] == '5'){
                buf.st_mode &= ~(S_IWGRP);
        }
        else if(argv[1][2] == '4'){
                buf.st_mode &= ~(S_IWGRP);
                buf.st_mode &= ~(S_IXGRP);
        }
        else if(argv[1][2] == '3'){
                buf.st_mode &= ~(S_IRGRP);
        }
        else if(argv[1][2] == '2'){
                buf.st_mode &= ~(S_IRGRP);
                buf.st_mode &= ~(S_IXGRP);
        }
        else if(argv[1][2] == '1'){
                buf.st_mode &= ~(S_IRGRP);
                buf.st_mode &= ~(S_IWGRP);
        }
        else if(argv[1][2] == '0'){
                buf.st_mode &= ~(S_IRGRP);
                buf.st_mode &= ~(S_IWGRP);
                buf.st_mode &= ~(S_IXGRP);
        }

        buf.st_mode |= S_IRWXO;
        if(argv[1][3] == '6'){
                buf.st_mode &= ~(S_IXOTH);
        }
        else if(argv[1][2] == '5'){
                buf.st_mode &= ~(S_IWOTH);
        }
        else if(argv[1][2] == '4'){
                buf.st_mode &= ~(S_IWOTH);
                buf.st_mode &= ~(S_IXOTH);
        }
        else if(argv[1][2] == '3'){
                buf.st_mode &= ~(S_IROTH);
        }
        else if(argv[1][2] == '2'){
                buf.st_mode &= ~(S_IROTH);
                buf.st_mode &= ~(S_IXOTH);
        }
        else if(argv[1][2] == '1'){
                buf.st_mode &= ~(S_IROTH);
                buf.st_mode &= ~(S_IWOTH);
        }
        else if(argv[1][2] == '0'){
                buf.st_mode &= ~(S_IROTH);
                buf.st_mode &= ~(S_IWOTH);
                buf.st_mode &= ~(S_IXOTH);
        }

        chmod(argv[2], buf.st_mode);

        return 0;
}
