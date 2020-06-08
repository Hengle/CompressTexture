#include "ImageLoader.h"


__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))

//압축되지 않은 일반 이미지(jpg, png 등)을 로드하여 텍스처하는 함수
//리턴 : texname
GLuint load_unpack_image(const char *filename, GLuint ReadRGBAType) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLvoid *data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		//GPU에서 업로드하기 위해 비트맵 포맷으로 변환
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	if (ReadRGBAType == GL_RGBA8UI)
		glTexImage2D(GL_TEXTURE_2D, 0, ReadRGBAType, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, ReadRGBAType, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);

	return texname;
}

//압축되지 않은 일반 이미지(jpg, png 등)을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : 업로드 시간
float load_unpack_image_checktime(const char *filename, GLuint ReadRGBAType) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLvoid *data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	float compute_time;

	glFinish();
	CHECK_TIME_START;
	if (ReadRGBAType == GL_RGBA8UI)
		glTexImage2D(GL_TEXTURE_2D, 0, ReadRGBAType, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, ReadRGBAType, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glFinish();
	CHECK_TIME_END(compute_time);
	//fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);

	return compute_time;
}

//압축되지 않은 일반 이미지(jpg, png 등)을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : 업로드 시간
float load_unpack_image_16bit_checktime(const char *filename, GLuint ReadRGBAType) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_64;

	int width, height;
	GLvoid *data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 64)
		tx_pixmap_64 = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_64 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_64);
	height = FreeImage_GetHeight(tx_pixmap_64);
	data = FreeImage_GetBits(tx_pixmap_64);

	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	float compute_time;

	glFinish();
	CHECK_TIME_START;
	if(ReadRGBAType == GL_RGBA16UI)
		glTexImage2D(GL_TEXTURE_2D, 0, ReadRGBAType, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, ReadRGBAType, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, data);

	glFinish();
	CHECK_TIME_END(compute_time);
	//fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap_64);
	if (tx_bits_per_pixel != 64)
		FreeImage_Unload(tx_pixmap);

	return compute_time;
}


void test_loadimg(const char *filename_ori, const char *filename_upper, const char *filename_lower) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_64, *tx_pixmap_32[2];

	int width, height;
	BYTE *data_ori, *data_upper, *data_lower;

	tx_file_format = FreeImage_GetFileType(filename_ori, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename_ori);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 64)
		tx_pixmap_64 = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_64 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_64);
	height = FreeImage_GetHeight(tx_pixmap_64);
	data_ori = FreeImage_GetBits(tx_pixmap_64);


	tx_file_format = FreeImage_GetFileType(filename_upper, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename_upper);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32[0] = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32[0] = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32[0]);
	height = FreeImage_GetHeight(tx_pixmap_32[0]);
	data_upper = FreeImage_GetBits(tx_pixmap_32[0]);


	tx_file_format = FreeImage_GetFileType(filename_lower, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename_lower);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32[1] = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32[1] = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32[1]);
	height = FreeImage_GetHeight(tx_pixmap_32[1]);
	data_lower = FreeImage_GetBits(tx_pixmap_32[1]);

	unsigned short* oridata = new unsigned short[width * height *4];



	//for (int i = 0; i < 16 * 16 * 4; i+=4) {
	//	printf("[%d] - %d %d %d %d\n",i/4, (int)data_ori[i], (int)data_ori[i+1], (int)data_ori[i+2], (int)data_ori[i+3]);
	//	//oridata[i] = data_ori[i * 2] + data_ori[i * 2 + 1] * 256;
	//}
	//for (int i = 0; i < 16 * 16 * 4; i+=4) {
	//	printf("[%d] - %d %d %d %d\n",i/4, (int)data_upper[i], (int)data_upper[i+1], (int)data_upper[i+2], (int)data_upper[i+3]);
	//}

	for (int i = 0; i < width * height * 4; i++) {
		oridata[i] = data_ori[i * 2] + data_ori[i * 2 + 1] * 256;
	}
	//printf("original-16bit\n");
	//for (int i = 0; i < 4*4 * 4; i+=4) {
	//	printf("[%3d] - %04x %04x %04x %04x\n",i/4, (int)oridata[i], (int)oridata[i+1], (int)oridata[i+2], (int)oridata[i+3]);
	//}
	//printf("upper-8bit\n");
	//for (int i = 0; i < 4 * 4 * 4; i += 4) {
	//	printf("[%3d] - %04x %04x %04x %04x\n", i / 4, (int)data_upper[i]<<8, (int)data_upper[i + 1] << 8, (int)data_upper[i + 2] << 8, (int)data_upper[i + 3] << 8);
	//}
	//printf("lower-8bit\n");
	//for (int i = 0; i < 4 * 4 * 4; i += 4) {
	//	printf("[%3d] - %04x %04x %04x %04x\n", i / 4, (int)data_lower[i], (int)data_lower[i + 1], (int)data_lower[i + 2], (int)data_lower[i + 3]);
	//}

	for (int i = 0; i < width * height * 4; i++) {
		if (oridata[i] == data_upper[i] * 256 + data_lower[i]) {

		}
		else {
			printf("error\n");
		}

	}

	//float으로
	float* ori_float = new float[width * height * 4];
	float* upper_float = new float[width * height * 4];
	float* lower_float = new float[width * height * 4];
	for (int i = 0; i < width * height * 4; i++) {
		ori_float[i] = (float)oridata[i] / 65535;
		upper_float[i] = (float)data_upper[i] / 255;
		lower_float[i] = (float)data_lower[i] / 255;

		if (ori_float[i] > 1.0f || ori_float[i] < 0.0f) {
			printf("ori err\n");
		}
		if (upper_float[i] > 1.0f || upper_float[i] < 0.0f) {
			printf("upper err\n");
		}
		if (lower_float[i] > 1.0f || lower_float[i] < 0.0f) {
			printf("lower err\n");
		}
	}

	printf("\n\n float type\n");
	printf("  original-16bit\n");
	for (int i = 0; i < 4  ; i += 4) {
		printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, ori_float[i], ori_float[i + 1], ori_float[i + 2], ori_float[i + 3]);
	}
	printf("  upper-8bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, upper_float[i] , upper_float[i + 1] , upper_float[i + 2] , upper_float[i + 3]);
	}
	printf("  lower-8bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, lower_float[i], lower_float[i + 1], lower_float[i + 2], lower_float[i + 3]);
	}
	printf("  combine-16bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, (upper_float[i] * 255 + lower_float[i]) / 255, (upper_float[i + 1] * 255 + lower_float[i + 1]) / 255, (upper_float[i + 2] * 255 + lower_float[i + 2]) / 255, (upper_float[i + 3] * 255 + lower_float[i + 3]) / 255);
	}
	printf("  diffrence-16bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		float d1 = -ori_float[i] + ((upper_float[i] * 255 + lower_float[i])) / 255;
		float d2= -ori_float[i + 1] + ((upper_float[i + 1] * 255 + lower_float[i + 1]) / 255);
		float d3= -ori_float[i + 2] + ((upper_float[i + 2] * 255 + lower_float[i + 2]) / 255);
		float d4= -ori_float[i + 3] + ((upper_float[i + 3] * 255 + lower_float[i + 3]) / 255);
		printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, d1 / ori_float[i] * 100, d2 / ori_float[i + 1] * 100, d3 / ori_float[i + 2] * 100, d4 / ori_float[i + 3] * 100);
		//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	}

	printf("\n\n float type_2\n");
	printf("  original-16bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, (unsigned int)(ori_float[i] * 65535), (unsigned int)(ori_float[i + 1] * 65535), (unsigned int)(ori_float[i + 2] * 65535), (unsigned int)(ori_float[i + 3] * 65535));
	}
	printf("  upper-8bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, (unsigned int)(upper_float[i] * 255), (unsigned int)(upper_float[i + 1] * 255), (unsigned int)(upper_float[i + 2] * 255), (unsigned int)(upper_float[i + 3] * 255));
	}
	printf("  lower-8bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, (unsigned int)(lower_float[i] * 255), (unsigned int)(lower_float[i + 1] * 255), (unsigned int)(lower_float[i + 2] * 255), (unsigned int)(lower_float[i + 3] * 255));
	}
	printf("  combine-16bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		//unsigned int d1 = ((unsigned int)(upper_float[i] * 255) * 256 + unsigned int(lower_float[i] * 255));
		//unsigned int d2 = ((unsigned int)(upper_float[i+1] * 255) * 256 + unsigned int(lower_float[i + 1] * 255));
		//unsigned int d3 = ((unsigned int)(upper_float[i+2] * 255) * 256 + unsigned int(lower_float[i + 2] * 255));
		//unsigned int d4 = ((unsigned int)(upper_float[i+3] * 255) * 256 + unsigned int(lower_float[i + 3] * 255));
		//printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, d1, d2, d3, d4);

		unsigned int d1 = (unsigned int)((upper_float[i] * 255 + lower_float[i]) / 255 * 65535);
		unsigned int d2 = (unsigned int)((upper_float[i + 1] * 255 + lower_float[i + 1]) / 255 * 65535);
		unsigned int d3 = (unsigned int)((upper_float[i + 2] * 255 + lower_float[i + 2]) / 255 * 65535);
		unsigned int d4 = (unsigned int)((upper_float[i + 3] * 255 + lower_float[i + 3]) / 255 * 65535);
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, d1, d2, d3, d4);
		//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, (upper_float[i] * 255 + lower_float[i]) / 255, (upper_float[i + 1] * 255 + lower_float[i + 1]) / 255, (upper_float[i + 2] * 255 + lower_float[i + 2]) / 255, (upper_float[i + 3] * 255 + lower_float[i + 3]) / 255);

	}
	printf("  diffrence-16bit\n");
	for (int i = 0; i < 4 ; i += 4) {
		unsigned int d1 = (unsigned int)((upper_float[i] * 255 + lower_float[i]) / 255 * 65535) - (unsigned int)(ori_float[i] * 65535);
		unsigned int d2 = (unsigned int)((upper_float[i + 1] * 255 + lower_float[i + 1]) / 255 * 65535) - (unsigned int)(ori_float[i + 1] * 65535);
		unsigned int d3 = (unsigned int)((upper_float[i + 2] * 255 + lower_float[i + 2]) / 255 * 65535) - (unsigned int)(ori_float[i + 2] * 65535);
		unsigned int d4 = (unsigned int)((upper_float[i + 3] * 255 + lower_float[i + 3]) / 255 * 65535) - (unsigned int)(ori_float[i + 3] * 65535);

		float f1 = (float)d1 / (ori_float[i]*65536) * 100;
		float f2 = (float)d2 / (ori_float[i + 1]*65536) * 100;
		float f3 = (float)d3 / (ori_float[i + 2]*65536) * 100;
		float f4 = (float)d4 / (ori_float[i + 3]*65536) * 100;
		printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, f1,f2,f3,f4);
		//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	}

	printf("\n\n");

	//data_upper[i] * 256 + data_lower[i];
	//for (int i = 0; i < width * height * 4; i++) {
	//	float color = (upper[i] * 255 + lower[i]) / 256;
	//	if (ori[i] == color) {

	//	}
	//	else {
	//		printf("error float\n");
	//	}

	//}

	//unsigned int 으로
	unsigned int* ori = new unsigned int[width * height * 4];
	unsigned int* upper = new unsigned int[width * height * 4];
	unsigned int* lower = new unsigned int[width * height * 4];
	for (int i = 0; i < width * height * 4; i++) {
		//ori[i] = (unsigned int)oridata[i] * 65536;
		//upper[i] = (unsigned int)data_upper[i] * 16777216;
		//lower[i] = (unsigned int)data_lower[i] * 16777216;
		ori[i] = (unsigned int)oridata[i] ;
		upper[i] = (unsigned int)data_upper[i] ;
		lower[i] = (unsigned int)data_lower[i] ;

	/*	if (ori[i] > 4294967295 || ori[i] < 0) {
			printf("ori err\n");
		}
		if (upper[i] > 4294967295 || upper[i] < 0) {
			printf("upper err\n");
		}
		if (lower[i] > 4294967295 || lower[i] < 0) {
			printf("lower err\n");
		}*/
	}

	//printf("\n\n unsigend int type\n");
	//printf("  original-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, ori[i], ori[i + 1], ori[i + 2], ori[i + 3]);
	//}
	//printf("  upper-8bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, upper[i], upper[i + 1], upper[i + 2], upper[i + 3]);
	//}
	//printf("  lower-8bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, lower[i], lower[i + 1], lower[i + 2], lower[i + 3]);
	//}
	//printf("  combine-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, (upper[i] * 256 + lower[i]), (upper[i + 1] * 256 + lower[i + 1] ), (upper[i + 2] * 256 + lower[i + 2] ), (upper[i + 3] * 256 + lower[i + 3] ));
	//}

	//printf("\n\n\n\n");

	////upper uncomp, lower comp
	//printf("  original-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, ori[i], ori[i + 1], ori[i + 2], ori[i + 3]);
	//}
	//printf("  upper-8bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, upper[i], upper[i + 1], upper[i + 2], upper[i + 3]);
	//}
	//printf("  lower-8bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, lower_float[i], lower_float[i + 1], lower_float[i + 2], lower_float[i + 3]);
	//}
	//printf("  combine-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	unsigned int d1 = (upper[i] * 256 + unsigned int(lower_float[i] * 255));
	//	unsigned int d2 = (upper[i + 1] * 256 + unsigned int(lower_float[i + 1] * 255));
	//	unsigned int d3 = (upper[i + 2] * 256 + unsigned int(lower_float[i + 2] * 255));
	//	unsigned int d4 = (upper[i + 3] * 256 + unsigned int(lower_float[i + 3] * 255));
	//	printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, d1,d2,d3,d4);
	//}
	//
	//printf("  diffrence-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	float d1 = ori[i]-(upper[i] * 256 + unsigned int(lower_float[i] * 255));
	//	float d2 = ori[i+1] - (upper[i + 1] * 256 + unsigned int(lower_float[i + 1] * 255));
	//	float d3 = ori[i+2] - (upper[i + 2] * 256 + unsigned int(lower_float[i + 2] * 255));
	//	float d4 = ori[i+3] - (upper[i + 3] * 256 + unsigned int(lower_float[i + 3] * 255));
	//	printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, d1 / ori[i] * 100, d2 / ori[i + 1] * 100, d3 / ori[i + 2] * 100, d4 / ori[i + 3] * 100);
	//	//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	//}

		//upper comp, lower uncomp
	printf("  original-16bit\n");
	for (int i = 0; i < 4 * 4; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, ori[i], ori[i + 1], ori[i + 2], ori[i + 3]);
	}
	printf("  upper-8bit\n");
	for (int i = 0; i < 4 * 4; i += 4) {
		printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, upper_float[i], upper_float[i + 1], upper_float[i + 2], upper_float[i + 3]);
	}
	printf("  lower-8bit\n");
	for (int i = 0; i < 4 * 4; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, lower[i], lower[i + 1], lower[i + 2], lower[i + 3]);
	}
	printf("  combine-16bit\n");
	for (int i = 0; i < 4 * 4; i += 4) {
		unsigned int d1 = (unsigned int(upper_float[i] * 255) * 256 + lower[i]);
		unsigned int d2 = (unsigned int(upper_float[i + 1] * 255) * 256 + lower[i + 1]);
		unsigned int d3 = (unsigned int(upper_float[i + 2] * 255) * 256 + lower[i + 2]);
		unsigned int d4 = (unsigned int(upper_float[i + 3] * 255) * 256 + lower[i + 3]);
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, d1,d2,d3,d4);
	}
	printf("  (lower compress) diffrence-16bit\n");
	for (int i = 0; i < 4 * 4; i += 4) {
		float d1 = ori[i] - (unsigned int(upper_float[i] * 255) * 256 + lower[i]);
		float d2 = ori[i + 1] - (unsigned int(upper_float[i + 1] * 255) * 256 + lower[i + 1]);
		float d3 = ori[i + 2] - (unsigned int(upper_float[i + 2] * 255) * 256 + lower[i + 2]);
		float d4 = ori[i + 3] - (unsigned int(upper_float[i + 3] * 255) * 256 + lower[i + 3]);
		printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, d1 / ori[i] * 100, d2 / ori[i + 1] * 100, d3 / ori[i + 2] * 100, d4 / ori[i + 3] * 100);
		//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	}


	//종류별 비교
	//printf("  (upper compress) diffrence-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	float d1 = ori[i] - (upper[i] * 256 + unsigned int(lower_float[i] * 255));
	//	float d2 = ori[i + 1] - (upper[i + 1] * 256 + unsigned int(lower_float[i + 1] * 255));
	//	float d3 = ori[i + 2] - (upper[i + 2] * 256 + unsigned int(lower_float[i + 2] * 255));
	//	float d4 = ori[i + 3] - (upper[i + 3] * 256 + unsigned int(lower_float[i + 3] * 255));
	//	printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, d1 / ori[i] * 100, d2 / ori[i + 1] * 100, d3 / ori[i + 2] * 100, d4 / ori[i + 3] * 100);
	//	//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	//}

	//printf("  (lower compress) diffrence-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	float d1 = ori[i] - (unsigned int(upper_float[i] * 255) * 256 + lower[i]);
	//	float d2 = ori[i + 1] - (unsigned int(upper_float[i+1] * 255) * 256 + lower[i+1]);
	//	float d3 = ori[i + 2] - (unsigned int(upper_float[i+2] * 255) * 256 + lower[i+2]);
	//	float d4 = ori[i + 3] - (unsigned int(upper_float[i+3] * 255) * 256 + lower[i+3]);
	//	printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, d1 / ori[i] * 100, d2 / ori[i + 1] * 100, d3 / ori[i + 2] * 100, d4 / ori[i + 3] * 100);
	//	//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	//}


	//printf("  (all compress) diffrence-16bit\n");
	//for (int i = 0; i < 4 * 4; i += 4) {
	//	float d1 = -ori_float[i] + ((upper_float[i] * 255 + lower_float[i])) / 255;
	//	float d2 = -ori_float[i + 1] + ((upper_float[i + 1] * 255 + lower_float[i + 1]) / 255);
	//	float d3 = -ori_float[i + 2] + ((upper_float[i + 2] * 255 + lower_float[i + 2]) / 255);
	//	float d4 = -ori_float[i + 3] + ((upper_float[i + 3] * 255 + lower_float[i + 3]) / 255);
	//	printf("  [%3d] - %.4f%% %.4f%% %.4f%% %.4f%%\n", i / 4, d1 / ori_float[i] * 100, d2 / ori_float[i + 1] * 100, d3 / ori_float[i + 2] * 100, d4 / ori_float[i + 3] * 100);
	//	//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, d1, d2 , d3 , d4 );
	//}

	printf("  original-16bit\n");
	for (int i = 0; i < 4  * 4; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, (unsigned int)(ori_float[i]*65535), (unsigned int)(ori_float[i + 1] * 65535), (unsigned int)(ori_float[i + 2] * 65535), (unsigned int)(ori_float[i + 3] * 65535));
		//printf("  [%3d] - %.10f %.10f %.10f %.10f\n", i / 4, ori_float[i], ori_float[i + 1], ori_float[i + 2], ori_float[i + 3]);
	}
	printf("  original-16bit\n");
	for (int i = 0; i < 4 * 4; i += 4) {
		printf("  [%3d] - 0x%x 0x%x 0x%x 0x%x\n", i / 4, ori[i], ori[i + 1], ori[i + 2], ori[i + 3]);
	}

	////data_upper[i] * 256 + data_lower[i];
	//for (int i = 0; i < width * height * 4; i++) {
	//	unsigned int color = (upper[i]  + lower[i] / 256) ;
	//	if (ori[i] == color) {

	//	}
	//	else {
	//		printf("error float\n");
	//	}

	//}



	FreeImage_Unload(tx_pixmap_64);
	FreeImage_Unload(tx_pixmap_32[0]);
	FreeImage_Unload(tx_pixmap_32[1]);

}

void combine_2File(const char *filename_comp1, const char *filename_comp2) {
	FILE* fp1, *fp2;

	fp1 = fopen(filename_comp1, "rb");
	fp2 = fopen(filename_comp2, "rb");

	int readsize = 2048 * 2048 * 2;

	BYTE* buf1, *buf2;
	buf1 = new BYTE[readsize];
	buf2 = new BYTE[readsize];
	fread(buf1, 1, readsize, fp1);
	fread(buf2, 1, readsize, fp2);

	fclose(fp1);
	fclose(fp2);

	for (int i = 0; i < readsize; i++) {
		if (buf1[i] != buf2[i]) {
			printf("this is shit\n");
			delete[] buf1;
			delete[] buf2;
			return;
		}
	}

	printf("no diff\n");
	delete[] buf1;
	delete[] buf2;

}

void test_loadimg_diff(const char *filename_ori, const char *filename_comp1) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32[2];

	int width, height;
	BYTE *data_ori, *data_comp1;

	tx_file_format = FreeImage_GetFileType(filename_ori, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename_ori);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32[0] = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32[0] = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32[0]);
	height = FreeImage_GetHeight(tx_pixmap_32[0]);
	data_ori = FreeImage_GetBits(tx_pixmap_32[0]);


	tx_file_format = FreeImage_GetFileType(filename_comp1, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename_comp1);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	//fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32[1] = tx_pixmap;
	else {
		//fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32[1] = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32[1]);
	height = FreeImage_GetHeight(tx_pixmap_32[1]);
	data_comp1 = FreeImage_GetBits(tx_pixmap_32[1]);




	for (int i = 0; i < width * height * 4; i++) {
		if (data_ori[i] != data_comp1[i]) {
			printf("err! not same ori, comp1\n");
			return;
		}
	}

	printf("no diff\n");

	FreeImage_Unload(tx_pixmap_32[0]);
	FreeImage_Unload(tx_pixmap_32[1]);

}


void test_loadimg_diff_bptc(const char *filename_ori, const char *filename_comp1, const char *filename_comp2) {

#define FOURCC_BPTC 0x30315844

	unsigned char header[124];

	FILE *fp;
	/* try to open the file */
	fp = fopen(filename_ori, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filename_ori); getchar();
		return ;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return ;
	}
	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer[3];
	unsigned int bufsize;
	if (fourCC == FOURCC_BPTC) {
		//if BC7 format, header is extend 20. 144 byte header.
		unsigned char exHeader[20];
		fread(&exHeader, 20, 1, fp);
	}

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer[0] = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer[0], 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);


	/* try to open the file */
	fp = fopen(filename_comp1, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filename_comp1); getchar();
		return;
	}

	/* verify the type of file */
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}
	//generate and bind texture
	texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	height = *(unsigned int*)&(header[8]);
	width = *(unsigned int*)&(header[12]);
	linearSize = *(unsigned int*)&(header[16]);
	mipMapCount = *(unsigned int*)&(header[24]);
	fourCC = *(unsigned int*)&(header[80]);

	if (fourCC == FOURCC_BPTC) {
		//if BC7 format, header is extend 20. 144 byte header.
		unsigned char exHeader[20];
		fread(&exHeader, 20, 1, fp);
	}

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer[1] = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer[1], 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);




	/* try to open the file */
	fp = fopen(filename_comp2, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", filename_comp2); getchar();
		return;
	}

	/* verify the type of file */
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return;
	}
	//generate and bind texture
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	height = *(unsigned int*)&(header[8]);
	width = *(unsigned int*)&(header[12]);
	linearSize = *(unsigned int*)&(header[16]);
	mipMapCount = *(unsigned int*)&(header[24]);
	fourCC = *(unsigned int*)&(header[80]);

	if (fourCC == FOURCC_BPTC) {
		//if BC7 format, header is extend 20. 144 byte header.
		unsigned char exHeader[20];
		fread(&exHeader, 20, 1, fp);
	}

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer[2] = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer[2], 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);

	if (memcmp(buffer[0], buffer[1], bufsize) == 0) {
		printf("same ori,1\n");
	}
	else {
		printf("diff ori,1\n");

	}
	if (memcmp(buffer[0], buffer[2], bufsize) == 0) {
		printf("same ori,2\n");
	}
	else {
		printf("diff ori,2\n");

	}
	if (memcmp(buffer[1], buffer[2], bufsize) == 0) {
		printf("same 1,2\n");
	}
	else {
		printf("diff 1,2\n");

	}


	free(buffer[0]);
	free(buffer[1]);
	free(buffer[2]);
}

struct astc_header
{
	uint8_t magic[4];
	uint8_t blockdim_x;
	uint8_t blockdim_y;
	uint8_t blockdim_z;
	uint8_t xsize[3];			// x-size = xsize[0] + xsize[1] + xsize[2]
	uint8_t ysize[3];			// x-size, y-size and z-size are given in texels;
	uint8_t zsize[3];			// block count is inferred
};


int suppress_progress_counter = 0;
int perform_srgb_transform = 0;

#define MAGIC_FILE_CONSTANT 0x5CA1AB13

//ASTC 포맷 파일을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : texname
GLuint load_astc_image(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	if (!f)
	{
		printf("Failed to open file %s\n", filename);
		exit(1);
	}
	astc_header hdr;
	size_t hdr_bytes_read = fread(&hdr, 1, sizeof(astc_header), f);
	if (hdr_bytes_read != sizeof(astc_header))
	{
		fclose(f);
		printf("Failed to read file %s\n", filename);
		exit(1);
	}

	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	uint32_t magicval = hdr.magic[0] + 256 * (uint32_t)(hdr.magic[1]) + 65536 * (uint32_t)(hdr.magic[2]) + 16777216 * (uint32_t)(hdr.magic[3]);

	if (magicval != MAGIC_FILE_CONSTANT)
	{
		fclose(f);
		printf("File %s not recognized\n", filename);
		exit(1);
	}

	int xdim = hdr.blockdim_x;
	int ydim = hdr.blockdim_y;
	int zdim = hdr.blockdim_z;

	if (xdim < 3 || xdim > 12 || ydim < 3 || ydim > 12 || (zdim < 3 && zdim != 1) || zdim > 12)
	{
		fclose(f);
		printf("File %s not recognized %d %d %d\n", filename, xdim, ydim, zdim);
		exit(1);
	}

	int xsize = hdr.xsize[0] + 256 * hdr.xsize[1] + 65536 * hdr.xsize[2];
	int ysize = hdr.ysize[0] + 256 * hdr.ysize[1] + 65536 * hdr.ysize[2];
	int zsize = hdr.zsize[0] + 256 * hdr.zsize[1] + 65536 * hdr.zsize[2];

	int xblocks = (xsize + xdim - 1) / xdim;
	int yblocks = (ysize + ydim - 1) / ydim;
	int zblocks = (zsize + zdim - 1) / zdim;
	int size = xblocks * yblocks * zblocks * 16;
	uint8_t *buffer = (uint8_t *)malloc(size);
	if (!buffer)
	{
		fclose(f);
		printf("Ran out of memory\n");
		exit(1);
	}
	size_t bytes_to_read = xblocks * yblocks * zblocks * 16;
	size_t bytes_read = fread(buffer, 1, bytes_to_read, f);
	fclose(f);
	if (bytes_read != bytes_to_read)
	{
		printf("Failed to read file %s\n", filename);
		exit(1);
	}

	if (xdim == 12 && ydim == 12) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_12x12_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 10 && ydim == 10) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_10x10_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 8 && ydim == 8) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_8x8_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 6 && ydim == 6) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_6x6_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 5 && ydim == 5) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_5x5_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 4 && ydim == 4) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_4x4_KHR, xsize, ysize, 0, size, buffer);
	}

	free(buffer);

	return texname;
}

//ASTC 포맷 파일을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : 업로드 시간
float load_astc_image_checktime(const char *filename)
{
	FILE *f = fopen(filename, "rb");
	if (!f)
	{
		printf("Failed to open file %s\n", filename);
		exit(1);
	}
	astc_header hdr;
	size_t hdr_bytes_read = fread(&hdr, 1, sizeof(astc_header), f);
	if (hdr_bytes_read != sizeof(astc_header))
	{
		fclose(f);
		printf("Failed to read file %s\n", filename);
		exit(1);
	}

	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	uint32_t magicval = hdr.magic[0] + 256 * (uint32_t)(hdr.magic[1]) + 65536 * (uint32_t)(hdr.magic[2]) + 16777216 * (uint32_t)(hdr.magic[3]);

	if (magicval != MAGIC_FILE_CONSTANT)
	{
		fclose(f);
		printf("File %s not recognized\n", filename);
		exit(1);
	}

	int xdim = hdr.blockdim_x;
	int ydim = hdr.blockdim_y;
	int zdim = hdr.blockdim_z;

	if (xdim < 3 || xdim > 12 || ydim < 3 || ydim > 12 || (zdim < 3 && zdim != 1) || zdim > 12)
	{
		fclose(f);
		printf("File %s not recognized %d %d %d\n", filename, xdim, ydim, zdim);
		exit(1);
	}

	int xsize = hdr.xsize[0] + 256 * hdr.xsize[1] + 65536 * hdr.xsize[2];
	int ysize = hdr.ysize[0] + 256 * hdr.ysize[1] + 65536 * hdr.ysize[2];
	int zsize = hdr.zsize[0] + 256 * hdr.zsize[1] + 65536 * hdr.zsize[2];

	int xblocks = (xsize + xdim - 1) / xdim;
	int yblocks = (ysize + ydim - 1) / ydim;
	int zblocks = (zsize + zdim - 1) / zdim;
	int size = xblocks * yblocks * zblocks * 16;
	uint8_t *buffer = (uint8_t *)malloc(sizeof(uint8_t)*size);
	if (!buffer)
	{
		fclose(f);
		printf("Ran out of memory\n");
		exit(1);
	}
	//size_t bytes_to_read = xblocks * yblocks * zblocks * 16;
	size_t bytes_read = fread(buffer, 1, size, f);
	fclose(f);
	if (bytes_read != size)
	{
		printf("Failed to read file %s\n", filename);
		exit(1);
	}
	float compute_time;
	glFinish();
	CHECK_TIME_START;
	if (xdim == 12 && ydim == 12) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_12x12_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 10 && ydim == 10) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_10x10_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 8 && ydim == 8) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_8x8_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 6 && ydim == 6) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_6x6_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 5 && ydim == 5) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_5x5_KHR, xsize, ysize, 0, size, buffer);
	}
	else if (xdim == 4 && ydim == 4) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_4x4_KHR, xsize, ysize, 0, size, buffer);
	}
	glFinish();
	CHECK_TIME_END(compute_time);

	free(buffer);
		
	//GLint bytes;
	//glGetTextureLevelParameteriv(texname, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bytes);
	//printf("memory size : %d byte, %dMB\n", bytes, bytes / 1024 / 1024);

	return compute_time;
}


//.DDS 포맷 파일(DXT1~5, BPTC)을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : texname
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
#define FOURCC_BPTC 0x30315844
#define FOURCC_BC4 0x53344342
GLuint load_dds_image(const char * imagepath) {

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}
	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	if (fourCC == FOURCC_BPTC) {
		//if BC7 format, header is extend 20. 144 byte header.
		unsigned char exHeader[20];
		fread(&exHeader, 20, 1, fp);
	}

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	unsigned int blockSize;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		blockSize = 8;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		blockSize = 16;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		blockSize = 16;
		break;
	case FOURCC_BC4:
		format = GL_COMPRESSED_RED_RGTC1;
		blockSize = 8;
		break;
	case FOURCC_BPTC:
		format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		blockSize = 16;
		break;
	default:
		free(buffer);
		return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return texname;
}

//.DDS 포맷 파일(DXT1~5, BPTC)을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : 업로드 시간
float load_dds_image_checktime(const char * imagepath) {

	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}
	//generate and bind texture
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);


	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);

	unsigned char * buffer;
	unsigned int bufsize;
	if (fourCC == FOURCC_BPTC) {
		//if BC7 format, header is extend 20. 144 byte header.
		unsigned char exHeader[20];
		fread(&exHeader, 20, 1, fp);
	}

	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);

	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	unsigned int blockSize;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		blockSize = 8;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		blockSize = 16;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		blockSize = 16;
		break;
	case FOURCC_BC4:
		format = GL_COMPRESSED_RED_RGTC1;
		blockSize = 8;
		break;
	case FOURCC_BPTC:
		format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		blockSize = 16;
		break;
	default:
		free(buffer);
		return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int offset = 0;

	float compute_time;
	glFinish();
	CHECK_TIME_START;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}
	glFinish();
	CHECK_TIME_END(compute_time);

	free(buffer);


/*

	GLint bytes;
	glGetTextureLevelParameteriv(texname, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &bytes);
	printf("memory size : %d byte, %dMB\n", bytes, bytes/1024/1024);*/


	return compute_time;

}


//두 이미지의 유사성 비교, 높을수록 좋은 것.
//리턴 : PSNR 점수
double getPSNR(const char* fileName1, const char* fileName2)
{
	FreeImage_Initialise(TRUE);
	FIBITMAP *dib1 = FreeImage_Load(FreeImage_GetFIFFromFilename(fileName1), fileName1, PNG_DEFAULT);
	FIBITMAP *dib2 = FreeImage_Load(FreeImage_GetFIFFromFilename(fileName2), fileName2, PNG_DEFAULT);

	unsigned int height = FreeImage_GetHeight(dib1);
	unsigned int width = FreeImage_GetWidth(dib1);
	double ssdR = 0, ssdG = 0, ssdB = 0, psnr = 0;
	RGBQUAD color1, color2;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			FreeImage_GetPixelColor(dib1, x, y, &color1);
			FreeImage_GetPixelColor(dib2, x, y, &color2);

			ssdR += (pow(((double)color1.rgbRed - (double)color2.rgbRed), 2));
			ssdG += (pow(((double)color1.rgbGreen - (double)color2.rgbGreen), 2));
			ssdB += (pow(((double)color1.rgbBlue - (double)color2.rgbBlue), 2));
		}
	}

	double mse = ((ssdR + ssdG + ssdB) / 3) / (height * width);
	psnr = 10 * log10(255 * 255 / mse);

	FreeImage_Unload(dib1);
	FreeImage_Unload(dib2);

	return psnr;
}
