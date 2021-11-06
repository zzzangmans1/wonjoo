#include <stdio.h>
#include <Windows.h>

/* FLAG 
660C 4C86 A62C 1C9C
1C66 1C2C 9C6C A6CC
A66C 6CAC A6A6 864C
2C46 EC8C EC46 8C9C
4CEC C666 4C46 864C
*/
DWORD FLAG[] = { 0x4C86660C, 0x1C9CA62C,
    0x1C2C1C66, 0xA6CC9C6C,
    0x6CACA66C, 0x864CA6A6,
    0xEC8C2C46, 0x8C9CEC46,
    0xC6664CEC, 0x864C4C46
    };
int main(){
    DWORD a1 = 0x0;
    int i=0, j=0;
    for(i=0x0;i<0xFFFFFFFF;i++)
    {
        a1 = i;
        a1 =(((16*((4*((2*a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555)) & 0xCCCCCCCC | (((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555) >> 2) & 0x33333333)) & 0xF0F0F0F0 | (((4 * ((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555)) & 0xCCCCCCCC | (((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555) >> 2) & 0x33333333) >> 4) & 0xF0F0F0F) << 8) & 0xFF00FF00 | (((16 * ((4 * ((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555)) & 0xCCCCCCCC | (((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555) >> 2) & 0x33333333)) & 0xF0F0F0F0 | (((4 * ((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555)) & 0xCCCCCCCC | (((2 * a1) & 0xAAAAAAAA | (a1 >> 1) & 0x55555555) >> 2) & 0x33333333) >> 4) & 0xF0F0F0F) >> 8) & 0xFF00FF;
        if(a1 == FLAG[j])
        {
            printf("%d :  %X\n",j+1, i);
            j++;
            i=0x0;
        }
        if(j > 10 ) break;
    }
    return 0;
}
