#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>


char *output[]= {               // time
        "%h %e %R"
};

int main(void) {
        char *cwd;              // current directory
        char wd[BUFSIZ];

        DIR *dp;                // Directroy information
        struct dirent *dent;

        struct stat buf;        // file struct

        cwd = getcwd(NULL, BUFSIZ);

        struct passwd *pw;      // uid 
        struct group *grp;      // gid

        time_t timer;           // time
        struct tm *tm;
        char buf1[257];

        int color =0;           // color check

        char sym_str[BUFSIZ];
        int sym;

        if((dp = opendir(cwd)) == NULL) {
                printf("opendir : %s\n", cwd);
                return -1;
        }

        while((dent = readdir(dp))) {
                lstat(dent->d_name, &buf);      // 파일이름　따와서　정보를　buf에　삽입
                color =0;
                switch(buf.st_mode & S_IFMT)
                {
                        case S_IFREG : printf("-");
                                       if((int)buf.st_mode > 0x81a4) {
                                               color =7;
                                               break;
                                       }
                                       color = 1; 
                                       break;
                        case S_IFDIR : printf("d"); color = 2; break;
                        case S_IFIFO : printf("p"); color = 3; break;
                        case S_IFLNK : printf("l"); color = 4; break;
                        case S_IFCHR : printf("-"); color = 5; break;
                        case S_IFBLK : printf("-"); color = 5; break;
                        case S_IFSOCK : printf("s"); color = 6; break;

                }
                for(int i=0;i<3;i++){
                         if(buf.st_mode & (S_IREAD >> i*3))
                                printf("r");
                         else   printf("-");
                         if(buf.st_mode & (S_IWRITE >> i*3))
                                printf("w");
                         else   printf("-");
                         if(buf.st_mode & (S_IEXEC >> i*3))
                                printf("x");
                         else   printf("-");
                }
                printf(" %d ", (int)buf.st_nlink);
                pw = getpwuid((uid_t)buf.st_uid);
                printf(" %s ", pw->pw_name);
                grp = getgrgid((gid_t)buf.st_gid);
                printf(" %s ", grp->gr_name);
                printf(" %5d\t", (int)buf.st_size);
                timer = (time_t)buf.st_mtime;           // mtime : 마지막으로　변경한　시각　저장
                tm = localtime(&timer);
                strftime(buf1, sizeof(buf1), output[0], tm);
                printf(" %s  ", buf1);
                switch(color){
                        case 1 : printf(" %s \n", dent->d_name);
                                 break;
                        case 2 : printf(" \033[;1;34m%s\033[0m\n", dent->d_name);
                                 break;
                        case 3 : printf(" \033[;40;33m%s\033[0m\n", dent->d_name);
                                 break;
                        case 4 : printf(" \033[;1;36m%s\033[0m", dent->d_name);
                                 printf(" -> ");
                                 sym = readlink(dent->d_name, sym_str, BUFSIZ);
                                 sym_str[sym] ='\0';
                                 printf(" \033[;1;32m%s\033[0m\n",sym_str);
                                 break;
                        case 5 : printf(" \033[40;33;01m%s\033[0m\n", dent->d_name);
                                 break;
                        case 6 : printf(" \033[;01;35m%s\033[0m\n", dent->d_name);
                                 break;
                        case 7 : printf(" \033[;1;32m%s\033[0m\n", dent->d_name);
                                 break;
                }
        }

        closedir(dp);

        return 0;
}