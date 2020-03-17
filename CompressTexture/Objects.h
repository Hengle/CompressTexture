// axes object
GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },
{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } };

void prepare_axes(void) { // draw coordinate axes
						  // initialize vertex buffer object
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// initialize vertex array object
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_axes(void) {
	// assume ShaderProgram_simple is used
	glBindVertexArray(axes_VAO);
	glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
}

// floor object
GLuint rectangle_VBO, rectangle_VAO;
GLfloat rectangle_vertices[6][8] = {  // vertices enumerated counterclockwise
	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
{ 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
{ 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }
};

//Material_Parameters material_floor;

void prepare_quad(void) { // Draw coordinate axes.
						   // Initialize vertex buffer object.
	glGenBuffers(1, &rectangle_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &rectangle_VAO);
	glBindVertexArray(rectangle_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rectangle_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

float prepare_texture_original(const char* filename, GLuint textureID) {
	float time;
	glActiveTexture(GL_TEXTURE0 + textureID);
	glBindTexture(GL_TEXTURE_2D, texture_names[textureID]);

	time = My_glTexImage2D_from_file(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return time;
}

float create_ACTC_texture(const char* filename, GLuint textureID) {

	float time;
	glActiveTexture(GL_TEXTURE0 + textureID);
	glBindTexture(GL_TEXTURE_2D, texture_names[textureID]);

	time = load_astc_file(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return time;
}

void prepare_texture_ASTC() {
	//create_ACTC_texture("Data/HDR/4kimg_4x4.astc", TEXTURE_INDEX_COMPRESS_ASTC4X4);
	//create_ACTC_texture("Data/HDR/4kimg_5x5.astc", TEXTURE_INDEX_COMPRESS_ASTC5X5);
	//create_ACTC_texture("Data/HDR/4kimg_6x6.astc", TEXTURE_INDEX_COMPRESS_ASTC6X6);
	//create_ACTC_texture("Data/HDR/4kimg_8x8.astc", TEXTURE_INDEX_COMPRESS_ASTC8X8);
	//create_ACTC_texture("Data/HDR/4kimg_10x10.astc", TEXTURE_INDEX_COMPRESS_ASTC10X10);
	//create_ACTC_texture("Data/HDR/4kimg_12x12.astc", TEXTURE_INDEX_COMPRESS_ASTC12X12);
	//create_ACTC_texture("Data/LDR/4kimg_12x12.astc", TEXTURE_INDEX_COMPRESS_ASTC12X12_LDR_SRGB);
	//create_ACTC_texture("Data/LINEAR/4kimg_12x12.astc", TEXTURE_INDEX_COMPRESS_ASTC12X12_LDR_LINEAR);

	//HDR 과 LDR(Linear colorspace)는 동일함. LDR(sRGB colorspace)는 약간 다름. HDR은 기본적으로 linear colorspace로 추정.

}

void upload_TEST_Texture_Original() {
	char name[100];
	float time = 0.0f;

	for (int i = 1; i <= 20; i++) {
		sprintf(name, "Data/testImg/jpg20/img%d.jpg", i);
		time += prepare_texture_original(name, TEXTURE_INDEX_ORIGINAL_TEST1 - 1 + i);
	}

	printf("upload time for original : %f\n", time);
}

void upload_TEST_Texture_ASTC(int compressLevel) {
	char name[100];
	float time = 0.0f;

	for (int i = 1; i <= 20; i++) {
		sprintf(name, "Data/testImg/%dx%d/img%d.astc", compressLevel, compressLevel,i);
		time += create_ACTC_texture(name, TEXTURE_INDEX_COMPRESS_TEST1-1+i);
	}

	printf("upload time for ASTC %dx%d compress : %f\n", compressLevel, compressLevel,time);
	
}



float create_DDS_Texture(const char* filename, GLuint textureID) {

	float time;
	glActiveTexture(GL_TEXTURE0 + textureID);
	glBindTexture(GL_TEXTURE_2D, texture_names[textureID]);

	time = loadDDS(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return time;
}


void upload_TEST_Texture_DDS(int compressLevel) {//DXT 1, 3, 5
	char name[100];
	float time = 0.0f;

	for (int i = 1; i <= 20; i++) {
		sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_DXT%d.DDS", i,compressLevel);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
	}

	printf("upload time for DXT%d compress : %f\n", compressLevel, time);

}


void upload_TEST_Texture_BPTC() {
	char name[100];
	float time = 0.0f;

	for (int i = 1; i <= 20; i++) {
		//sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_DXT%d.DDS", i, 5);
		//time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);

		sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_BC7.DDS", i);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
	}

	printf("upload time for BPTC compress : %f\n", time);

}

void compare_PSNR() {
	char name_ori[20][100];
	char name[100];

	double score;
	float time = 0.0f;

	for (int i = 1; i <= 20; i++) {
		sprintf(name_ori[i], "Data/testImg/jpg20/img%d.jpg", i);
	}

	for (int i = 1; i <= 20; i++) {
		printf("image %2d, PSNR Score => ", i);
		//DXT
		for (int compressLevel = 1; compressLevel <= 5; compressLevel += 2) {
			sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_DXT%d.DDS", i, compressLevel);
			score = getPSNR(name_ori[i], name);
			printf("DXT %d - %lf ", compressLevel, score);
		}

		//BC7
		sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_BC7.DDS", i);
		score = getPSNR(name_ori[i], name);
		printf("BPTC - %lf ", score);

		printf("\n");
	}

	////ASTC
	//int size[] = {4,5,6,8,10,12};
	//for (int j = 0; j < 6; j ++) {
	//	int compressLevel = size[j];
	//	for (int i = 1; i <= 20; i++) {
	//		sprintf(name, "Data/testImg/%dx%d/img%d.astc", compressLevel, compressLevel, i);
	//		score = getPSNR(name_ori[i], name);
	//		printf("image %d, ASTC(hdr) %dx%d PSNR Score = %lf\n", i, compressLevel, compressLevel ,score);
	//	}
	//}

}


void compress_enum() {
	//지원하는 포맷 리스트.
	GLint * compressed_format;
	GLint num_compressed_format;
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &num_compressed_format);
	compressed_format = (GLint*)malloc(num_compressed_format * sizeof(GLint));
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, compressed_format);
}

void SaveTexture(const char* filename, int width, int height, GLint compressed_size, unsigned char * img, int format ,int depth) {

	FILE* fp = fopen(filename,"wb");

	fwrite(&width, sizeof(int), 1, fp);
	fwrite(&height, sizeof(int), 1, fp);
	fwrite(&compressed_size, sizeof(int), 1, fp);
	fwrite(&format, sizeof(int), 1, fp);

	fwrite(img,sizeof(unsigned char *), compressed_size,fp);

	fclose(fp);

}


void LoadTexture(const char* filename) {
	int width, height;
	GLint compressed_size;
	unsigned char * img;
	int format;

	FILE* fp = fopen(filename, "rb");

	fread(&width, sizeof(int), 1, fp);
	fread(&height, sizeof(int), 1, fp);
	fread(&compressed_size, sizeof(int), 1, fp);
	fread(&format, sizeof(int), 1, fp);


	img = (unsigned char *)malloc(compressed_size * sizeof(unsigned char));

	fread(img, sizeof(unsigned char), compressed_size, fp);
	
	fclose(fp);

	glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_TEST);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_INDEX_TEST]);
	glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height,	0, compressed_size, img);

}


float compress_test(const char *filename, const char* outfilename) {
	//glGetIntegerv()
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLvoid *data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	float compute_time;

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	GLint compressed_decal_map;
	GLint compressed;
	GLint internalformat;
	GLint compressed_size;
	unsigned char * img;
	//glActiveTexture(GL_TEXTURE0 + TEXTURE_INDEX_TEST);
	glBindTexture(GL_TEXTURE_2D, texture_names[TEXTURE_INDEX_TEST]);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, &compressed);
	/* if the compression has been successful */
	if (compressed == GL_TRUE)
	{
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_size);
		img = (unsigned char *)malloc(compressed_size * sizeof(unsigned char));
		glGetCompressedTexImageARB(GL_TEXTURE_2D, 0, img);
		SaveTexture(outfilename,width, height, compressed_size, img, internalformat, 0);
		free(img);
	}
	else {

		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_size);
		img = (unsigned char *)malloc(compressed_size * sizeof(unsigned char));
		glGetCompressedTexImage(GL_TEXTURE_2D, 0, img);
		SaveTexture(outfilename,width, height, compressed_size, img, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 0);
	}


	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);


	return 0.0f;
}

void draw_quad(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// tiger object
#define N_TIGER_FRAMES 12
GLuint tiger_VBO, tiger_VAO;
int tiger_n_triangles[N_TIGER_FRAMES];
int tiger_vertex_offset[N_TIGER_FRAMES];
GLfloat *tiger_vertices[N_TIGER_FRAMES];

//Material_Parameters material_tiger;

int read_geometry(GLfloat **object, int bytes_per_primitive, const char *filename) {
	int n_triangles;
	FILE *fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);
	*object = (float *)malloc(n_triangles*bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

