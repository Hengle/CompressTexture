#include "ImageLoader.h"


__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))

//압축되지 않은 일반 이미지(jpg, png 등)을 로드하여 텍스처하는 함수
//리턴 : texname
GLuint load_unpack_image(const char *filename) {
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);

	return texname;
}

//압축되지 않은 일반 이미지(jpg, png 등)을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : 업로드 시간
float load_unpack_image_checktime(const char *filename) {
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	glFinish();
	CHECK_TIME_END(compute_time);
	//fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);

	return compute_time;
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
	return compute_time;
}


//.DDS 포맷 파일(DXT1~5, BPTC)을 로드하여 텍스처하는 함수 - 시간 측정 버전
//리턴 : texname
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
#define FOURCC_BPTC 0x30315844
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
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	case FOURCC_BPTC:
		format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		break;
	default:
		free(buffer);
		return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
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
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	case FOURCC_BPTC:
		format = GL_COMPRESSED_RGBA_BPTC_UNORM;
		break;
		GL_COMPRESSED_RED_RGTC1;
	default:
		free(buffer);
		return 0;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
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


void saveGrayImage() {


	FreeImage_sa

}