#include <stdio.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char* argv[]){
        struct passwd *pw;

        struct group *grp;

        if(argc != 2){
                printf("Usage : ./UserToGroup [ User Name ]\n");
                return 1;
        }

        pw = getpwnam(argv[1]);

        grp = getgrgid(pw->pw_gid);
        printf("Group Name : %s\n", grp->gr_name);

        return 0;
}
