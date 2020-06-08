//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"
#include <time.h>


GLuint h_ShaderProgram_TXPS;

// for simple shaders
GLint loc_ModelViewProjectionMatrix_simple, loc_primitive_color;

GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;
GLint loc_flag_texture_mapping, loc_flag_texture_diffrence, loc_flag_texture_reverse, loc_flag_depth_or_color;
GLint loc_drawtype, loc_typeof_original_image, loc_typeof_compressed_image;
GLint loc_base_texture, loc_base_texture_y, loc_base_texture_u, loc_base_texture_v, loc_base_texture_index;
GLint loc_original_texture, loc_original_texture_y, loc_original_texture_u, loc_original_texture_v, loc_original_texture_index;


GLint loc_depth_num, loc_depth_version, loc_depth_uncomp_16bit, loc_depth_uncomp_upper, loc_depth_uncomp_lower, loc_depth_comp_upper, loc_depth_comp_lower, loc_depth_comp_split1, loc_depth_comp_split2;
GLint loc_depth_type, loc_depth_lower_gray, loc_depth_lower_4frame, loc_depth_lower_a, loc_depth_lower_rgba, loc_depth_lower_4cam;

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;
glm::mat4 ViewMatrix, ProjectionMatrix;

//이미지가 rgb인지, yuv인지 구분하기 위해서. 0 -rgb, 1- yuv
int type_of_original_image = 0;
int type_of_compressed_image = 0;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2


struct _flag {
	int texture_mapping;
	int texture_diffrence;
	int depth_ot_color;
} flag;


struct _WINDOW_param {
	int width, height;
} WINDOW_param;


#include "ImageLoader.h"
#include "ImageWriter.h"
#include "Objects.h"

char titleTex[N_NORMAL_TEXTURES_USED][30] = {
	"ORIGINAL",
	  "TEST",
	  "TEST_COMPRESSED_1",
	  "TEST_COMPRESSED_2",
	  "TEST_COMPRESSED_3",
	  "TEST_COMPRESSED_4",
	  "TEST_COMPRESSED_5",
	  "TEST_COMPRESSED_6",
	  "TEST_COMPRESSED_7",
	  "TEST_COMPRESSED_8",
	  "TEST_COMPRESSED_9",
	  "TEST_COMPRESSED_10",
	  "TEST_COMPRESSED_11",
	  "TEST_COMPRESSED_12",
	  "TEST_COMPRESSED_13",
	  "TEST_COMPRESSED_14",
	  "TEST_COMPRESSED_15",
	  "TEST_COMPRESSED_16",
	  "TEST_COMPRESSED_17",
	  "TEST_COMPRESSED_18",
	  "TEST_COMPRESSED_19",
	  "TEST_COMPRESSED_20",
	  "TEST_COMPRESSED_21",
	  "TEST_COMPRESSED_22",
	  "TEST_COMPRESSED_23",
	  "TEST_COMPRESSED_24",
	  "TEST_Original_1",
	  "TEST_Original_2",
	  "TEST_Original_3",
	  "TEST_Original_4",
	  "TEST_Original_5",
	  "TEST_Original_6",
	  "TEST_Original_7",
	  "TEST_Original_8",
	  "TEST_Original_9",
	  "TEST_Original_10",
	  "TEST_Original_11",
	  "TEST_Original_12",
	  "TEST_Original_13",
	  "TEST_Original_14",
	  "TEST_Original_15",
	  "TEST_Original_16",
	  "TEST_Original_17",
	  "TEST_Original_18",
	  "TEST_Original_19",
	  "TEST_Original_20",
	  "TEST_Original_21",
	  "TEST_Original_22",
	  "TEST_Original_23",
	  "TEST_Original_24",
};


#define IMGSIZE 600.0f
GLuint texnum_ori = TEXTURE_INDEX_ORIGINAL_TEST1;		//original (unpack) texture name
GLuint texnum_comp = TEXTURE_INDEX_COMPRESS_TEST1;		//compressed texture name
GLuint depth_version = 0;
GLuint depth_type = 0;
GLuint texnum_depth = 0;

int draw_mode = 0;

void setDepthTex() {

	glUseProgram(h_ShaderProgram_TXPS);

	int texchannal = texnum_depth / 4;

	glUniform1i(loc_depth_uncomp_16bit, TEXTURE_INDEX_DEPTH_UNCOMP_16BIT_TEST1 + texchannal);
	glUniform1i(loc_depth_uncomp_upper, TEXTURE_INDEX_DEPTH_UNCOMP_UPPER_TEST1 + texchannal);
	glUniform1i(loc_depth_uncomp_lower, TEXTURE_INDEX_DEPTH_UNCOMP_LOWER_TEST1 + texchannal);
	glUniform1i(loc_depth_comp_upper, TEXTURE_INDEX_DEPTH_COMP_UPPER_TEST1 + texchannal);
	glUniform1i(loc_depth_comp_lower, TEXTURE_INDEX_DEPTH_COMP_LOWER_TEST1 + texchannal);


	glUniform1i(loc_depth_lower_gray, TEXTURE_INDEX_DEPTH_LOWERCOMP_GRAY_TEST1 + texnum_depth);
	glUniform1i(loc_depth_lower_4frame, TEXTURE_INDEX_DEPTH_LOWERCOMP_4FRAME_TEST1 + texnum_depth);
	glUniform1i(loc_depth_lower_a, TEXTURE_INDEX_DEPTH_LOWERCOMP_A_TEST1 + texnum_depth);
	glUniform1i(loc_depth_lower_rgba, TEXTURE_INDEX_DEPTH_LOWERCOMP_RGBA_TEST1 + texnum_depth);
	glUniform1i(loc_depth_lower_4cam, TEXTURE_INDEX_DEPTH_LOWERCOMP_4CAM_TEST1 + texchannal);

	//printf("image num : %d %d %d %d %d\n", TEXTURE_INDEX_DEPTH_UNCOMP_16BIT_TEST1 + texchannal, TEXTURE_INDEX_DEPTH_UNCOMP_UPPER_TEST1 + texchannal, TEXTURE_INDEX_DEPTH_UNCOMP_LOWER_TEST1 + texchannal, TEXTURE_INDEX_DEPTH_COMP_UPPER_TEST1 + texchannal, TEXTURE_INDEX_DEPTH_COMP_LOWER_TEST1 + texchannal);


	//glUniform1i(loc_depth_comp_split1, TEXTURE_INDEX_DEPTH_COMP_SPLIT1_TEST1 + texnum_depth);
	//glUniform1i(loc_depth_comp_split2, TEXTURE_INDEX_DEPTH_COMP_SPLIT2_TEST1 + texnum_depth);

	glUseProgram(0);
}

void draw_original_texture() {
	glm::mat4 ModelMatrix;

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_drawtype, 0);

	glUniform1i(loc_typeof_original_image, type_of_original_image);
	glUniform1i(loc_typeof_compressed_image, 0);

	glUniform1i(loc_flag_texture_reverse, true);
	//glUniform1i(loc_base_texture, texnum_ori);
	//glUniform1i(loc_original_texture, texnum_ori);

	if (type_of_original_image == 0) {//RGB image
		glUniform1i(loc_original_texture, texnum_ori);			//랜더링할 텍스쳐
		//printf("ori : %d texnum : %d\n", texnum_ori, texnum_comp);
	}
	else {//yuv image
		glUniform1i(loc_flag_texture_reverse, true);//DDS 포맷은 상하가 반전되어 있다
		int texindex = texnum_ori - TEXTURE_INDEX_ORIGINAL_TEST1;
		int y = texindex - texindex % 3 + TEXTURE_INDEX_ORIGINAL_TEST1, u = y + 1, v = y + 2;
		glUniform1i(loc_original_texture_y, y);			//랜더링할 텍스쳐 y
		glUniform1i(loc_original_texture_u, u);			//랜더링할 텍스쳐 u
		glUniform1i(loc_original_texture_v, v);			//랜더링할 텍스쳐 v
		glUniform1i(loc_original_texture_index, texindex % 3 + 1);			//랜더링할 텍스처 안에서의 이미지 번호. 한 텍스처에 3개의 이미지 삽입
		//printf("texcomp : %d. ", texnum_comp);
		//printf("texnum : %d, y=%d u=%d v=%d\n", texindex % 3 + 1, y+24,u+24,v+24);
	}

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -IMGSIZE / 2, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(IMGSIZE, IMGSIZE, IMGSIZE));
	ModelViewMatrix = ViewMatrix * ModelMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_quad();

	glUseProgram(0);
}


void draw_compressed_texture() {
	glm::mat4 ModelMatrix;

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_drawtype, 1);

	glUniform1i(loc_typeof_original_image, type_of_original_image);
	glUniform1i(loc_typeof_compressed_image, type_of_compressed_image);
	//printf("type of com tex : %d\n", type_of_compressed_image);

	glUniform1i(loc_flag_texture_reverse, false);//DDS 포맷은 상하가 반전되어 있다
	//glUniform1i(loc_original_texture, texnum_ori);		//비교 대상 텍스쳐(원본)

	if (type_of_compressed_image == 0) {//RGB image
		glUniform1i(loc_base_texture, texnum_comp);			//랜더링할 텍스쳐
		//printf("ori : %d texnum : %d\n", texnum_ori, texnum_comp);
	}
	else {//yuv image
		//glUniform1i(loc_flag_texture_reverse, true);//DDS 포맷은 상하가 반전되어 있다
		int texindex = texnum_comp - TEXTURE_INDEX_COMPRESS_TEST1;
		int y = texindex - texindex % 3 + TEXTURE_INDEX_COMPRESS_TEST1, u = y + 1, v = y + 2;
		glUniform1i(loc_base_texture_y, y);			//랜더링할 텍스쳐 y
		glUniform1i(loc_base_texture_u, u);			//랜더링할 텍스쳐 u
		glUniform1i(loc_base_texture_v, v);			//랜더링할 텍스쳐 v
		glUniform1i(loc_base_texture_index, texindex % 3 + 1);			//랜더링할 텍스처 안에서의 이미지 번호. 한 텍스처에 3개의 이미지 삽입
		//printf("texcomp : %d. ", texnum_comp);
		//printf("texnum : %d, y=%d u=%d v=%d\n", texindex % 3 + 1, y+24,u+24,v+24);
	}

	//glUniform1i(loc_base_texture_y, TEXTURE_TEST_R);			//랜더링할 텍스쳐 y
	//glUniform1i(loc_base_texture_u, TEXTURE_TEST_G);			//랜더링할 텍스쳐 u
	//glUniform1i(loc_base_texture_v, TEXTURE_TEST_B);			//랜더링할 텍스쳐 v

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f - IMGSIZE, -IMGSIZE / 2, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(IMGSIZE, IMGSIZE, IMGSIZE));
	ModelViewMatrix = ViewMatrix * ModelMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));

	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_quad();

	glUseProgram(0);
}

//정의 시 1000frame 랜더링 시간 측정.
//#define TIMETEST 0

int timer_cnt = 0;

// callbacks
void display(void) {
#ifdef TIMETEST
	float dislaytime = 0.0f;
	__int64 start_main, freq_main, end_main;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq_main); QueryPerformanceCounter((LARGE_INTEGER*)&start_main)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end_main); a = (float)((float)(end_main - start_main) / (freq_main / 1000.0f))

	float computetime;

	timer_cnt++;

	CHECK_TIME_START;
	glClearColor(1, 0, 0, 0);
	glViewport(0, 0, WINDOW_param.width, WINDOW_param.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (draw_mode != 2)
		draw_original_texture();
	if (draw_mode != 1)
		draw_compressed_texture();
	CHECK_TIME_END(computetime);

	dislaytime += computetime;

	if (timer_cnt == 1000) {
		printf("1000frame time for ");
		switch (draw_mode) {
		case 0: printf("draw all image"); break;
		case 1: printf("draw original image"); break;
		case 2: printf("draw compressed image"); break;
		}
		printf(" : %f\n", dislaytime);
		timer_cnt = 0; dislaytime = 0.0f;
		if (draw_mode == 1) {

			draw_mode = 2;
		}
		else if (draw_mode == 2) {
			draw_mode = 1;
			texnum_comp++;
			if (texnum_comp >= TEXTURE_INDEX_COMPRESS_TEST20) {
				texnum_comp = TEXTURE_INDEX_COMPRESS_TEST1;
			}

			texnum_ori++;
			if (texnum_ori >= TEXTURE_INDEX_ORIGINAL_TEST20) {
				texnum_ori = TEXTURE_INDEX_ORIGINAL_TEST1;
			}
		}
	}
#else

	setDepthTex();

	glClearColor(1, 0, 0, 0);
	glViewport(0, 0, WINDOW_param.width, WINDOW_param.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (draw_mode != 2)
		draw_original_texture();
	if (draw_mode != 1)
		//draw_compressed_texture_yuv();
		draw_compressed_texture();

#endif
	glutSwapBuffers();
}

glm::vec3 campos;

void setCamera() {
	ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -IMGSIZE * 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ViewMatrix = glm::translate(ViewMatrix, campos);
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	//int i;
	float dist = 50.0f;
	switch (key) {

	case 'a':
		campos.x -= dist;
		setCamera();
		break;
	case 'd':
		campos.x += dist;
		setCamera();
		break;
	case 'w':
		campos.z -= dist;
		setCamera();
		break;
	case 's':
		campos.z += dist;
		setCamera();
		break;
	case 'z':
		campos.y += dist;
		setCamera();
		break;
	case 'c':
		campos.y -= dist;
		setCamera();
		break;

	case 'p':
		flag.texture_diffrence = 1 - flag.texture_diffrence;
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_flag_texture_diffrence, flag.texture_diffrence);
		glUseProgram(0);
		glutPostRedisplay();
		break;
	case 'n':
		timer_cnt = 0;
		draw_mode++;
		draw_mode %= 3;
		switch (draw_mode) {
		case 0: printf("draw all image\n"); break;
		case 1: printf("draw original image\n"); break;
		case 2: printf("draw compressed image\n"); break;
		}

		glutSetWindowTitle(titleTex[texnum_comp]);
		glutPostRedisplay();
		break;
	case 'v':
		flag.depth_ot_color= 1 - flag.depth_ot_color;
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_flag_depth_or_color, flag.depth_ot_color);
		glUseProgram(0);
		switch (flag.depth_ot_color) {
		case 0:printf("color mod\n");
			break;
		case 1:printf("depth mod\n");
			break;
		}

		glutPostRedisplay();
		break;
	case 'h':
		depth_type++;

		if (depth_type > 5) {
			depth_type = 0;
		}
		switch (depth_type) {
		case 0:printf("gray lower\n");
			glutSetWindowTitle("gray lower"); break;
		case 1:printf("4frame lower\n");
			glutSetWindowTitle("4frame lower"); break;
		case 4:printf("A lower\n");
			glutSetWindowTitle("A lower"); break;
		case 3:printf("RGBA Lower\n");
			glutSetWindowTitle("RGBA Lower"); break;
		case 2:printf("4cam Lower\n");
			glutSetWindowTitle("4cam Lower"); break;
		case 5:printf("uncomp Lower\n");
			glutSetWindowTitle("uncomp Lower"); break;
		}

		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_depth_type, depth_type);
		glUseProgram(0);

		break;
	case 'b':
		depth_version++;

		if (depth_version >3) {
			depth_version = 0;
		}
		switch (depth_version) {
		case 0:printf("depth_uncomp_16bit\n");
			glutSetWindowTitle("depth_uncomp_16bit"); break;
		case 1:printf("depth_uncomp upper/ BPTC lower\n");
			glutSetWindowTitle("depth_uncomp upper/ BPTC lower"); break;
		case 2:printf("depth_BPTC upper/ uncomp lower\n");
			glutSetWindowTitle("depth_BPTC upper/ uncomp lower"); break;
		case 3:printf("depth_BPTC upper/ BPTC lower\n");
			glutSetWindowTitle("depth_BPTC upper/ BPTC lower"); break;
		case 4:printf("depth_DXT1_High Fill\n");
			glutSetWindowTitle("depth_DXT1_High Fill"); break;
		case 5:printf("depth_BPTC_high Fill\n");
			glutSetWindowTitle("depth_BPTC_high Fill"); break;
		case 99:printf("depth_uncomp upper/ uncomp lower\n");
			glutSetWindowTitle("depth_uncomp upper/ uncomp lower"); break;
		}
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_depth_version, depth_version);
		glUseProgram(0);
		break;

	case 'm':
		texnum_depth++;

		if (texnum_depth >= 0 + TEST_IMAGE_COUNT) {
			texnum_depth = 0;
		}
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_depth_num, texnum_depth);
		glUseProgram(0);

		texnum_comp++;
		if (texnum_comp >= TEXTURE_INDEX_COMPRESS_TEST1 + TEST_IMAGE_COUNT) {
			texnum_comp = TEXTURE_INDEX_COMPRESS_TEST1;
		}

		texnum_ori++;
		if (texnum_ori >= TEXTURE_INDEX_ORIGINAL_TEST1 + TEST_IMAGE_COUNT ) {
			texnum_ori = TEXTURE_INDEX_ORIGINAL_TEST1;
		}

		glutSetWindowTitle(titleTex[texnum_comp]);
		glutPostRedisplay();
		break;

	case 't':
		flag.texture_mapping = 1 - flag.texture_mapping;
		glUseProgram(h_ShaderProgram_TXPS);
		glUniform1i(loc_flag_texture_mapping, flag.texture_mapping);
		glUseProgram(0);
		glutPostRedisplay();
		break;

	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups
		break;
	}
}

void reshape(int width, int height) {
	float aspect_ratio;

	WINDOW_param.width = width;
	WINDOW_param.height = height;

	glViewport(0, 0, width, height);

	aspect_ratio = (float)width / height;
	ProjectionMatrix = glm::perspective(45.0f*TO_RADIAN, aspect_ratio, 1.0f, IMGSIZE*1.6f);

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutCloseFunc(cleanup);
	glutIdleFunc(display);
}

void prepare_shader_program(void) {

	ShaderInfo shader_info_TXPS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong_Tx.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/Phong_Tx.frag" },
	{ GL_NONE, NULL }
	};

	//////////////////
	h_ShaderProgram_TXPS = LoadShaders(shader_info_TXPS);
	loc_ModelViewProjectionMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrixInvTrans");

	loc_typeof_original_image = glGetUniformLocation(h_ShaderProgram_TXPS, "u_type_of_original_image");
	loc_typeof_compressed_image = glGetUniformLocation(h_ShaderProgram_TXPS, "u_type_of_compressed_image");

	loc_drawtype = glGetUniformLocation(h_ShaderProgram_TXPS, "u_drawtype");

	loc_base_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");
	loc_base_texture_y = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture_y");
	loc_base_texture_u = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture_u");
	loc_base_texture_v = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture_v");
	loc_base_texture_index = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_textureIndex");

	loc_original_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_original_texture");
	loc_original_texture_y = glGetUniformLocation(h_ShaderProgram_TXPS, "u_original_texture_y");
	loc_original_texture_u = glGetUniformLocation(h_ShaderProgram_TXPS, "u_original_texture_u");
	loc_original_texture_v = glGetUniformLocation(h_ShaderProgram_TXPS, "u_original_texture_v");
	loc_original_texture_index = glGetUniformLocation(h_ShaderProgram_TXPS, "u_original_textureIndex");

	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_mapping");
	loc_flag_texture_diffrence = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_diffrence");
	loc_flag_texture_reverse = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_reverse");

	loc_flag_depth_or_color = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_Depth_of_color");

	loc_depth_num = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_number");
	loc_depth_version = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_version");
	loc_depth_uncomp_16bit = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_uncomp_16bit");
	loc_depth_uncomp_upper = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_uncomp_upper");
	loc_depth_uncomp_lower = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_uncomp_lower");
	loc_depth_comp_upper = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_upper");
	loc_depth_comp_lower = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_lower");
	loc_depth_comp_split1 = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_split1");
	loc_depth_comp_split2 = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_split2");


	loc_depth_type = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_type");
	loc_depth_lower_gray = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_lower_gray");
	loc_depth_lower_4frame = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_lower_4frame");
	loc_depth_lower_a = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_lower_a");
	loc_depth_lower_rgba = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_lower_rgba");
	loc_depth_lower_4cam = glGetUniformLocation(h_ShaderProgram_TXPS, "u_depth_comp_lower_4cam");
}



void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	campos = glm::vec3(0.0f, 0.0f, 0.0f);
	ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -IMGSIZE * 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//glGenTextures(N_NORMAL_TEXTURES_USED, texture_names);//미리 텍스쳐 생성
}

void depthMapWrite() {
	//한번에 다수의 png를 생성하는데 이게 멀티스레딩 방식인지 뭔지, 제대로 진행되지 않고 끊기는 경우가 있음. 로그를 볼 것.
	char name[24][100];

	for(int i=0;i<24;i++)
		sprintf(name[i],"Data/DepthRawData/v%d_depth_2048x2048_yuv420p16le.data", i);
	char filename[100];

	int readsize = 2048 * 2048 * 2;
	BYTE* readbuf[4];

	int imageSize = 2048 * 2048;
	BYTE* upperBuf[4];
	BYTE* lowerBuf[4];
	unsigned short* usBuf[4];

	for (int i = 0; i < 4; i++) {
		readbuf[i] = new BYTE[readsize];
		upperBuf[i] = new BYTE[imageSize];
		lowerBuf[i] = new BYTE[imageSize];
		usBuf[i] = new unsigned short[imageSize];
	}

	for (int i = 0; i < 6; i++) {

		for (int j = 0; j < 4; j++) {

			FILE* fp = fopen(name[i * 4 + j], "rb");
			fread(readbuf[j], sizeof(BYTE), readsize, fp);

			for (int k = 0; k < readsize; k++) {
				if (k % 2 == 1) {
					upperBuf[j][k / 2] = readbuf[j][k];
				}
				else if (k % 2 == 0) {
					lowerBuf[j][k / 2] = readbuf[j][k];
				}
			}


			for (int k = 0; k < readsize; k++) {
				if (k % 2 == 0) {
					usBuf[j][k / 2] = (unsigned short)upperBuf[j][k / 2] * 256 + (unsigned short)lowerBuf[j][k / 2];
				}
			}

			fclose(fp);
		}

		printf("load 4 image.\n");
		for (int j = 0; j < 4; j++) {
			printf("%s\n",name[i*4+j]);
		}
		
		sprintf(filename, "output/depth_1frame/upperDepth_rgba_%d.png", i);
		FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, upperBuf[0], upperBuf[1], upperBuf[2], upperBuf[3], filename);
		printf("create image %s\n", filename);

		sprintf(filename, "output/depth_1frame/lowerDepth_rgba_%d.png", i);
		FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, lowerBuf[0], lowerBuf[1], lowerBuf[2], lowerBuf[3], filename);
		printf("create image %s\n", filename);

		sprintf(filename, "output/depth_1frame/Depth_rgba_%d.png", i );
		FreeImageSaveFile_16bit_RGBA_4Image(2048, 2048, usBuf[0], usBuf[1], usBuf[2], usBuf[3], filename);
		printf("create image %s\n", filename);

		//for (int j = 0; j < 4; j++) {
		//	sprintf(filename, "output/Depth/split/dxt1/splitDepth_dxt1_min_%d.png", i * 4 + j);
		//	FreeImageSetupRGB_SPLIT_min(2048, 2048, usBuf[j], filename);
		//	printf("create image %s\n", filename);
		//	sprintf(filename, "output/Depth/split/dxt1/splitDepth_dxt1_max_%d.png", i * 4 + j);
		//	FreeImageSetupRGB_SPLIT_max(2048, 2048, usBuf[j], filename);
		//	printf("create image %s\n", filename);

		//	sprintf(filename, "output/Depth/split/bptc/splitDepth_bptc_min_%d.png", i * 4 + j);
		//	FreeImageSetupRGBA_SPLIT_min(2048, 2048, usBuf[j], filename);
		//	printf("create image %s\n", filename);
		//	sprintf(filename, "output/Depth/split/bptc/splitDepth_bptc_max_%d.png", i * 4 + j);
		//	FreeImageSetupRGBA_SPLIT_max(2048, 2048, usBuf[j], filename);
		//	printf("create image %s\n", filename);
		//}
	}


	for (int i = 0; i < 4; i++) {
		delete readbuf[i];
		delete upperBuf[i];
		delete lowerBuf[i];
		delete usBuf[i];
	}



	printf("load depth map.\n");

}

void depthMapWrite_multiframe() {
	//한번에 다수의 png를 생성하는데 이게 멀티스레딩 방식인지 뭔지, 제대로 진행되지 않고 끊기는 경우가 있음. 로그를 볼 것.
	char name[24][100];

	for (int frame = 0; frame < 25; frame+=5) {
			   
		for (int i = 0; i < 24; i++)
			sprintf(name[i], "Data/DepthRawData/test5frame/v%d_depth_2048x2048_yuv420p16le_%d.data", i, frame);
		char filename[100];

		int readsize = 2048 * 2048 * 2;
		BYTE* readbuf[4];

		int imageSize = 2048 * 2048;
		BYTE* upperBuf[4];
		BYTE* lowerBuf[4];
		unsigned short* usBuf[4];

		for (int i = 0; i < 4; i++) {
			readbuf[i] = new BYTE[readsize];
			upperBuf[i] = new BYTE[imageSize];
			lowerBuf[i] = new BYTE[imageSize];
			usBuf[i] = new unsigned short[imageSize];
		}

		for (int i = 0; i < 6; i++) {

			for (int j = 0; j < 4; j++) {

				FILE* fp = fopen(name[i * 4 + j], "rb");
				fread(readbuf[j], sizeof(BYTE), readsize, fp);

				for (int k = 0; k < readsize; k++) {
					if (k % 2 == 1) {
						upperBuf[j][k / 2] = readbuf[j][k];
					}
					else if (k % 2 == 0) {
						lowerBuf[j][k / 2] = readbuf[j][k];
					}
				}


				for (int k = 0; k < readsize; k++) {
					if (k % 2 == 0) {
						usBuf[j][k / 2] = (unsigned short)upperBuf[j][k / 2] * 256 + (unsigned short)lowerBuf[j][k / 2];
					}
				}

				fclose(fp);
			}

			printf("load 4 image.\n");
			for (int j = 0; j < 4; j++) {
				printf("%s\n", name[i * 4 + j]);
			}


			sprintf(filename, "output/depth_5frame/upperDepth_rgba_%d_%d.png", i,frame);
			FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, upperBuf[0], upperBuf[1], upperBuf[2], upperBuf[3], filename);
			printf("create image %s\n", filename);

			sprintf(filename, "output/depth_5frame/lowerDepth_rgba_%d_%d.png", i, frame);
			FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, lowerBuf[0], lowerBuf[1], lowerBuf[2], lowerBuf[3], filename);
			printf("create image %s\n", filename);

			sprintf(filename, "output/depth_5frame/Depth_rgba_%d_%d.png", i, frame);
			FreeImageSaveFile_16bit_RGBA_4Image(2048, 2048, usBuf[0], usBuf[1], usBuf[2], usBuf[3], filename);
			printf("create image %s\n", filename);

			//for (int j = 0; j < 4; j++) {
			//	sprintf(filename, "output/Depth/split/dxt1/splitDepth_dxt1_min_%d.png", i * 4 + j);
			//	FreeImageSetupRGB_SPLIT_min(2048, 2048, usBuf[j], filename);
			//	printf("create image %s\n", filename);
			//	sprintf(filename, "output/Depth/split/dxt1/splitDepth_dxt1_max_%d.png", i * 4 + j);
			//	FreeImageSetupRGB_SPLIT_max(2048, 2048, usBuf[j], filename);
			//	printf("create image %s\n", filename);

			//	sprintf(filename, "output/Depth/split/bptc/splitDepth_bptc_min_%d.png", i * 4 + j);
			//	FreeImageSetupRGBA_SPLIT_min(2048, 2048, usBuf[j], filename);
			//	printf("create image %s\n", filename);
			//	sprintf(filename, "output/Depth/split/bptc/splitDepth_bptc_max_%d.png", i * 4 + j);
			//	FreeImageSetupRGBA_SPLIT_max(2048, 2048, usBuf[j], filename);
			//	printf("create image %s\n", filename);
			//}
		}

		for (int i = 0; i < 4; i++) {
			delete readbuf[i];
			delete upperBuf[i];
			delete lowerBuf[i];
			delete usBuf[i];
		}
	}

	printf("create depth map.\n");

}

float load_rgb_Image(const char *filename, BYTE* depthBuf, int index, int imagesize) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	BYTE *data;

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
	BYTE* rgbBuf[4];

	for (int j = 0; j < 4; j++) {
		rgbBuf[j] = new BYTE[imagesize];
	}

	//TODO : 이거 색상별로 나누기!

	for (int i = 0; i < imagesize; i++) {
		rgbBuf[0][i] = (BYTE)(data[i * 4 + 0]);//B
		rgbBuf[1][i] = (BYTE)(data[i * 4 + 1]);//G
		rgbBuf[2][i] = (BYTE)(data[i * 4 + 2]);//R
		rgbBuf[3][i] = (BYTE)(data[i * 4 + 3]);//A (255)
		if (i < 10 && index == 0)
			printf("[%d] %x %x %x %x\n", i, (int)rgbBuf[0][i], (int)rgbBuf[1][i], (int)rgbBuf[2][i], (int)rgbBuf[3][i]);
	}

	//return 0;

	char savename[100];
	sprintf(savename, "output/lowerDepth_8bit_a_%d.png", index);
	FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, rgbBuf[2], rgbBuf[1], rgbBuf[0], depthBuf, savename);
	printf("create image %s\n", savename);

	//fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);

	return 0;
}

void depthMapWrite_test() {
	char name[4][24][100];

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 24; j++)
		sprintf(name[i][j], "Data/DepthRawData/5frame/v%d_depth_2048x2048_yuv420p16le_%d.data", j,i);
	char filename[100];

	int readsize = 2048 * 2048 * 2;
	BYTE* readbuf[4][24];

	int imageSize = 2048 * 2048;
	BYTE* upperBuf[4][24];
	BYTE* lowerBuf[4][24];
	unsigned short* usBuf[4][24];
	BYTE* blank;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 24; j++) {
			readbuf[i][j] = new BYTE[readsize];
			lowerBuf[i][j] = new BYTE[imageSize];
			upperBuf[i][j] = new BYTE[imageSize];
		}
	}
	blank = new  BYTE[imageSize];
	memset(blank, 0, sizeof(BYTE)*imageSize);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 24; j++) {

			FILE* fp = fopen(name[i][j], "rb");
			fread(readbuf[i][j], sizeof(BYTE), readsize, fp);

			for (int k = 0; k < readsize; k++) {
				if (k % 2 == 1) {
					upperBuf[i][j][k / 2] = readbuf[i][j][k];
				}
				else if (k % 2 == 0) {
					lowerBuf[i][j][k / 2] = readbuf[i][j][k];
				}
			}
					   
			fclose(fp);
		}
	}


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 24; j++) {
			delete readbuf[i][j];
			usBuf[i][j] = new unsigned short[imageSize];
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 24; j++) {
			for (int k = 0; k < readsize; k++) {
				if (k % 2 == 0) {
					usBuf[i][j][k / 2] = (unsigned short)upperBuf[i][j][k / 2] * 256 + (unsigned short)lowerBuf[i][j][k / 2];
				}
			}
		}
	}

	printf("load all image.\n");

	//original
	//for (int i = 0; i < 24; i++) {
	//	sprintf(filename, "output/Depth_Original_%d.png", i);
	//	FreeImageSaveFile_16bit_RGBA_4Image(2048, 2048, usBuf[0][i], usBuf[1][i], usBuf[2][i], usBuf[3][i], filename);
	//	printf("create image %s\n", filename);


	//	printf("==DEBUG=== %d, %d\n", i, 0);
	//	for (int k = 0; k < 10; k++) {
	//		printf("%d ", usBuf[0][i][k]);
	//	}
	//	printf("\n");
	//	for (int k = 0; k < 10; k++) {
	//		printf("%d ", upperBuf[0][i][k]);
	//	}
	//	printf("\n");
	//	for (int k = 0; k < 10; k++) {
	//		printf("%d ", lowerBuf[0][i][k]);
	//	}
	//	printf("\n");

	//}
	//return;

	//bc4(grayscale)
	for (int frame = 0; frame < 4; frame++) {
		for (int i = 0; i < 24; i++) {
			sprintf(filename, "output_ori/lowerDepth_8bit_gray_%d_%d.png", i,frame);
			FreeImageSaveFile_8bit_grayscale(2048, 2048, lowerBuf[frame][i], filename);
			printf("create image %s\n", filename);
		}
	}
	return;

	//rxxx, 0 frame
	for (int i = 0; i < 24; i++) {
		sprintf(filename, "output/lowerDepth_8bit_rxxx_%d.png", i);
		FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, lowerBuf[0][i], blank, blank, blank, filename);
		printf("create image %s\n", filename);
	}


	//r-g-b-a, 0 frame
	for (int i = 0; i < 24; i++) {
		sprintf(filename, "output/lowerDepth_8bit_rgba_%d.png", i);
		FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, lowerBuf[0][i], lowerBuf[0][i], lowerBuf[0][i], lowerBuf[0][i], filename);
		printf("create image %s\n", filename);
	}

	//4 frame
	for (int i = 0; i < 24; i++) {
		sprintf(filename, "output/lowerDepth_8bit_4frame_%d.png", i);
		FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, lowerBuf[0][i], lowerBuf[1][i], lowerBuf[2][i], lowerBuf[3][i], filename);
		printf("create image %s\n", filename);
	}


	//rgb + a
	for (int i = 0; i < 24; i++) {
		sprintf(filename, "data/yuv/original_rgb/image_rgb_%d.bmp", i);
		load_rgb_Image(filename, lowerBuf[0][i], i, imageSize);
	}


	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 24; j++) {
			delete usBuf[i][j];
			delete lowerBuf[i][j];
			delete upperBuf[i][j];
		}
	}

	printf("load test depth map.\n");
	exit(0);
}


void depthMapWrite_test_2() {

	int readsize = 2048 * 2048 * 2;
	BYTE* readbuf[24];

	int imageSize = 2048 * 2048;
	BYTE* lowerBuf[24];
	BYTE* blank;

	for (int j = 0; j < 24; j++) {
		readbuf[j] = new BYTE[readsize];
		lowerBuf[j] = new BYTE[imageSize];
	}

	for (int frame = 0; frame < 12; frame++) {

		char name[24][100];

		for (int j = 0; j < 24; j++)
			sprintf(name[j], "Data/DepthRawData/12frame/v%d_depth_2048x2048_yuv420p16le_%d.data", j, frame);
		char filename[100];

		blank = new  BYTE[imageSize];
		memset(blank, 0, sizeof(BYTE)*imageSize);

		for (int j = 0; j < 24; j++) {

			FILE* fp = fopen(name[j], "rb");
			fread(readbuf[j], sizeof(BYTE), readsize, fp);

			for (int k = 0; k < readsize; k++) {
				if (k % 2 == 1) {
					//upperBuf[i][j][k / 2] = readbuf[i][j][k];
				}
				else if (k % 2 == 0) {
					lowerBuf[j][k / 2] = readbuf[j][k];
				}
			}

			fclose(fp);
		}

		printf("load all image.\n");

		//bc4(grayscale)
		for (int i = 0; i < 24; i++) {
			sprintf(filename, "output/depthTex_lower_%d_%d.png", i, frame);
			FreeImageSaveFile_8bit_grayscale(2048, 2048, lowerBuf[i], filename);
			printf("create image %s\n", filename);
		}
	}

		for (int j = 0; j < 24; j++) {
			delete lowerBuf[j];
		}
	

	printf("load test depth map.\n");

}


void depthMapWrite_test_3() {

	int readsize = 2048 * 2048 * 2;
	BYTE* readbuf[4];

	int imageSize = 2048 * 2048;
	BYTE* upperBuf[4];
	BYTE* lowerBuf[4];

	char name[4][100];
	char filename[100];

	for (int j = 0; j < 24; j++) {
		readbuf[j] = new BYTE[readsize];
		upperBuf[j] = new BYTE[imageSize];
		lowerBuf[j] = new BYTE[imageSize];
	}

	for (int cam = 0; cam < 24; cam++) {
		for (int frame = 0; frame < 300; frame+=4) {

			for (int j = 0; j < 4; j++) {
				sprintf(name[j], "D:/texout/debugraw/v%d_depth_2048x2048_yuv420p16le_%d.data", cam, frame + j);

				FILE* fp = fopen(name[j], "rb");
				fread(readbuf[j], sizeof(BYTE), readsize, fp);

				for (int k = 0; k < readsize; k++) {
					if (k % 2 == 1) {
						upperBuf[j][k / 2] = readbuf[j][k];
					}
					else if (k % 2 == 0) {
						lowerBuf[j][k / 2] = readbuf[j][k];
					}
				}

				fclose(fp);

			}

			for (int i = 0; i < 4; i++) {
				sprintf(filename, "D:/texout/newdepth/depthTex_lower_%d_%d.png", cam, frame+i);
				FreeImageSaveFile_8bit_grayscale(2048, 2048, lowerBuf[i], filename);
				printf("create image %s\n", filename);
			}

			sprintf(filename, "D:/texout/newdepth/depthTex_upper_%d_%d.png", cam, frame/4);
			FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, upperBuf[0], upperBuf[1], upperBuf[2], upperBuf[3], filename);
			printf("create image %s\n", filename);			

		}
	}


	for (int j = 0; j < 4; j++) {
		delete[] lowerBuf[j];
		delete[] upperBuf[j];
	}


	printf("load test depth map.\n");

}


void depth_color_CombineTest() {

	char name[24][100];

	for (int j = 0; j < 24; j++)
		sprintf(name[j], "Data/DepthRawData/v%d_depth_2048x2048_yuv420p16le.data", j);
	char filename[100];

	int readsize = 2048 * 2048 * 2;
	BYTE* readbuf[24];

	int imageSize = 2048 * 2048;
	BYTE* lowerBuf[24];
	BYTE* blank;

	for (int j = 0; j < 24; j++) {
		readbuf[j] = new BYTE[readsize];
		lowerBuf[j] = new BYTE[imageSize];
	}

	blank = new  BYTE[imageSize];
	memset(blank, 0, sizeof(blank));

	for (int j = 0; j < 24; j++) {

		FILE* fp = fopen(name[j], "rb");
		fread(readbuf[j], sizeof(BYTE), readsize, fp);

		for (int k = 0; k < readsize; k++) {
			if (k % 2 == 0) {
				lowerBuf[j][k / 2] = readbuf[j][k];
			}
		}


		fclose(fp);
	}



	for (int j = 0; j < 24; j++) {
		delete readbuf[j];
	}


	printf("load all image.\n");



	for (int i = 0; i < 24; i++) {

		sprintf(filename, "data/yuv/original_rgb/image_rgb_%d.bmp", i);
		load_rgb_Image(filename, lowerBuf[i], i, imageSize);

		//sprintf(filename, "output/lowerDepth_8bit_rxxx_%d.png", i);
		//FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, lowerBuf[0][i], blank, blank, blank, filename);
		//printf("create image %s\n", filename);

	}



}

void createTestBitmap() {
	char filename[100];

	int imageSize = 16 * 16;
	BYTE* upperBuf[4];
	BYTE* lowerBuf[4];
	unsigned short* usBuf[4];

	for (int i = 0; i < 4; i++) {
		upperBuf[i] = new BYTE[imageSize];
		lowerBuf[i] = new BYTE[imageSize];
		usBuf[i] = new unsigned short[imageSize];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < imageSize; j++) {
			lowerBuf[i][j] = j;
			upperBuf[i][j] = imageSize-j-1;
			usBuf[i][j] = (unsigned short)upperBuf[i][j] * 256 + lowerBuf[i][j];
		/*	if (i == 3) {
				usBuf[i][j] = 65535;
				lowerBuf[i][j] = 255;
				upperBuf[i][j] = 255;
			}*/
		}
	}


	sprintf(filename, "TestImg_8bit_lower.png");
	FreeImageSaveFile_8bit_RGBA_4Image(16, 16, lowerBuf[0], lowerBuf[1], lowerBuf[2], lowerBuf[3], filename);
	printf("create image %s\n", filename);

	sprintf(filename, "TestImg_8bit_upper.png");
	FreeImageSaveFile_8bit_RGBA_4Image(16, 16, upperBuf[0], upperBuf[1], upperBuf[2], upperBuf[3], filename);
	printf("create image %s\n", filename);

	sprintf(filename, "TestImg_16bit.png");
	FreeImageSaveFile_16bit_RGBA_4Image(16, 16, usBuf[0], usBuf[1], usBuf[2], usBuf[3], filename);
	printf("create image %s\n", filename);
}


void createTestBitmap_compress() {
	char filename[100];

	int imageSize = 2048 * 2048;
	BYTE* buf[2];//r,g,b,0
	
	for (int i = 0; i < 2; i++) {
		buf[i] = new BYTE[imageSize];
	}
	for (int i = 0; i < imageSize; i++) {
		buf[0][i] = 255;
		buf[1][i] = 0;
	}
	   
	sprintf(filename, "TestImg_R.png");
	FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, buf[0], buf[1], buf[1], buf[0], filename);
	printf("create image %s\n", filename);

	sprintf(filename, "TestImg_G.png");
	FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, buf[1], buf[0], buf[1], buf[0], filename);
	printf("create image %s\n", filename);

	sprintf(filename, "TestImg_B.png");
	FreeImageSaveFile_8bit_RGBA_4Image(2048, 2048, buf[1], buf[1], buf[0], buf[0], filename);
	printf("create image %s\n", filename);

}


void prepare_scene(void) {
	prepare_quad();
	//create_ORIGINAL_texture("Data/4kimg.jpg", TEXTURE_INDEX_ORIGINAL);
	//create_ORIGINAL_texture("Data/grass_tex.jpg", TEXTURE_INDEX_TEST);

	//createTestBitmap_compress();
	//create_ORIGINAL_texture("TestImg_R.png", TEXTURE_INDEX_COMPRESS_TEST1);
	//create_ORIGINAL_texture("TestImg_R.png", TEXTURE_INDEX_COMPRESS_TEST1);
	//create_ORIGINAL_texture("TestImg_G.png", TEXTURE_INDEX_COMPRESS_TEST2);
	//create_ORIGINAL_texture("TestImg_B.png", TEXTURE_INDEX_COMPRESS_TEST3);
	//type_of_compressed_image = 1;
	//type_of_original_image = 1;


	//create_DDS_Texture("test_rgba/image_a.DDS", TEXTURE_INDEX_ORIGINAL_TEST2);
	//create_DDS_Texture("test_rgb/image_rgb.DDS", TEXTURE_INDEX_ORIGINAL_TEST1);
	//create_DDS_Texture("test_rgb/image_r.DDS", TEXTURE_INDEX_COMPRESS_TEST1);
	//create_DDS_Texture("test_rgb/image_g.DDS", TEXTURE_INDEX_COMPRESS_TEST2);
	//create_DDS_Texture("test_rgb/image_b.DDS", TEXTURE_INDEX_COMPRESS_TEST3);
	//create_DDS_Texture("image_a.DDS", TEXTURE_INDEX_COMPRESS_TEST3);

	//create_ORIGINAL_RGBA_16_texture("TestImg_16bit.png", TEXTURE_INDEX_ORIGINAL);
	//create_ORIGINAL_texture("TestImg_8bit_upper.png", TEXTURE_INDEX_TEST);
	//create_ORIGINAL_texture("TestImg_8bit_lower.png", TEXTURE_INDEX_TEST+1);

	//test_loadimg("Data/Depth/original/Depth_rgba_0.png", "Data/Depth/original/upperDepth_rgba_0.png", "Data/Depth/original/lowerDepth_rgba_0.png");
	//test_loadimg("TestImg_16bit.png", "TestImg_8bit_upper.png", "TestImg_8bit_lower.png");

	//depthMapWrite_test_2();
	//for (int i = 0; i < 24; i++) {

	//	for (int j = 0; j < 5; j++) {
	//		char name[2][100];
	//		sprintf(name[0], "Data/DepthRawData/5frame/v%d_depth_2048x2048_yuv420p16le_%d.data", i, j);
	//		sprintf(name[1], "Data/DepthRawData/12frame/v%d_depth_2048x2048_yuv420p16le_%d.data", i, j);
	//		printf("%d %d - ", i, j);
	//		combine_2File(name[0], name[1]);
	//	}
	//}


	//for (int i = 0; i < 24; i++) {
	//	for (int j = 0; j < 12; j++) {
	//		char name[2][100];
	//		sprintf(name[0], "output/lowerDepth_8bit_gray_%d_%d.png", i, j);
	//		//sprintf(name[1], "output_ori/lowerDepth_8bit_gray_%d_%d.png", i, j);
	//		sprintf(name[1],"D:/texout/newdepth/depthTex_lower_%d_%d.png",i,j);
	//		printf("%d %d - " , i,j);
	//		test_loadimg_diff(name[0], name[1]);
	//	}
	//}

	//depthMapWrite_test_3();

	//test_loadimg_diff("output/lowerDepth_8bit_gray_0_0.png", "debugtex/depthTex_lower_0_0.png");
	//test_loadimg_diff("output/old_tex/upperDepth_rgba_0.png", "output/new_tex_1/upperDepth_rgba_0.png", "output/new_tex_2/upperDepth_rgba_0_0.png");
	//test_loadimg_diff_bptc("output/comp/old_tex/upperDepth_rgba_0.dds", "output/comp/new_tex_1/upperDepth_rgba_0.dds", "output/comp/new_tex_2/upperDepth_rgba_0_0.dds");
	//create_ORIGINAL_RGBA_16_texture("Depth_16bit_0.png", TEXTURE_INDEX_ORIGINAL);
	//create_ORIGINAL_texture("upperDepth_8bit_0.png", TEXTURE_INDEX_TEST);
	//create_ORIGINAL_texture("lowerDepth_8bit_0.png", TEXTURE_INDEX_TEST + 1);
	//test_loadimg("Data/depth/original/Depth_rgba_0.png", "data/depth/original/upperDepth_rgba_0.png", "data/depth/original/lowerDepth_rgba_0.png");
	//depthMapWrite();
	//depthMapWrite_multiframe();
	//createTestBitmap();
	//depthMapWrite_test();
	//depth_color_CombineTest();

	upload_TEST_Texture_Original();
	//upload_TEST_Texture_Original_YUV();
	upload_TEST_Texture_DXT(1);
	//upload_TEST_Texture_DXT(3);
	//upload_TEST_Texture_DXT(5);
	//upload_TEST_Texture_BPTC();
	//upload_TEST_Texture_ASTC(4);
	//upload_TEST_Texture_ASTC(5);
	//upload_TEST_Texture_ASTC(6);
	//upload_TEST_Texture_ASTC(8);
	//upload_TEST_Texture_ASTC(10);
	//upload_TEST_Texture_ASTC(12);
	//upload_TEST_Texture_ASTC(112);
	//upload_TEST_Texture_YUV_DXT1();
	//upload_TEST_Texture_YUV_BPTC();
	upload_TEST_Texture_Depth();
	//compare_PSNR();

	//upload_TEST_Texture_Depth_MultiType(0);
	//upload_TEST_Texture_Depth_MultiType(1);
	//upload_TEST_Texture_Depth_MultiType(2);
	//upload_TEST_Texture_Depth_MultiType(3);
	//upload_TEST_Texture_Depth_MultiType(4);
	//upload_TEST_Texture_Depth_MultiType(5);
	int result;

	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &result);
	printf("max tex : %d\n",result);

	//create_DDS_Texture("Data/depth/bc7/upperDepth_rgba.DDS", TEXTURE_TEST_R);
	//create_ORIGINAL_RGBA_texture("Data/depth/original/upperDepth_rgba.png", TEXTURE_TEST_G);

	//exit(0);
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();

}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE3170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n'w', 's', 'a', 'd', 'z', 'c' => Move camera\n");
	fprintf(stdout, "'p' => View Diffrence\n");
	fprintf(stdout, "'v' => Toggle View Depth/Color\n");
	fprintf(stdout, "'b' => Change Depth Map Version\n");
	fprintf(stdout, "'n' => Change Draw Mode\n");
	fprintf(stdout, "'m' => View Next Image\n");
	fprintf(stdout, "'t' => Toggle Texture\n");
	fprintf(stdout, "**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
int main(int argc, char *argv[]) {
	// Phong Shading
	char program_name[64] = "Texture compress 에 따른 비교";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: 'w', 's', 'a', 'd', 'z', 'c', 'p', 'n', 'm', 't' 'ESC'" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize((int)IMGSIZE * 2, (int)IMGSIZE);
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();

	return 0;
}