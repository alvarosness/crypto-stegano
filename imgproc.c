#include "imgproc.h"

void headerReader(FILE* imageFile,char* filetype, int* height, int* width, int* grayscale)
{
	char auxHeight[10];
	char auxWidth[10];
	char auxGray[10];
	
	int ch;
	int i = 0;

	while ((ch = fgetc(imageFile)) != 0x0A)
	{
		*filetype = ch;
		filetype++;
	}
	*filetype = '\0';

	while ((ch = fgetc(imageFile)) != 0x20)
	{
		auxWidth[i] = ch;
		i++;
	}
	auxWidth[i] = '\0';
	*width = alphatoint(auxWidth);

	i = 0;
	while ((ch = fgetc(imageFile)) != 0x0A)
	{
		auxHeight[i] = ch;
		i++;
	}
	auxHeight[i] = '\0';
	*height = alphatoint(auxHeight);


	i = 0;
	while ((ch = fgetc(imageFile)) != 0x0A)
	{
		auxGray[i] = ch;
		i++;
	}
	auxGray[i] = '\0';
	*grayscale = alphatoint(auxGray);
}

/*! Function that will perform the copy of one image file to another one
 *
 */
int copyImage(char *fileName)	
{
	FILE* image;
	int c;	//< variable that will retain a copy of a byte of data that will later be written on the second file
	FILE* newImage;
	char filetype[3];
	int height;
	int width;
	int grayscale;
	char newFileName[50] = "copied/copyOf";

	strcat(newFileName,fileName);

	image = fopen(fileName,"rb");
	newImage = fopen(newFileName,"wb");
	
	if(newImage == NULL || image == NULL){
		puts("Unable to read file!");
		return FAIL;
	}

	headerReader(image, filetype, &height, &width, &grayscale);
	
	fprintf(newImage, "%s\n%d %d\n%d\n", filetype, width, height, grayscale);
	
	while ((c = fgetc(image)) != EOF )
	{
		fputc(c, newImage);
	}
	
	fclose(image);
	fclose(newImage);
	return SUCCESSFUL;
}

/*! Function that will read an image and write the inverted image in another file
 *
 */
int invertImage(char* fileName) 
{
	FILE* invertedImage;	//< file descriptor for the inverted image
	int c;					//< variable that will retain a byte from the image file
	FILE* image;
	char fileType[3];
	int height;
	int width;
	int grayscale;
	char newFileName[50] = "inverted/inverted";

	strcat(newFileName,fileName);

	image = fopen(fileName,"rb");
	invertedImage = fopen(newFileName,"wb");

	if (invertedImage == NULL || image == NULL){
		puts("Unable to read file!");
		puts("Terminating program...");
		return FAIL;
	}

	headerReader(image, fileType, &height, &width, &grayscale);

	fprintf(invertedImage,"%s\n%d %d\n%d\n",fileType,width,height,grayscale);

	while ((c = fgetc(image)) != EOF) 
	{
		c = grayscale - c;
		fputc(c, invertedImage);
	}
	
	fclose(image);
	fclose(invertedImage);
	return SUCCESSFUL;
}

/*! Function that will convert the grayscale image into a black and white image
*
*/
int bwImage(char* fileName)
{
	FILE* bwImage;		//< file descriptor for the black and white image
	int c;				//< byte retainer
	FILE* image;
	char fileType[3];
	int height;
	int width;
	int grayscale;
	char newFileName[50] = "blackandwhite/bw";

	strcat(newFileName, fileName);

	
	image = fopen(fileName,"rb");
	bwImage = fopen(newFileName, "wb");

	if (bwImage == NULL || fileName == NULL){
		puts("Unable to read file!");
		return FAIL;
	}

	headerReader(image, fileType, &height, &width, &grayscale);

	fprintf(bwImage, "%s\n%d %d\n%d\n", fileType, width, height, grayscale);


	while ((c = fgetc(image)) != EOF)
	{
		if (c > THRESHOLD)
			c = WHITE;
		else
			c = BLACK;
		fputc(c, bwImage);
	}
	
	fclose(image);
	fclose(bwImage);

	return SUCCESSFUL;
}

int createThumbprint(char* fileName, int shrinkFactor)
{
	FILE* thumbPrintFile;
	int sum = 0;
	char fileType[3];
	int height;
	int width;
	int grayscale;
	int newWidth;
	int newHeight;
	short** image;
	short** thumbPrint;
	FILE* imageFile;
	char newFileName[50] = "thumbprints/thumbprintOf";

	strcat(newFileName, fileName);
	
	imageFile = fopen(fileName,"rb");
	thumbPrintFile = fopen(newFileName, "wb");
	
	if (thumbPrintFile == NULL || imageFile == NULL){
		puts("Cannot open file!");
		return FAIL;
	}

	headerReader(imageFile, fileType, &height, &width, &grayscale);

	image = (short**)malloc(height*sizeof(short*));
	for (int i = 0; i < height; i++) 
	{
		*(image + i) = (short*)malloc(width*sizeof(short));
	}

	newHeight = height / shrinkFactor;
	newWidth = width / shrinkFactor;

	thumbPrint = (short**)malloc(newHeight*sizeof(short*));
	for (int i = 0; i < newHeight; i++)
	{
		*(thumbPrint + i) = (short*)malloc(newWidth*sizeof(short));
	}


	for (int i = 0;i < height;i++)
		for (int j = 0;j < width;j++)
			*(*(image + i) + j) = fgetc(imageFile);
		


	for (int m = 0; m < newHeight; m++)
	{
		for (int n = 0; n < newWidth; n++) 
		{
			for (int i = m * shrinkFactor; i < (m + 1) * shrinkFactor; i++)
			{
				for (int j = n * shrinkFactor; j < (n + 1) * shrinkFactor; j++)
				{
					sum += image[i][j];
				}
			}
			thumbPrint[m][n] = sum / (shrinkFactor * shrinkFactor);
			sum = 0;
		}
	}

	fprintf(thumbPrintFile, "%s\n%d %d\n%d\n",fileType,newWidth,newHeight,grayscale);
	
	for (int i = 0; i < newHeight; i++)
		for (int j = 0; j < newWidth; j++)
			fputc(thumbPrint[i][j],thumbPrintFile);
	
	fclose(imageFile);
	fclose(thumbPrintFile);
	return SUCCESSFUL;
}

int expandImage(char* imageFileName) 
{
	FILE* expandedImageFile;
	short* image;
	short** expandedImage;
	char fileType[3];
	int height;
	int width;
	int grayscale;
	FILE* imageFile;
	char newFileName[50] = "expanded/expanded";


	imageFile = fopen(imageFileName, "rb");
	if(imageFile == NULL)
		return FAIL;

	headerReader(imageFile, fileType, &height, &width, &grayscale);
	
	image = (short*)malloc(width*sizeof(short));
	expandedImage = (short**)malloc(2*sizeof(short*));

	for (int i = 0; i < 2; i++)
	{
		*(expandedImage + i) = (short*)malloc(2*width*sizeof(short));
	}

	strcat(newFileName, imageFileName);

	expandedImageFile = fopen(newFileName, "wb");

	if (expandedImageFile == NULL)
		return FAIL;

	fprintf(expandedImageFile, "%s\n%d %d\n%d\n",fileType,2*width,2*height,grayscale);

	while (!feof(imageFile))
	{
		//Reading one line from the original image
		for (int i = 0;i < width;i++)
			image[i] = fgetc(imageFile);

		//mapping each pixel of the original image to the expanded image
		for (int i = 0; i < width; i++)
			for (int m = 0; m < 2; m++)
				for (int n = 2 * i; n < 2 * (i + 1);n++)
					expandedImage[m][n] = image[i];

		//writing the expanded image on the file
		for (int m = 0; m < 2; m++)
			for (int n = 0; n < 2*width; n++)
				fputc(expandedImage[m][n], expandedImageFile);
	}
	
	fclose(imageFile);
	fclose(expandedImageFile);

	return SUCCESSFUL;
}

//MSE
float MSE(char* imageFileName, char* otherImageFileName)
{
	float total = 0;
	int pixelOne;
	int pixelTwo;
	char typeOne[3];
	int heightOne;
	int widthOne;
	int grayscaleOne;
	char typeTwo[3];
	int heightTwo;
	int widthTwo;
	int grayscaleTwo;

	FILE* imageFile;
	FILE* otherImageFile;

	imageFile = fopen(imageFileName, "rb");
	otherImageFile = fopen(otherImageFileName, "rb");

	if(imageFile == NULL || otherImageFile == NULL){
		puts("Unable to open file...");
		return 0;
	}


	headerReader(imageFile, typeOne, &heightOne, &widthOne, &grayscaleOne);
	headerReader(otherImageFile, typeTwo, &heightTwo, &widthTwo, &grayscaleTwo);

	if(widthOne != widthTwo || heightOne != heightTwo){
			puts("Dimensions do not match...");
			return 0;
	}

	for (int i = 0; i < heightOne; i++)
	{
		for (int j = 0; j < widthOne; j++)
		{
			pixelOne = fgetc(imageFile);
			pixelTwo = fgetc(otherImageFile);

			total += (pixelOne - pixelTwo)*(pixelOne - pixelTwo);
		}
	}

	return total / (widthOne*heightOne);
}

float PSNR(char* imageFileName, char* otherImageFileName)
{
	return 20 * log10f(WHITE) - 10 * log10f(MSE(imageFileName, otherImageFileName));
}
