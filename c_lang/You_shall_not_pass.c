#include <stdio.h>

int dec[] = {
    0x94, 0x1b, 0x0E, 0x1b, 
0x22, 0x19, 0x0A, 0x1E,
0x30, 0x21, 0x17, 0x0F, 
0x13, 0x2b, 0x2e, 0x1E,
0x17, 0x0F, 0x13, 0x2b, 
0x21, 0x22, 0x3C, 0x36, 
0x31, 0x2f, 0x2a, 0x2a, 
0x33, 0x50, 0x3f, 0x89
};
int flag[32] ={0,};     // 1단계 디코딩
int end_flag[32]= {0,}; // 2단계 디코딩
/*
    0x21 ~ 0x7E 입력할 수 있는 값
*/
int main(){
    int i=0,j=2, f0=0x44, f1=0x48, f2=0x7B,f31=0x7D,temp =0, temp1=0, temp2=0;    // DH{ 맨 앞 입력 3글자 맨뒤 }
    temp1 = f0^f1;
    temp2 = f1^f2;
    flag[31] = f31;
    flag[0] = temp1;
    flag[1] = temp2;
    for(i =29;i>=0;i--){
        flag[j] = dec[i] - temp2;
        temp2 = flag[j];
        j++;
    }
    end_flag[0] = f0;
    end_flag[1] = f1;
    end_flag[31] = f31;
    printf("%c%c",end_flag[0],end_flag[1]);
    for(i = 1;i<31;i++) {            
        temp = end_flag[i];
        for(j=0x20;j<0x7E;j++)
        {
            if((temp ^ j) == flag[i]){
                end_flag[i+1] = j;
                printf("%c",end_flag[i+1]);
                break;
            }
        }
    }
    return 0;
}
