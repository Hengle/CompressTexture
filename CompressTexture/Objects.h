
#define TEST_IMAGE_COUNT 24
#define N_NORMAL_TEXTURES_USED 50

#define TEXTURE_INDEX_ORIGINAL 0
#define TEXTURE_INDEX_TEST 1

#define TEXTURE_INDEX_COMPRESS_TEST1 2
#define TEXTURE_INDEX_COMPRESS_TEST2 3
#define TEXTURE_INDEX_COMPRESS_TEST3 4
#define TEXTURE_INDEX_COMPRESS_TEST4 5
#define TEXTURE_INDEX_COMPRESS_TEST5 6
#define TEXTURE_INDEX_COMPRESS_TEST6 7
#define TEXTURE_INDEX_COMPRESS_TEST7 8
#define TEXTURE_INDEX_COMPRESS_TEST8 9
#define TEXTURE_INDEX_COMPRESS_TEST9 10
#define TEXTURE_INDEX_COMPRESS_TEST10 11
#define TEXTURE_INDEX_COMPRESS_TEST11 12
#define TEXTURE_INDEX_COMPRESS_TEST12 13
#define TEXTURE_INDEX_COMPRESS_TEST13 14
#define TEXTURE_INDEX_COMPRESS_TEST14 15
#define TEXTURE_INDEX_COMPRESS_TEST15 16
#define TEXTURE_INDEX_COMPRESS_TEST16 17
#define TEXTURE_INDEX_COMPRESS_TEST17 18
#define TEXTURE_INDEX_COMPRESS_TEST18 19
#define TEXTURE_INDEX_COMPRESS_TEST19 20
#define TEXTURE_INDEX_COMPRESS_TEST20 21
#define TEXTURE_INDEX_COMPRESS_TEST21 22
#define TEXTURE_INDEX_COMPRESS_TEST22 23
#define TEXTURE_INDEX_COMPRESS_TEST23 24
#define TEXTURE_INDEX_COMPRESS_TEST24 25

#define TEXTURE_INDEX_ORIGINAL_TEST1 26
#define TEXTURE_INDEX_ORIGINAL_TEST2 27
#define TEXTURE_INDEX_ORIGINAL_TEST3 28
#define TEXTURE_INDEX_ORIGINAL_TEST4 29
#define TEXTURE_INDEX_ORIGINAL_TEST5 30
#define TEXTURE_INDEX_ORIGINAL_TEST6 31
#define TEXTURE_INDEX_ORIGINAL_TEST7 32
#define TEXTURE_INDEX_ORIGINAL_TEST8 33
#define TEXTURE_INDEX_ORIGINAL_TEST9 34
#define TEXTURE_INDEX_ORIGINAL_TEST10 35
#define TEXTURE_INDEX_ORIGINAL_TEST11 36
#define TEXTURE_INDEX_ORIGINAL_TEST12 37
#define TEXTURE_INDEX_ORIGINAL_TEST13 38
#define TEXTURE_INDEX_ORIGINAL_TEST14 39
#define TEXTURE_INDEX_ORIGINAL_TEST15 40
#define TEXTURE_INDEX_ORIGINAL_TEST16 41
#define TEXTURE_INDEX_ORIGINAL_TEST17 42
#define TEXTURE_INDEX_ORIGINAL_TEST18 43
#define TEXTURE_INDEX_ORIGINAL_TEST19 44
#define TEXTURE_INDEX_ORIGINAL_TEST20 45
#define TEXTURE_INDEX_ORIGINAL_TEST21 46
#define TEXTURE_INDEX_ORIGINAL_TEST22 47
#define TEXTURE_INDEX_ORIGINAL_TEST23 48
#define TEXTURE_INDEX_ORIGINAL_TEST24 49

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

float create_ORIGINAL_texture(const char* filename, GLuint textureID) {
	float time;
	glActiveTexture(GL_TEXTURE0 + textureID);
	//glBindTexture(GL_TEXTURE_2D, texture_names[textureID]);

	time = load_unpack_image_checktime(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return time;
}

float create_ASTC_texture(const char* filename, GLuint textureID) {

	float time;
	glActiveTexture(GL_TEXTURE0 + textureID);
	//glBindTexture(GL_TEXTURE_2D, texture_names[textureID]);

	time = load_astc_image_checktime(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return time;
}

float create_DDS_Texture(const char* filename, GLuint textureID) {

	float time;
	glActiveTexture(GL_TEXTURE0 + textureID);
	//glBindTexture(GL_TEXTURE_2D, texture_names[textureID]);

	time = load_dds_image_checktime(filename);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return time;
}

void upload_TEST_Texture_Original() {
	char name[100];
	float time = 0.0f;

	type_of_original_image = 0;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
		//sprintf(name, "Data/original/img%d.png", i);
		sprintf(name, "Data/yuv/original_rgb/image_yuv_%d.bmp", i-1);
		time += create_ORIGINAL_texture(name, TEXTURE_INDEX_ORIGINAL_TEST1 - 1 + i);
	}

	printf("upload time for original rgb : %f\n", time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for original rgb : %f\n", time);
	fclose(fp);
}

void upload_TEST_Texture_Original_YUV() {

	char name[100];
	float time = 0.0f;
	type_of_original_image = 1;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i += 3) {

		sprintf(name, "Data/yuv/original_yuv/image_y_%d.bmp", i / 3);
		time += create_ORIGINAL_texture(name, TEXTURE_INDEX_ORIGINAL_TEST1 - 1 + i);
		sprintf(name, "Data/yuv/original_yuv/image_u_%d.bmp", i / 3);
		time += create_ORIGINAL_texture(name, TEXTURE_INDEX_ORIGINAL_TEST1 + 0 + i);
		sprintf(name, "Data/yuv/original_yuv/image_v_%d.bmp", i / 3);
		time += create_ORIGINAL_texture(name, TEXTURE_INDEX_ORIGINAL_TEST1 + 1 + i);

	}
	printf("upload time for original yuv : %f\n", time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for original yuv : %f\n", time);
	fclose(fp);

}

void upload_TEST_Texture_ASTC(int compressLevel) {
	char name[100];
	float time = 0.0f;
	type_of_compressed_image = 0;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
		sprintf(name, "Data/astc/%dx%d/img%d.astc", compressLevel, compressLevel, i);
		time += create_ASTC_texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
	}

	printf("upload time for ASTC %dx%d compress : %f\n", compressLevel, compressLevel, time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for ASTC %dx%d compress : %f\n", compressLevel, compressLevel, time);
	fclose(fp);
}

void upload_TEST_Texture_DXT(int compressLevel) {//DXT 1, 3, 5
	char name[100];
	float time = 0.0f;
	type_of_compressed_image = 0;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
		//sprintf(name, "Data/dxt/img%d_JPG_DXT%d.DDS", i, compressLevel);
		sprintf(name, "Data/yuv/dxt%d/image_yuv_%d.DDS", compressLevel, i-1);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
	}

	printf("upload time for DXT%d compress : %f\n", compressLevel, time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for DXT%d compress : %f\n", compressLevel, time);
	fclose(fp);

}

void upload_TEST_Texture_BPTC() {
	char name[100];
	float time = 0.0f;
	type_of_compressed_image = 0;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
		//sprintf(name, "Data/bc7/img%d.DDS", i);
		sprintf(name, "Data/yuv/bc7/image_yuv_%d.DDS", i-1);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
	}
	printf("upload time for BPTC compress : %f\n", time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for BPTC compress : %f\n", time);
	fclose(fp);

}

void upload_TEST_Texture_YUV() {

	char name[100];
	float time = 0.0f;
	type_of_compressed_image = 1;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i+=3) {

		sprintf(name, "Data/yuv/yuv_dxt1/image_y_%d.DDS", i / 3);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
		sprintf(name, "Data/yuv/yuv_dxt1/image_u_%d.DDS", i / 3);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 + 0 + i);
		sprintf(name, "Data/yuv/yuv_dxt1/image_v_%d.DDS", i / 3);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 + 1 + i);

	}
	printf("upload time for YUV+DXT1 compress : %f\n", time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for YUV+DXT1 compress : %f\n", time);
	fclose(fp);

}

void compare_PSNR() {
	char name_ori[TEST_IMAGE_COUNT][100];
	char name[100];

	double score;
	float time = 0.0f;

	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
		sprintf(name_ori[i], "Data/testImg/jpg20/img%d.jpg", i);
	}

	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
		printf("image %2d, PSNR Score => ", i);
		//DXT
		for (int compressLevel = 1; compressLevel <= 5; compressLevel += 2) {
			sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_DXT%d.DDS", i, compressLevel);
			score = getPSNR(name_ori[i], name);
			printf("DXT %d - %lf ", compressLevel, score);
		}

		//BC7
		//sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_BC7.DDS", i);
		//score = getPSNR(name_ori[i], name);
		//printf("BPTC - %lf ", score);

		//printf("\n");
	}

	////ASTC
	//int size[] = {4,5,6,8,10,12};
	//for (int j = 0; j < 6; j ++) {
	//	int compressLevel = size[j];
	//	for (int i = 1; i <= TEST_IMAGE_COUNT; i++) {
	//		sprintf(name, "Data/testImg/%dx%d/img%d.astc", compressLevel, compressLevel, i);
	//		score = getPSNR(name_ori[i], name);
	//		printf("image %d, ASTC(hdr) %dx%d PSNR Score = %lf\n", i, compressLevel, compressLevel ,score);
	//	}
	//}

}


void draw_quad(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
