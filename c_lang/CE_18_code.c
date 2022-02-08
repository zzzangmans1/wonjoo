#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

/*
  CodeEngn advnaced 18 code
/*

uint8_t key[0x2000];
uint32_t tmp_buffer[0x1000] = { 0, };
uint32_t seed = 0x0;
uint8_t tmp_data1[32] = "";
uint8_t tmp_data2[32] = "";
uint8_t tmp_data3[32] = "";
uint8_t name_pad[0x20] = {
	0x43,0x6F,0x64,0x65,0x45,
	0x6E,0x67,0x6E,0
};
uint8_t calc_value1[8] = "";
uint8_t calc_value2[8] = "";
uint8_t calc_value3[8] = "";

uint8_t name[9] = "CodeEngn";
uint8_t tname[9] = "";
uint8_t fname[11] = "19.dat";

uint32_t name_check();
uint32_t file_check(uint32_t*, uint32_t*);
uint32_t check_data(size_t, size_t);
uint32_t NameToSeed();
uint32_t input_tmp_data(uint8_t*);
uint32_t Lcg();
uint32_t swap(uint8_t*, size_t);
uint32_t name_add_data();
uint32_t calc(uint8_t*, uint8_t*, uint8_t*);
uint32_t sub_401D6E(uint8_t*, uint8_t*, size_t, uint32_t*, uint8_t*);

uint32_t BitTest(size_t, size_t);
uint32_t BitTestSet(size_t, size_t);
uint32_t Rol(uint32_t, size_t);
uint32_t ByteToDword(uint8_t*);

uint32_t main()
{
	int jmp = 1;
	uint32_t mok = 0;
	uint32_t key_size = 0;
	//printf("name : ");
	//scanf("%s", name);
	for (size_t i = 0x0; i < 9; i++) tname[i] = name[i];

	//printf("filename : ");
	//scanf("%s", fname);
	if (name_check()) {
		if (file_check(&mok, &key_size)) {
			if (check_data(key_size, mok)) {
				NameToSeed();
				input_tmp_data(tmp_data1);
				printf("\ttmp_data1\n");
				for (size_t j = 0x0; j < 32; j++) {
					printf("%X ",tmp_data1[j]);
					if (jmp++ == 8) {
						printf("\n");
						jmp = 1;
					}
				}
				printf("\n");
				input_tmp_data(tmp_data2);
				printf("\ttmp_data2\n");
				jmp = 1;
				for (size_t j = 0x0; j < 32; j++) {
					printf("%X ", tmp_data2[j]);
					if (jmp++ == 8) {
						printf("\n");
						jmp = 1;
					}
				}
				printf("\n");
				input_tmp_data(tmp_data3);
				printf("\ttmp_data3\n");
				jmp = 1;
				for (size_t j = 0x0; j < 32; j++) {
					printf("%X ", tmp_data3[j]);
					if (jmp++ == 8) {
						printf("\n");
						jmp = 1;
					}
				}
				printf("\n");
				for (size_t i = 0x0; i < 0xA; i++) {
					name_add_data();
					printf("name_pad : ");
					for (size_t j = 0x0; j < 0x8; j ++) {
						printf("%X ", name_pad[j]);
					}
					printf("\n");
					calc(name_pad, tmp_data1, calc_value1);
					printf("calc_val1 : ");
					for (size_t j = 0x0; j < 0x8; j ++) {
						printf("%X ", calc_value1[j]);
					}
					printf("\n");
					calc(calc_value1, tmp_data2, calc_value2);
					printf("calc_val2 : ");
					for (size_t j = 0x0; j < 0x8; j ++) {
						printf("%X ", calc_value2[j]);
					}
					printf("\n");
					calc(calc_value2, tmp_data3, calc_value1);
					printf("calc_val1 : ");
					for (size_t j = 0x0; j < 0x8; j ++) {
						printf("%X ", calc_value1[j]);
					}
					printf("\n");
					/*
					printf("\nCalc_value1 : ");
					for (size_t j = 0x0; j < 0x8; j++) printf("0x%X, ", calc_value1[j]);
					*/
					sub_401D6E(name_pad, key, mok, tmp_buffer, calc_value3);
					/*
					printf("\nCalc_value3 : ");
					for (size_t j = 0x0; j < 0x8; j++) printf("0x%X ", calc_value3[j]);
					*/
					if (memcmp(calc_value1, calc_value3, 8)) {
						printf("\nInvalid Keyfile!\n");
						return 0;
					}
					printf("i :%X\n", i+1);
				}
			}
		}
	}
	printf("\n*** Congratulations! Your key is correct! ***\n");
	return 0;
}


uint32_t name_check() {
	uint32_t len = strlen((char*)name);
	if (len == 0) {
		printf("Please enter a name!n");
		return 0;
	}

	if (len < 6) {
		printf("Invalid name!\n");
		return 0;
	}

	int8_t j = 7;
	for (size_t i = strlen((char*)name); i < 0x20; i++) {	// Name ADD Padding
		name_pad[i] = name[j--];
		if (j < 0) j = 7;
	}
	for (size_t i = 0x0; i < 0x20; i++) {
		printf("%X ", name_pad[i]);
	}
	printf("\n*** name_check func clear ***\n");
	return 1;
}

uint32_t file_check(uint32_t* mok, uint32_t* file_size) {
	if (!fname) {
		printf("Please enter the path to a kerfile!\n");
		return 0;
	}
	FILE* fp = fopen((char*)fname, "r");
	if (fp == NULL) {
		printf("Cannot open keyfile!\n");
		return 0;
	}

	fseek(fp, 0, SEEK_END);
	uint32_t fsize = ftell(fp);
	if (fsize == NULL) return 0;
	fseek(fp, 0, SEEK_SET);
	printf("fsize : %X\n", fsize);
	if (fsize % 0xC != 0) {
		printf("Invalid Keyfile!\n");
		return 0;
	}
	*mok = fsize / 0xC;
	*file_size = fsize;

	fread(key, 1, fsize , fp);
	fclose(fp);
	if (!key) return 0;
	else {
		printf("\n*** file_check func clear ***\n");
		return 1;
	}
}

uint32_t check_data(size_t file_size, size_t mok) {
	size_t first_check = 0, second_check = 0, third_check = 0, index = 0x0, max = mok + 0x40;
	printf("mok : %X\tmax : %X\n", mok, max);
	memset(tmp_buffer, 0, 0x1000);
	for (size_t i = 0x0; i < mok; i++) {
		first_check = ByteToDword(key + index);
		index += 4;
		if (first_check >= max)	return 0;
		
		second_check = ByteToDword(key + index);
		index += 4;
		if (second_check >= max) return 0;
		
		third_check = ByteToDword(key + index);
		index += 4;
		if (third_check >= max) return 0;
		if (((tmp_buffer[third_check >> 5] >> (third_check & 0x1F)) & 1) == 1) {
			return 0;
		}
		tmp_buffer[third_check >> 5] |= 1 << (third_check & 0x1F);
		
	}
	if (tmp_buffer[0] != 0 || tmp_buffer[1] != 0) return 0;
	for (size_t i = 0x2; i < ((mok >> 5) + 2); i++) {
		if (tmp_buffer[i] != 0xFFFFFFFF) return 0;
	}
	uint32_t rol_res = (0x80000000 >> ((mok & 0x1F) - 1)) | 0x80000000;
	if ((mok & 0x1F) != 0) {
		if (tmp_buffer[2] != Rol(rol_res, mok & 0x1F)) {
			printf("tmp_buffer[2] : %X\t Rol : %X\n", tmp_buffer[2], Rol(rol_res, mok & 0x1F));
			return 0;
		}
	}
	memset(tmp_buffer, 0, 0x1000);
	for (size_t i = 0x0; i < 0x2; i++) tmp_buffer[i] = 0xFFFFFFFF;
	index = 0;
	for (size_t i = 0x0; i < mok; i++) {
		first_check = ByteToDword(key + index);
		index += 4;
		if (((tmp_buffer[first_check >> 5] >> (first_check & 0x1F)) & 1) == 0) return 0;
		second_check = ByteToDword(key + index);
		index += 4;
		if (((tmp_buffer[second_check >> 5] >> (second_check & 0x1F)) & 1) == 0) return 0;
		third_check = ByteToDword(key + index + 8);
		index += 4;
		tmp_buffer[third_check >> 5] |= 1 << (third_check & 0x1F);
	}
	printf("\n*** check_data func clear ***\n");
	return 1;
}

// name 값으로 seed 생성
uint32_t NameToSeed() {
	for (size_t i = 0x0; i < 0x20; i += 4) {
		uint32_t tmp_name = ByteToDword(name_pad + i);
		seed += tmp_name;
	}
	return 0;
}

uint32_t input_tmp_data(uint8_t* tmp) {
	uint8_t i = 8, j = 0;
	do {
		tmp[j] = --i;
		tmp[j + 8] = i;
		tmp[j + 16] = i;
		j++;
	} while (i);

	for (i = 0; i < 0x64; i++) {
		swap(tmp, 0);
		swap(tmp, 8);
		swap(tmp, 16);
	}
	i = 0x8, j = 0x0;
	do {
		tmp[j + 24] = Lcg() % 6;
		j++;
		i--;
	} while (i);

	//for (i = 0; i <32; i++) printf("tmp_data : %X\n", tmp[i]);
	printf("\n*** input_tmp_data func clear ***\n");
	return 0;
}

// Linear Congruential Generator 선형 합동 생성기 난수 생성기
uint32_t Lcg() {
	seed = 0x343FD * seed + 0x269EC3;
	uint32_t res = seed & 0x7FFF0000;
	return res >>= 0x10;
}

uint32_t swap(uint8_t* tmp, size_t idx) {
	uint8_t temp = 0;
	size_t n1 = Lcg() & 7;
	size_t n2 = Lcg() & 7;
	temp = tmp[idx + n1];
	tmp[idx + n1] = tmp[idx + n2];
	tmp[idx + n2] = temp;
	return 0;
}

uint32_t name_add_data() {
	size_t i = 0x20, j = 0x1F;
	uint16_t res = 0xAC, tmp;
	do {
		res = (res + name_pad[j]);
		tmp = res;
		res <<= 8;
		res |= (tmp >> 8) & 0xFF;
		name_pad[j--] = res & 0xFF;
		i--;
	} while (i);
	//printf("\n*** name_add_data func clear ***\n");
	return 0;
}

uint32_t calc(uint8_t* tmp_data, uint8_t* tmp, uint8_t* calc_value) {
	for (size_t i = 0x0; i < 8; i++) {
		uint8_t result = tmp_data[tmp[i]];
		uint8_t value = tmp_data[tmp[i + 8]];
		switch (tmp[i + 24]) {
		case 0:
			result += value;
			break;
		case 1:
			result -= value;
			break;
		case 2:
			result *= value;
			break;
		case 3:
			if (result > value) result = value;
			break;
		case 4:
			if (result < value) {
				result = value;
			}
			break;
		case 5:
			result ^= value;
			break;
		}
		calc_value[tmp[i + 16]] = result;
	}
	return 0;
}

uint32_t sub_401D6E(uint8_t* name_tmp_data, uint8_t* file_data, size_t mok, uint32_t* buffer, uint8_t* calc_value) {
	size_t index = 0x0, check;
	buffer[0] = ByteToDword(name_tmp_data);
	buffer[1] = ByteToDword(name_tmp_data + 4);
	for (size_t i = 0x0; i < mok; i++) {
		check = 0;
		uint32_t first = ByteToDword(file_data + index);
		index += 4;
		check = Rol(check, 1) | ((buffer[first >> 5] >> first & 0x1F) & 1);
		uint32_t second = ByteToDword(file_data + index);
		index += 4;
		check = Rol(check, 1) | ((buffer[second >> 5] >> second & 0x1F) & 1);
		uint32_t third = ByteToDword(file_data + index);
		//printf("Buffer[2] : %08X Buffer[3] : %08X\n", buffer[2], buffer[3]);
		index += 4;
		if (check == 3) {
			buffer[third >> 5] &= ~(1 << (third & 0x1F)) & 0xFFFFFFFF;
		}
		else buffer[third >> 5] |= 1 << (third & 0x1F);
		//printf("buffer[%X] : %X\n",third >> 5, buffer[third >> 5]);
	}
	size_t j = 2, idx = 0x0;
	for (size_t i = 0x0; i < 0x8; i++) {
		calc_value[i] = (buffer[j] >> idx) & 0xFF;
		idx += 0x8;
		if (i == 3) {
			idx = 0x0;
			j++;
		}
	}
	return 0;
}

uint32_t Rol(uint32_t data, size_t cnt) {
	size_t bit = 32;
	return ((data << cnt) | (data >> (bit - cnt)));
}
uint32_t ByteToDword(uint8_t* byte) {
	uint32_t result = 0x0, idx = 0x1;
	for (size_t i = 0x0; i < 0x4; i++) {
		result += byte[i] * idx;
		idx *= 0x100;
	}
	return result;
}
