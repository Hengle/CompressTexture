
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

	for (int i = 1; i <= 20; i++) {
		sprintf(name, "Data/original/img%d.jpg", i);
		time += create_ORIGINAL_texture(name, TEXTURE_INDEX_ORIGINAL_TEST1 - 1 + i);
	}

	printf("upload time for original : %f\n", time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for original : %f\n", time);
	fclose(fp);
}

void upload_TEST_Texture_ASTC(int compressLevel) {
	char name[100];
	float time = 0.0f;

	for (int i = 1; i <= 20; i++) {
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

	for (int i = 1; i <= 20; i++) {
		sprintf(name, "Data/dxt/img%d_JPG_DXT%d.DDS", i, compressLevel);
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

	for (int i = 1; i <= 20; i++) {
		sprintf(name, "Data/bc7/img%d.DDS", i);
		time += create_DDS_Texture(name, TEXTURE_INDEX_COMPRESS_TEST1 - 1 + i);
	}
	printf("upload time for BPTC compress : %f\n", time);

	FILE* fp = fopen("log.txt", "a");
	fprintf(fp, "upload time for BPTC compress : %f\n", time);
	fclose(fp);

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
		//sprintf(name, "Data/testImg/jpg20/results/img%d_JPG_BC7.DDS", i);
		//score = getPSNR(name_ori[i], name);
		//printf("BPTC - %lf ", score);

		//printf("\n");
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


void draw_quad(void) {
	glFrontFace(GL_CCW);

	glBindVertexArray(rectangle_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
