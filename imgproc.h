/*
	Image Processing Functionalities
	Written by Alvaro Esperanca
*/

#ifndef IMGPROC_H
#define IMGPROC_H

#include <stdio.h>
#include "utilities.h"
#include "consts.h"

int copyImage(char* imageFileName);
int invertImage(char* imageFileName);
int bwImage(char* imageFileName);
int createThumbprint(char* imageFileName, int shrinkFactor);
int expandImage(char* imageFileName);
void headerReader(FILE* imageFile, char* filetype, int* height, int* width, int* grayscale);
float MSE(char* imageFile, char* otherImageFile);
float PSNR(char* imageFile, char* otherImageFile);

#endif
