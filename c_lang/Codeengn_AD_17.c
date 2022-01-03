#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
    CRC64_ECMA
    1. crc 테이블 세팅           
    2. crc 데이터 세팅           
    3. 0~ 0x39  까지 CRC 정방향
    4. 0xFF~ 0x40 CRC 역방향
    5. 둘 체크
*/

#define MAX_SIZE 95*95*95*95        // 95^4 
#define CRC_SIZE 0x100              // table, data size

// Struct
typedef struct _com_crc_data{
    uint64_t crc;
    uint32_t value;
} com_crc_data;

// Variable 
uint8_t crc_data[] =
"      GF(2^64)[X]/(x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 + x^46 + x^45 + x^40 + x^39 +"
" x^38 + x^37 + x^35 + x^33 + x^32 + x^31 + x^29 + x^27 + x^24 + x^23 + x^22 + x^21 + x^19 + x^17 + x^1"
"3 + x^12 + x^10 + x^9 + x^7 + x^4 + x^1 + 1)       ";


uint64_t crc64_ecma_poly = 0xC96C5795D7870F42;
uint64_t crc_table[CRC_SIZE] = { 0, };
uint8_t crc_table_idx[CRC_SIZE] = { 0, };

uint64_t crc64_checksum = 0x6963636972706163;
uint8_t start = 0xFF;
uint8_t end = 0x80;

// Function
int32_t Init_crc_table();
uint8_t Find_crc_table_idx(uint64_t);
uint64_t In_crc64();
uint64_t Un_crc64(uint64_t);
int32_t Compare(const void*, const void*);

int main() {
    //*(uint64_t*)&crc_data[0x20] = 0x66BB16A328F74639;   // crc_data_salt (serial) Name.Length == 8
    *(uint64_t*)&crc_data[0x1C] = 0x66BB16A328F74639;   // crc_data_salt (serial) Name.Length == 7
    Init_crc_table();
    
    crc64_checksum = Un_crc64(crc64_checksum);  // 0xFF ~ 0x80 reverse crc64 checksum
    com_crc_data* map1 = (com_crc_data*)malloc(MAX_SIZE * sizeof(com_crc_data));
    com_crc_data* map2 = (com_crc_data*)malloc(MAX_SIZE * sizeof(com_crc_data));

    com_crc_data* com1 = map1;
    // name[0] ~ name[3] forward
    printf("\nCRC64 name[0] ~ nmae[3] forward\n\n");
    for (int8_t a = 0x20; a < 0x7F; a++) {
        for (int8_t b = 0x20; b < 0x7F; b++) {
            for (int8_t c = 0x20; c < 0x7F; c++) {
                crc_data[0] = a;
                crc_data[0x10] = b;
                crc_data[0x30] = c;
                com1->crc = In_crc64();
                com1->value = a + 0x100 * b + 0x10000 * 0x20 + 0x1000000 * c;
                com1++;
            }
        }
    }
    com_crc_data* com2 = map2;
    // name[4] ~ name[7] reverse
    printf("\nCRC64 name[4] ~ name[7] reverse\n\n");
    start = 0x7F;
    end = 0x40;
    for (int8_t a = 0x20; a < 0x7F; a++) {
        for (int8_t b = 0x20; b < 0x7F; b++) {
            for (int8_t c = 0x20; c < 0x7F; c++) {
                //for (int8_t d = 0x20; d < 0x7F; d++) {       // crc_data_salt (serial) Name.Length == 8 
                    crc_data[0x40] = a;
                    crc_data[0x50] = b;
                    crc_data[0x60] = c;
                    //crc_data[0x70] = d;
                    com2->crc =  Un_crc64(crc64_checksum);
                    com2->value = a + 0x100 * b + 0x10000 * c; //+ 0x1000000; * d;
                    com2++;
                //}
            }
        }
    }
    // compare data sort
    printf("\nCRC64 data qsort\n\n");
    qsort(map1, MAX_SIZE, sizeof(com_crc_data), Compare);
    qsort(map2, MAX_SIZE, sizeof(com_crc_data), Compare);
    com1 = map1;
    com2 = map2;
    printf("\nCRC64 data compare\n\n");
    while (1) {
        if (com1 - map1 > MAX_SIZE)
            break;
        if (com2 - map2 > MAX_SIZE)
            break;
        if (com1->crc > com2->crc) {
            com2++;
        }
        else if (com1->crc < com2->crc) {
            com1++;
        }
        else {
            uint8_t flag[9] = { 0 };
            *(uint32_t*)&flag[0] = com1->value;
            *(uint32_t*)&flag[4] = com2->value;
            printf("flag = %s\n", flag);
            break;
        }
    }
    free(map1);
    free(map2);
    return 0;
}

int32_t Init_crc_table()    // Init crc table Func
{
    for (size_t i = 0; i < 0x100; i++)
    {
        uint64_t result = i;
        for (size_t j = 0; j < 8; j++)
        {
            if (result & 1) {
                result >>= 1;
                result ^= 0xC96C5795D7870F42;
            }
            else {
                result >>= 1;
            }
        }
        crc_table[i] = result;
        crc_table_idx[result >> 0x38] = (uint8_t)i;
    }
    return 0;
}

uint8_t Find_crc_table_idx(uint64_t crc) {
    return crc_table_idx[(crc >> 0x38)];
}

uint64_t In_crc64()     // in crc64
{
    uint64_t ret_in_crc = 0;
    for (size_t i = 0x0; i < 0x40; i++) {
        uint8_t idx = (uint8_t)(ret_in_crc^crc_data[i]);
        ret_in_crc = crc_table[idx]^(ret_in_crc >> 8);
    }
    return ret_in_crc;
}

uint64_t Un_crc64( uint64_t crc)     // un crc64
{
    for (size_t i = start; i >= end; i--) {
        uint8_t idx = Find_crc_table_idx(crc);
        crc ^= crc_table[idx];
        crc <<= 8;
        crc |= idx ^ crc_data[i];
    }
    return crc;
}

int32_t Compare(const void* first, const void* second) // sort Func
{
    if (((const com_crc_data*)first)->crc > ((const com_crc_data*)second)->crc)
        return 1;
    else if (((const com_crc_data*)first)->crc < ((const com_crc_data*)second)->crc)
        return -1;
    else
        return 0;
}
