#ifndef CRYPTO_H
#define CRYPTO_H

#include "imgproc.h"
#include "consts.h"

int visualEncrypt(char* imageFile);
int encryptCarrierImage(char* payloadFile, char* carrierFile);
int visualDecrypt(char* shareOneFileName, char* shareTwoFileName);
int decryptCarrierImage(char* shareOneFileName, char* shareTwoFileName);

#endif
