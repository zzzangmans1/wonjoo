#include <stdio.h>

// secret message wargame
// Run Length Encoding

#pragma warning (disable : 4996)

#define MAX_SIZE		1000
int main(void)
{
	FILE* fr = NULL;
	FILE* fw =NULL;

	int str[MAX_SIZE] = { 0, };

	int prev_val = 0x0;

	int cnt = 0x0;
	int i = 0;
	int curr_val = 0;
	fr = fopen("secretMessage.enc", "r+");
	if (fr == NULL) {
		printf("file read error!!\n");
		return -1;
	}
	fw = fopen("secretMessage.raw", "w+");
	if (fw == NULL) {
		printf("file write error!!\n");
		return -1;
	}
	while(1)
	{
		curr_val = fgetc(fr);
		if (curr_val == -1) break;
		fputc(curr_val, fw);
		if (curr_val == prev_val)	// 앞 뒤 같으면
		{
			cnt = fgetc(fr);
			for (i = 0x0; i < cnt; i++)	fputc(prev_val, fw); // 어차피 앞 뒤 같아서 prev_val
		}
		else						// 앞 뒤 다르면
		{
			prev_val = curr_val;
		}
	}
	fclose(fr);
	fclose(fw);
	return 0;
}