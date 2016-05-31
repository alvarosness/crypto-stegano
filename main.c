#include "imgproc.h"
#include "crypto.h"
#include "stegano.h"
#include "utilities.h"
#include <string.h>

char getToken(char *text);

int main(int argc, char *argv[])
{
	float* value;
	switch(getToken(argv[1])){
		case 'i':
			switch(getToken(argv[2])){
				case 'c':
					copyImage(argv[3]);
				break;

				case 'i':
					invertImage(argv[3]);
				break;

				case 'b':
					bwImage(argv[3]);
				break;

				case 't':
					createThumbprint(argv[3], atoi(argv[4]));
				break;

				case 'e':
					expandImage(argv[3]);
				break;

				case 'p':
					value = (float*)malloc(sizeof(float));
					*value = PSNR(argv[3], argv[4]);
					printf("PSNR: %.2f\n", *value);
					free(value);
				break;
			}
		break;

		case 'c':
			switch(getToken(argv[2])){
				case 'e':
					visualEncrypt(argv[3]);
				break;

				case 'd':
					visualDecrypt(argv[3], argv[4]);
				break;

				case 'E':
					encryptCarrierImage(argv[3], argv[4]);
				break;

				case 'D':
					decryptCarrierImage(argv[3], argv[4]);
				break;
			}
		break;

		case 's':
			switch(getToken(argv[2])){
				case 'w':
					newWatermark(argv[3], argv[4], atoi(argv[5]));
				break;

				case 'e':
					extractWatermark(argv[3], atoi(argv[4]));
				break;
			}
		break;
	}

	return 0;
}

char getToken(char *text){
	if(text[0] != '-')
		return 0;
	return text[1];
}
