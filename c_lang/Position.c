#include <stdio.h>

int main() {
    int p = 0x70,i, j;
    for( i = 0x61; i < 0x7B ;i++)
    {
        for( j = 0x61; j < 0x7B ;j++)
        {
            if( ((i & 1) + 5) + ( ((j & 4) != 0) + 1) == 7 ){
                if ( (((i & 8) != 0) + 5) + (((j & 8) != 0) + 1) == 6){
                    if ( (((i  & 2) != 0) + 5) + (((j & 0x10) != 0) + 1) == 8){
                        if ( (((i & 4) != 0) + 5) + ((j & 1) + 1) == 7){
                            if ( (((i & 0x10) != 0) + 5) + (((j & 2) != 0) + 1) == 6) {
                                printf("%c%c",i, j);
                                break;
                            }
                        }
                    }
                }
            }
        }
        if ( (((i & 0x10) != 0) + 5) + (((j & 2) != 0) + 1) == 6) break;
    }
    for( i = 0x61; i < 0x7B ;i++)
    {
        if( ((((i & 0x10) != 0) + 5) + ((p & 2) != 0) + 1) == 6 ){
            if( (((i & 4) != 0) + 5) + ((0x70& 1) + 1) ==  7){
                if(  (((i & 2) != 0) + 5) + (((0x70 & 0x10) != 0) + 1) == 7 ){
                    if( (((i & 8) != 0) + 5) + (((0x70 & 8) != 0) + 1) == 7 ){
                        if( ((i & 1) + 5) +(((0x70 & 4) != 0) + 1) == 7){
                            printf("%c%c\n", i,p);
                        }
                    }
                }
            }
        } 
    }
    return 0;
}