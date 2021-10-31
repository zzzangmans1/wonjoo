#include <stdio.h>
#include <string.h>

unsigned long long get_d(unsigned long long a, unsigned long long b);
unsigned long long get_p_q(unsigned long long n);
unsigned long long get_dec(unsigned long long n, unsigned long long d, unsigned long long enc);

unsigned char flag[4];

int main(){
    unsigned long long n = 4271010253;
    unsigned long long e = 201326609;
    unsigned long long p;
    unsigned long long q;
    unsigned long long d;

    unsigned long long tmp;
    int j =0;
    printf("n = %llu\n", n);
    p = get_p_q(n);
    q = n/p;
    unsigned long long phi = (p-1) * (q-1);
    printf("q = %llu\n", q);
    printf("phi = %llu\n", phi);
    d = get_d(e, phi);

    // 암호화된 값
    unsigned long long enc[] = { 0xd4dce140, 0xa183dfe2,
    0xc363e306, 0xf9d2e268, 0xc41a2409, 0x2a9bc0fb, 0x9a4e22b5,
    0x7d38ef9a, 0xb15f929f, 0x9ed67bef, 0x99cdeae7
    };
    printf("flag = ");
    for(int i=0;i < 11;i++)
    {
        tmp = get_dec(n,d,enc[i]);
        memcpy(flag, &tmp,4);
        printf("%c%c%c%c",flag[0],flag[1],flag[2],flag[3]);
    }
    return 0;
}

// d 구하는 함수
unsigned long long get_d(unsigned long long e, unsigned long long phi)
{
    unsigned long long i =2;
    while(1){
        if(i*e%phi == 1) break;
        i++;
    }
    printf("d = %llu\n",i);
    return i;
}

// n 소인수 분해하여 p q 구하는 함수
unsigned long long get_p_q(unsigned long long n)
{
    unsigned long long i = 2;
    while(1){
        if(n % i == 0) break;
        i++;
    }
    printf("p = %llu\n", i);
    return i;
}

// RSA 복호화 함수
unsigned long long get_dec(unsigned long long n, unsigned long long d, unsigned long long enc){
    unsigned long long temp = 1;
    for(unsigned long long i = 0;i<d;++i) {
        temp *= enc;
        temp %=n;
    }
    return temp;
}
