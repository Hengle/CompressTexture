#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>


void FreeImageSaveFile_16bit_Grayscale(int width, int height, unsigned short* plane, const char* name);
void FreeImageSaveFile_16bit_RGBA(int width, int height, unsigned short* plane, const char* name);
void FreeImageSaveFile_16bit_RGB(int width, int height, unsigned short* plane, const char* name);
void FreeImageSaveFile_8bit_RGBA(int width, int height, BYTE* plane, const char* name);

void FreeImageSetupRGB_SPLIT_min(int width, int height, unsigned short* plane, const char* name);
void FreeImageSetupRGB_SPLIT_max(int width, int height, unsigned short* plane, const char* name);
void FreeImageSetupRGBA_SPLIT_min(int width, int height, unsigned short* plane, const char* name);
void FreeImageSetupRGBA_SPLIT_max(int width, int height, unsigned short* plane, const char* name);


void FreeImageSaveFile_8bit_RGBA_4Image(int width, int height, BYTE* plane_R, BYTE* plane_G, BYTE* plane_B, BYTE* plane_A, const char* name);
void FreeImageSaveFile_16bit_RGBA_4Image(int width, int height, unsigned short* plane_R, unsigned short* plane_G, unsigned short* plane_B, unsigned short* plane_A, const char* name);
