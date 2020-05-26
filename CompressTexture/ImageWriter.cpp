#include "ImageWriter.h"

//unsigned short(16bit) array 로 된 image raw data(grayscale)을 받아 출력한다.
void FreeImageSaveFile_16bit_Grayscale(int width, int height, unsigned short* plane, const char* name) {

	int w = width; int h = height;

	//16bit grayscale
	FIBITMAP* image = FreeImage_AllocateT(FIT_UINT16, w, h, 16);
	for (int y = 0; y < FreeImage_GetHeight(image); y++) {
		unsigned short* bits = (unsigned short*)FreeImage_GetScanLine(image, y);
		for (int x = 0; x < FreeImage_GetWidth(image); x++) {
			bits[x] = plane[x + y * w];
		}
	}

	FreeImage_Save(FIF_PNG, image, name);
	FreeImage_Unload(image);
}

//unsigned short(16bit) array 로 된 image raw data(grayscale)을 받아 RGBA 이미지로 출력한다.
void FreeImageSaveFile_16bit_RGBA(int width, int height, unsigned short* plane, const char* name)
{
	//16bit rgba
	int w = width; int h = height; int c = 4;
	FIBITMAP* image = FreeImage_AllocateT(FIT_RGBA16, w, h, 16 * c);
	for (int y = 0; y < FreeImage_GetHeight(image); y++) {
		unsigned short* bits = (unsigned short*)FreeImage_GetScanLine(image, y);
		for (int x = 0; x < FreeImage_GetWidth(image); x++) {
			bits[x * c + 0] = plane[x + y * w]; //R
			bits[x * c + 1] = plane[x + y * w]; //G
			bits[x * c + 2] = plane[x + y * w]; //B
			bits[x * c + 3] = plane[x + y * w]; //A
			//bits[x * c + 3] = 65535;//A
		}
	}

	FreeImage_Save(FIF_PNG, image, name);
	FreeImage_Unload(image);
}

//unsigned short(16bit) array 로 된 image raw data(grayscale)을 받아 RGBA 이미지로 출력한다.
void FreeImageSaveFile_16bit_RGBA_4Image(int width, int height, unsigned short* plane_R, unsigned short* plane_G, unsigned short* plane_B, unsigned short* plane_A, const char* name)
{
	//16bit rgba
	int w = width; int h = height; int c = 4;
	FIBITMAP* image = FreeImage_AllocateT(FIT_RGBA16, w, h, 16 * c);
	for (int y = 0; y < FreeImage_GetHeight(image); y++) {
		unsigned short* bits = (unsigned short*)FreeImage_GetScanLine(image, y);
		for (int x = 0; x < FreeImage_GetWidth(image); x++) {
			bits[x * c + 0] = plane_R[x + y * w]; //R
			bits[x * c + 1] = plane_G[x + y * w]; //G
			bits[x * c + 2] = plane_B[x + y * w]; //B
			bits[x * c + 3] = plane_A[x + y * w]; //A
			//bits[x * c + 3] = 65535;//A
		}
	}

	FreeImage_Save(FIF_PNG, image, name);
	FreeImage_Unload(image);
}



//unsigned short(16bit) array 로 된 image raw data(grayscale)을 받아 RGB 이미지로 출력한다.
void FreeImageSaveFile_16bit_RGB(int width, int height, unsigned short* plane, const char* name)
{
	//16bit rgb
	int w = width; int h = height; int c = 3;
	FIBITMAP* image = FreeImage_AllocateT(FIT_RGBA16, w, h, 16 * c);
	for (int y = 0; y < FreeImage_GetHeight(image); y++) {
		unsigned short* bits = (unsigned short*)FreeImage_GetScanLine(image, y);
		for (int x = 0; x < FreeImage_GetWidth(image); x++) {
			bits[x * c + 0] = plane[x + y * w]; //R
			bits[x * c + 1] = plane[x + y * w]; //G
			bits[x * c + 2] = plane[x + y * w]; //B
		}
	}

	FreeImage_Save(FIF_PNG, image, name);
	FreeImage_Unload(image);
}

//BYTE(8bit) array 로 된 image raw data(grayscale)을 받아 RGBA 이미지로 출력한다.
void FreeImageSaveFile_8bit_grayscale(int width, int height, BYTE* plane, const char* name)
{

	int w = width; int h = height; int c = 1;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		src[i * c] = plane[i];//gray
	}

	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;
}

//BYTE(8bit) array 로 된 image raw data(grayscale)을 받아 RGBA 이미지로 출력한다.
void FreeImageSaveFile_8bit_RGBA(int width, int height, BYTE* plane, const char* name)
{

	int w = width; int h = height; int c = 4;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		src[i * c + 0] = plane[i];//R
		src[i * c + 1] = plane[i];//G
		src[i * c + 2] = plane[i];//B
		src[i * c + 3] = plane[i];//A
	}

	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;
}

//BYTE(8bit) array 로 된 image raw data(grayscale) 4개를 받아 각각을 한 채널씩 RGBA 이미지로 출력한다.
void FreeImageSaveFile_8bit_RGBA_4Image(int width, int height, BYTE* plane_R, BYTE* plane_G, BYTE* plane_B, BYTE* plane_A, const char* name)
{

	int w = width; int h = height; int c = 4;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		src[i * c + 0] = plane_B[i];//B
		src[i * c + 1] = plane_G[i];//G
		src[i * c + 2] = plane_R[i];//R
		src[i * c + 3] = plane_A[i];//A
	}

	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;
}


void FreeImageSetupRGB_SPLIT_min(int width, int height, unsigned short* plane, const char* name)
{

	int w = width; int h = height; int c = 3;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		BYTE r, g, b;
		b = plane[i] & 31;	//하위 5bit
		g = (plane[i] & 0x7E0) >> 5;//중위 6bit
		r = (plane[i] & 0xF800) >> 11;//상위 5bit

		unsigned short sum = b + (g << 5) + (r << 11);

		if (sum != plane[i])
			printf("error!!\n");

		src[i * c + 0] = b;//B
		src[i * c + 1] = g;//G
		src[i * c + 2] = r;//R
	}

	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;

}
void FreeImageSetupRGB_SPLIT_max(int width, int height, unsigned short* plane, const char* name)
{

	int w = width; int h = height; int c = 3;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		BYTE r, g, b;
		b = plane[i] & 31;	//하위 5bit
		g = (plane[i] & 0x7E0) >> 5;//중위 6bit
		r = (plane[i] & 0xF800) >> 11;//상위 5bit

		b = b << 3;
		g = g << 2;
		r = r << 3;

		unsigned short sum = ((unsigned short)b >> 3) + ((unsigned short)g << 3) + ((unsigned short)r << 8);

		if (sum != plane[i])
			printf("error!!\n");

		src[i * c + 0] = b;//B
		src[i * c + 1] = g;//G
		src[i * c + 2] = r;//R
	}

	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;

}



void FreeImageSetupRGBA_SPLIT_min(int width, int height, unsigned short* plane, const char* name)
{

	int w = width; int h = height; int c = 4;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		BYTE r, g, b, a;
		b = plane[i] & 0xF;	//하위 4bit
		g = (plane[i] & 0xF0) >> 4;//중하위 4bit
		r = (plane[i] & 0xF00) >> 8;//중상위 4bit
		a = (plane[i] & 0xF000) >> 12;//상위 4bit

		unsigned short sum = b + (g << 4) + (r << 8) + (a << 12);

		if (sum != plane[i])
			printf("error!!\n");

		src[i * c + 0] = b;//B
		src[i * c + 1] = g;//G
		src[i * c + 2] = r;//R
		src[i * c + 3] = a;//A
	}

	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;

}
void FreeImageSetupRGBA_SPLIT_max(int width, int height, unsigned short* plane, const char* name)
{

	int w = width; int h = height; int c = 4;
	FIBITMAP * bitmap = FreeImage_Allocate(w, h, c);
	BYTE * src = new BYTE[w*h*c];
	for (int i = 0; i < w*h; i++) {
		BYTE r, g, b, a;
		b = plane[i] & 0xF;	//하위 4bit
		g = (plane[i] & 0xF0) >> 4;//중하위 4bit
		r = (plane[i] & 0xF00) >> 8;//중상위 4bit
		a = (plane[i] & 0xF000) >> 12;//상위 4bit

		b = b << 4;
		g = g << 4;
		r = r << 4;
		a = a << 4;


		unsigned short sum = ((unsigned short)b >> 4) + ((unsigned short)g ) + ((unsigned short)r << 4) + ((unsigned short)a << 8);

		if (sum != plane[i])
			printf("error!!\n");

		src[i * c + 0] = b;//B
		src[i * c + 1] = g;//G
		src[i * c + 2] = r;//R
		src[i * c + 3] = a;//A
	}



	FIBITMAP* Image = FreeImage_ConvertFromRawBits(src, w, h, w*c, 8 * c, 0, 0, 0, false);
	FreeImage_Save(FIF_PNG, Image, name);
	FreeImage_Unload(Image);
	delete src;

}
