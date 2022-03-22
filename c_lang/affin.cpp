#include <stdio.h>
// 아핀 암호 암호화 복호화
#pragma warning (disable : 4996)

int affinEnc();
int affinDec();

int main() {
	affinEnc();
	affinDec();
	return 0;
}

int affinEnc() {
	char plain, cipher;
	int key1 = 11, key2 = 4;

	FILE* fr = fopen("plain.txt", "r");
	if (fr == NULL) {
		printf("file read error!\n");
		return 1;
	}

	FILE* fw = fopen("cipher.txt", "w");
	if (fw == NULL) {
		printf("file write error!\n");
		return 1;
	}

	while (plain = fgetc(fr))
	{
		if (feof(fr)) break;
		if (plain == 0x20) continue;
		plain -= 0x61;			// 소문자를 숫자로 변경
		cipher = (plain * key1 + key2) % 26;
		cipher += 0x41;			// 숫자를 대문자로 변경
		fputc(cipher, fw);
		printf("%c", cipher);
	}
	printf("\n");
	fclose(fr);
	fclose(fw);
	return 0;
}

int affinDec() {
	char plain, cipher;
	int key1 = 19, key2 = 22;

	FILE* fr = fopen("cipher.txt", "r");
	if(fr == NULL){
		printf("file read error!\n");
		return 1;
	}
	/*
	FILE* fw = fopen("plain.txt", "w");
	if (fw == NULL) {
		printf("file write error!\n");
		return 1;
	}
	*/
	while (cipher = fgetc(fr)) {
		if (feof(fr)) break;
		cipher -= 0x41;			// 대문자 숫자로 변경
		plain = ((cipher + key2) * key1) % 26;
		plain += 0x61;			// 숫자를 소문자로 변경
		printf("%c", plain);
	}
	fclose(fr);
	//fclose(fw);
	return 0;
}
