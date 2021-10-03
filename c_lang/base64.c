#include <stdio.h>
#include <string.h>

#define MAX_SIZE 10000

const char base64table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char find_base64_table[64] = "";

int Enter_string(char *base64_str, char *enter_str, int* enter_idx);
int encoding(int* base64_hex, char* encode, int *index);
int decoding();
int Print_encode(char* encode);
int Bin_to_Hex(int * base64, int * base64_hex, char * base64_str);
int Dec_to_Bin(char* base64_str, int *base64_bin);
int Reset_base64(int * bsae64_hex);
int Find_Flag(char * flag_str);
int main(){
    char enter_str[MAX_SIZE] ={0,};
    char base64_str[3] = "";
    char encode[MAX_SIZE] = {0,};

    int base64_bin[24] = {0,};
    int base64_hex[4] = {0,};

    int base_copy_idx, enter_idx=0, enter_len=0, encode_idx=0;

    int* intPtr= &encode_idx;
    int* entPtr = &enter_idx;
    int n=0;
    printf("1 : encode, 2: decode :");
    scanf("%d", &n);

    switch (n)
    {
    case 1:
        printf("enter str : ");
        scanf("%[^\n]", enter_str);    // %[^\n] 엔터빼고 다받기

        enter_len = strlen(enter_str);
        printf("enter_len : %d\n", enter_len);

        while(1)
        {
            Enter_string(base64_str, enter_str, entPtr);
            Dec_to_Bin(base64_str, base64_bin);
            Bin_to_Hex(base64_bin, base64_hex, base64_str);
            encoding(base64_hex, encode, intPtr);
            Reset_base64(base64_hex);
            if(enter_idx >= enter_len)break;
        }
        Print_encode(encode);
        break;
    case 2:
        decoding();
        break;
    default:
        printf("error!!\n");
        break;
    }

    
    return 0;
}

int Enter_string(char *base64_str, char *enter_str, int *enter_idx)
{
    int i=*enter_idx,j;
    // base64_str <- enter-str enter_str < 
    for(j =i%3; j<3; j++)       
    {
        base64_str[j] = enter_str[i];
        i++;
    }
    *enter_idx = i;
    return 0;
}

int encoding(int *base64_hex, char* encode, int *index)
{   //encoding func
    int i = *index, j;
    for(j =0; j <4; j++)
    {   
        if(base64_hex[j] == 0) encode[i++] = '=';
        else encode[i++] = base64table[(base64_hex[j])];
    }
    *index = i;
    return 0;
}

int decoding()
{
    int i,j, n=0,n2=0, m=0, temp=0;
    char enc_str[] = "7/OkZQIau/jou/R1by9acyjjutd0cUdlWshecQhkZUn1cUH1by9g4/9qNAn1byGaby9pbQSjWshgbUmqZAF+JtOBZUn1b8e1YoMPYoM1ny95ZAO+J/jaNAOB2vhrNLhVNDO0cshWNDIjbnrnZQhj4AM1S/Fmu/jou/GjN/n1bUm5JUFpNte1NyH1VA9yZUqLZQu13VR=";
    char dec_str[] = "Pepero is a cookie stick, dipped in compound chocolate, manufactured by ????? Confectionery in South Korea\nPepero Day is held annually on November 11";
    char flag_str[] = "S/jeutjaJvhlNA9Du/GaJBhLbQdjd+n1Jy9BcD3=";
    int base64_bin[24] = {0,};
    int base64_hex[4] = {0,};
    
    m = strlen(dec_str);
    while(n <= m){   // binary 넣고
        for(i = 0; i<3;i++){ 
            temp = dec_str[n++];
            for(j =(8*(i+1))-1;j>=(i*8);j--) {
                if (temp == 0)  {
                    base64_bin[j] = 0;
                    continue;
                }
                else{
                    base64_bin[j] = temp%2;
                    temp/=2;
                }
            }
        }
        temp=0;
        for(i =0; i<24;i++)
        {
            if(temp == 0 ) temp = 32;
            if(base64_bin[i] == 1) base64_hex[i/6] += temp;
            temp /=2;
        }

        for(i = 0; i <4;i++) find_base64_table[(base64_hex[i])] = enc_str[n2++];
        Reset_base64(base64_hex);
    }
    Find_Flag(flag_str);
    return 0;
}

int Find_Flag(char * flag_str)
{
    int i, j=0,m=0,k =0, idx=0;
    unsigned int temp=0;
    char flag[30] ="";
    int Flag_int[40] = {0,};
    int base64_bin[24] = {0,};
    int flag_idx=0;

    while(1){
        for(i=0;i < 64;i++){
            if(flag_str[j] == find_base64_table[i]) {
                Flag_int[j] = i;
                idx++;
                break;
            }
        }
        if(Flag_int[j] == 0) break;
        j++;
    }
    while(1){
        temp =0;
        for(i = 0; i<4;i++) { 
            temp = Flag_int[m++];
            for(j = (6*(i+1))-1; j>=(i*6); j--) {
                base64_bin[j] = temp%2;
                temp/=2;
            }
        }
        temp =128;
        i= 0;
        while(1)
        {
            if(flag_idx == 8)
            {
                printf("flag[%d] : %c\n",k, flag[k]);
                temp =128;
                k++;
                flag_idx =0;
            }
            if(base64_bin[i] == 1) flag[k] +=temp;
            i++;
            flag_idx++;
            temp/=2;
            if(i == 24) break;
        }
        if(m >= idx) break;
    }
    return 0;
}

int Print_encode(char* encode)
{
    int i=0;
    printf("base64 : ");
    while(1)
    {   // print endcode
        printf("%c",encode[i++]);
        if(encode[i] == '\0') break;
    }
    return 0;
}

// Bin -> Hex
int Bin_to_Hex(int * base64, int * base64_hex, char * base64_str)
{
    int i, temp=0;
    // print 
    printf("------------------------------------------------------\n\n");
    printf("Text content\t\t%c\t%c\t%c\n\nASCII\t\t\t%d\t%d\t%d\n\nBit pattern\t", base64_str[0], base64_str[1], base64_str[2], base64_str[0], base64_str[1], base64_str[2]);
    for(i =0; i<24;i++)
    {
        if(temp == 0 ) temp = 32;
        if(base64[i] == 1) base64_hex[i/6] += temp;
        temp /=2;
        printf("%d", base64[i]);
        if(((i+1)%6) == 0) printf("\t");
    }
    printf("\n\nindex\t\t%d\t  %d\t  %d\t  %d",base64_hex[0], base64_hex[1], base64_hex[2],base64_hex[3]);
    printf("\n\n------------------------------------------------------\n\n");

    return 0;
}

// base64_hex reset
int Reset_base64(int * base64_hex) { for(int i = 0; i<4; i++) base64_hex[i] = 0; return 0;}

int Dec_to_Bin(char * base64_str, int *base64_bin){
    int i, j, temp;
    // dec -> bin
    for(i = 0; i<3; i++){
        temp = base64_str[i];
        for(j = (8*(i+1))-1; j>=(i*8); j--){  
            if (temp == 0)  {
                base64_bin[j] = 0;
                continue;
            } 
            else 
            {
                base64_bin[j] = temp%2;
                temp /= 2;
            }
        }
    }
    return 0;
}