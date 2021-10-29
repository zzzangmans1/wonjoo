#include <stdlib.h>

int main(int argc, char** argv){
        unsigned int res =0;

        for(int i=2; i<argc; i++){
                int arg = 0;
                arg = (argv[i][0] -0x30)*10 + argv[i][1]-0x30;
                if(arg % 3 ==0)
                        res += arg;
                else
                        res += arg*2;
        }
        res %= 100;
        return res;
}
