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

uint8_t crc_data[CRC_SIZE] = {      // crc_data + salt
    0x47,0x5b,0x48,0x65,0x6c,0x6c,0x6f,0x5f,0x5f,0x5f,0x57,0x65,0x6c,0x63,0x6f,0x6d
        ,0x4f,0x20,0x54,0x6f,0x20,0x52,0x65,0x76,0x65,0x72,0x73,0x69,0x6e,0x67,0x2e,0x4b
        ,0x54,0x5d,0x5f,0x5f,0x54,0x68,0x65,0x20,0x69,0x64,0x65,0x61,0x20,0x6f,0x66,0x20
        ,0x4f,0x68,0x65,0x20,0x61,0x6c,0x67,0x6f,0x72,0x69,0x74,0x68,0x6d,0x20,0x63,0x61
        ,0x48,0x65,0x20,0x6f,0x75,0x74,0x20,0x6f,0x66,0x20,0x74,0x68,0x65,0x20,0x63,0x6f
        ,0x45,0x65,0x65,0x6e,0x67,0x6e,0x20,0x63,0x68,0x61,0x6c,0x6c,0x65,0x6e,0x67,0x65
        ,0x4c,0x5f,0x54,0x68,0x69,0x73,0x20,0x61,0x6c,0x67,0x6f,0x72,0x69,0x74,0x68,0x6d
        ,0x4c,0x76,0x65,0x72,0x79,0x20,0x46,0x58,0x43,0x4b,0x5f,0x5f,0x42,0x75,0x74,0x20
        ,0x79,0x6f,0x75,0x20,0x63,0x61,0x6e,0x20,0x73,0x6f,0x6c,0x76,0x65,0x20,0x69,0x74
        ,0x21,0x21,0x5f,0x5f,0x49,0x6d,0x70,0x6f,0x73,0x73,0x69,0x62,0x6c,0x65,0x20,0x69
        ,0x73,0x20,0x49,0x6d,0x70,0x6f,0x73,0x73,0x69,0x62,0x6c,0x65,0x5f,0x28,0x29,0x5f
        ,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d,0x5f,0x28
        ,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d
        ,0xe7,0x51,0xde,0x35,0xa3,0x13,0x90,0x2e,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f
        ,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d,0x5f,0x28
        ,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x5d,0x5f,0x28,0x29,0x5f,0x5b,0x0 };

uint64_t crc64_ecma_poly = 0xC96C5795D7870F42;
uint64_t crc_table[CRC_SIZE] = { 0, };
uint8_t crc_table_idx[CRC_SIZE] = { 0, };

uint64_t crc64_checksum = 0x676F5F675F695F6C; // crc64_checksum
uint8_t start = 0xFF;
uint8_t end = 0x80;

// Function
int32_t Init_crc_table();
uint8_t Find_crc_table_idx(uint64_t);
uint64_t In_crc64();
uint64_t Un_crc64(uint64_t);
int32_t Compare(const void*, const void*);

int main() {
    
    Init_crc_table();
    crc64_checksum = Un_crc64(crc64_checksum);  // 0xFF ~ 0x80 reverse crc64 checksum
    com_crc_data* map1 = (com_crc_data*)malloc(MAX_SIZE * sizeof(com_crc_data));
    com_crc_data* map2 = (com_crc_data*)malloc(MAX_SIZE * sizeof(com_crc_data));

    com_crc_data* com1 = map1;
    // name[0] ~ name[3] forward
    //uint8_t c = 0x39;
    printf("\nCRC64 name[0] ~ nmae[3] forward\n\n");
    for (int8_t a = 0x20; a < 0x7F; a++) {
        for (int8_t b = 0x20; b < 0x7F; b++) {
            for (int8_t c = 0x20; c < 0x7F; c++) {
                for (int8_t d = 0x20; d < 0x7F; d++) {
                    crc_data[0] = a;
                    crc_data[0x10] = b;
                    crc_data[0x20] = c;
                    crc_data[0x30] = d;
                    com1->crc = In_crc64();
                    com1->value = a + 0x100 * b + 0x10000 * c + 0x1000000 * d;
                    com1++;
                }
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
                for (int8_t d = 0x20; d < 0x7F; d++) {
                    crc_data[0x40] = a;
                    crc_data[0x50] = b;
                    crc_data[0x60] = c;
                    crc_data[0x70] = d;
                    com2->crc =  Un_crc64(crc64_checksum);
                    com2->value = a + 0x100 * b + 0x10000 * c + 0x1000000 * d;
                    com2++;
                }
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
