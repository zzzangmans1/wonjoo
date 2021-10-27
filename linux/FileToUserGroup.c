#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[]){
        struct stat info;

        struct passwd *pw;

        struct group *grp;

        if (argc != 2){
                printf("Usage : ./FileToUserGroup [file name]\n");
                return 1;
        }

        stat(argv[1], &info);

        pw = getpwuid(info.st_uid);
        printf("uid : %s\n", pw->pw_name);

        grp = getgrgid(info.st_gid);
        printf("gid : %s\n", grp->gr_name);
        return 0;
}
