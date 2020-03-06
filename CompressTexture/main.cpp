//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"
#include <time.h>


GLuint h_ShaderProgram_simple, h_ShaderProgram_TXPS, h_ShaderProgram_SHOW_SM;

// for simple shaders
GLint loc_ModelViewProjectionMatrix_simple, loc_primitive_color;

// for Phong Shading shaders
#define NUMBER_OF_LIGHT_SUPPORTED 4 
#define NUMBER_OF_LIGHT_COUNT 4
//GLint loc_global_ambient_color;
//loc_light_Parameters loc_light[NUMBER_OF_LIGHT_SUPPORTED];
//loc_Material_Parameters loc_material;
GLint loc_ModelViewProjectionMatrix_TXPS, loc_ModelViewMatrix_TXPS, loc_ModelViewMatrixInvTrans_TXPS;
GLint loc_base_texture, loc_original_texture, loc_flag_texture_mapping, loc_flag_texture_diffrence, loc_flag_texture_reverse, loc_flag_fog;


// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.
glm::mat4 ModelViewProjectionMatrix, ModelViewMatrix;
glm::mat3 ModelViewMatrixInvTrans;
glm::mat4 ViewMatrix, ProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

// lights in scene
//Light_Parameters light[NUMBER_OF_LIGHT_SUPPORTED];
float light0_position_WC[2][4], light0_lerp_t; // for light animation

struct _flag {
	int texture_mapping;
	int texture_diffrence;
} flag;


struct _WINDOW_param {
	int width, height;
} WINDOW_param;

// texture stuffs

//#define TEXTURE_INDEX_TIGER 1
//#define TEXTURE_INDEX_SHADOW 2

#include "ImageLoader.h"

GLuint texture_names[N_NORMAL_TEXTURES_USED];

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
	  "TEST_Original_20"
};



// for tiger animation
//int cur_frame_tiger = 0;
//float rotation_angle_tiger = 0.0f;
//int rotation_speed_tiger = 100;

#include "Objects.h"

#define IMGSIZE 1024.0f
GLuint texnum_ori = TEXTURE_INDEX_ORIGINAL_TEST1;
GLuint texnum_comp = TEXTURE_INDEX_COMPRESS_TEST1;

int draw_mode = 0;

void draw_original_texture() {
	glm::mat4 ModelMatrix;


	//glUseProgram(h_ShaderProgram_simple);
	//ModelViewMatrix = glm::scale(ViewMatrix, glm::vec3(50.0f, 50.0f, 50.0f));
	//ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	//glUniformMatrix4fv(loc_ModelViewProjectionMatrix_simple, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	//glLineWidth(2.0f);
	//draw_axes();
	//glLineWidth(1.0f);

	glUseProgram(h_ShaderProgram_TXPS);
	//glUniform1i(loc_shadow_texture, ShadowMapping.texture_unit);

	glUniform1i(loc_flag_texture_reverse, false);
	//set_material_floor();
	//glUniform1i(loc_base_texture, TEXTURE_INDEX_ORIGINAL);
	glUniform1i(loc_base_texture, texnum_ori);
	//glUniform1i(loc_original_texture, TEXTURE_INDEX_TEST);
	glUniform1i(loc_original_texture, texnum_ori);

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, -IMGSIZE/2, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(IMGSIZE, IMGSIZE, IMGSIZE));
	//ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
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
	//glUniform1i(loc_shadow_texture, ShadowMapping.texture_unit);

	glUniform1i(loc_flag_texture_reverse, true);
	//set_material_floor();
	glUniform1i(loc_base_texture, texnum_comp);
	//glUniform1i(loc_base_texture, TEXTURE_INDEX_TEST);
	glUniform1i(loc_original_texture, texnum_ori);
	//glUniform1i(loc_original_texture, TEXTURE_INDEX_COMPRESS_ASTC12X12);

	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f- IMGSIZE, -IMGSIZE/2, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(IMGSIZE, IMGSIZE, IMGSIZE));
	//ModelMatrix = glm::rotate(ModelMatrix, -90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewMatrix = ViewMatrix * ModelMatrix;
	ModelViewProjectionMatrix = ProjectionMatrix * ModelViewMatrix;
	ModelViewMatrixInvTrans = glm::inverseTranspose(glm::mat3(ModelViewMatrix));


	glUniformMatrix4fv(loc_ModelViewProjectionMatrix_TXPS, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(loc_ModelViewMatrix_TXPS, 1, GL_FALSE, &ModelViewMatrix[0][0]);
	glUniformMatrix3fv(loc_ModelViewMatrixInvTrans_TXPS, 1, GL_FALSE, &ModelViewMatrixInvTrans[0][0]);
	draw_quad();

	glUseProgram(0);

}

//#define TIMETEST 0
int timer_cnt = 0;
float dislaytime = 0.0f;

__int64 start_main, freq_main, end_main;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq_main); QueryPerformanceCounter((LARGE_INTEGER*)&start_main)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end_main); a = (float)((float)(end_main - start_main) / (freq_main / 1000.0f))
// callbacks
void display(void) {
#ifdef TIMETEST
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
	
	glClearColor(1, 0, 0, 0);
	glViewport(0, 0, WINDOW_param.width, WINDOW_param.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (draw_mode != 2)
		draw_original_texture();
	if (draw_mode != 1)
		draw_compressed_texture();
	
#endif
	

	glutSwapBuffers();
}

glm::vec3 campos;

void setCamera() {
	ViewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, -IMGSIZE*1.5f), glm::vec3(0.0f, 0.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f));
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
	case 'm':
		//printf("log, texnum : %d\n",texnum);
		texnum_comp++;
		if (texnum_comp >= TEXTURE_INDEX_COMPRESS_TEST20) {
			texnum_comp = TEXTURE_INDEX_COMPRESS_TEST1;
		}

		texnum_ori++;
		if (texnum_ori >= TEXTURE_INDEX_ORIGINAL_TEST20) {
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
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);

	glDeleteVertexArrays(1, &tiger_VAO);
	glDeleteBuffers(1, &tiger_VBO);

	glDeleteVertexArrays(1, &rectangle_VAO);
	glDeleteBuffers(1, &rectangle_VBO);
	
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	//glutTimerFunc(rotation_speed_tiger, timer_scene, 1);
	glutCloseFunc(cleanup);
	glutIdleFunc(display);
}

void prepare_shader_program(void) {
//	int i;
	//char string[256];
	ShaderInfo shader_info_simple[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
	{ GL_NONE, NULL }
	};
	ShaderInfo shader_info_TXPS[3] = {
		{ GL_VERTEX_SHADER, "Shaders/Phong_Tx.vert" },
	{ GL_FRAGMENT_SHADER, "Shaders/Phong_Tx.frag" },
	{ GL_NONE, NULL }
	};


	//////////////////
	h_ShaderProgram_simple = LoadShaders(shader_info_simple);
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram_simple, "u_primitive_color");
	loc_ModelViewProjectionMatrix_simple = glGetUniformLocation(h_ShaderProgram_simple, "u_ModelViewProjectionMatrix");

	//////////////////
	h_ShaderProgram_TXPS = LoadShaders(shader_info_TXPS);
	loc_ModelViewProjectionMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewProjectionMatrix");
	loc_ModelViewMatrix_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrix");
	loc_ModelViewMatrixInvTrans_TXPS = glGetUniformLocation(h_ShaderProgram_TXPS, "u_ModelViewMatrixInvTrans");

	loc_base_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_base_texture");
	loc_original_texture = glGetUniformLocation(h_ShaderProgram_TXPS, "u_original_texture");
	loc_flag_texture_mapping = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_mapping");
	loc_flag_texture_diffrence = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_diffrence");
	loc_flag_texture_reverse = glGetUniformLocation(h_ShaderProgram_TXPS, "u_flag_texture_reverse");
}

void initialize_flags(void) {
	flag.texture_mapping = 1;
	flag.texture_diffrence = 0;

	glUseProgram(h_ShaderProgram_TXPS);
	glUniform1i(loc_flag_texture_mapping, flag.texture_mapping);
	glUniform1i(loc_flag_texture_diffrence, flag.texture_diffrence);
	glUseProgram(0);
}

void initialize_OpenGL(void) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	campos = glm::vec3(0.0f, 0.0f, 0.0f);

#define CAMERA_COORDINATE 000.0f
	ViewMatrix = glm::lookAt(glm::vec3(CAMERA_COORDINATE, CAMERA_COORDINATE, CAMERA_COORDINATE - IMGSIZE*1.5f), glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	//initialize_lights_and_material();
	initialize_flags();

	glGenTextures(N_NORMAL_TEXTURES_USED, texture_names);
}


void prepare_scene(void) {
	prepare_axes();
	prepare_quad();
	prepare_texture_original("Data/4kimg.jpg", TEXTURE_INDEX_ORIGINAL);
	prepare_texture_original("Data/grass_tex.jpg", TEXTURE_INDEX_TEST);
	//loadDDS("Data/4kimg_JPG_DXT3_7.DDS");
	upload_TEST_Texture_Original();
	upload_TEST_Texture_DDS(1);
	upload_TEST_Texture_DDS(3);
	upload_TEST_Texture_DDS(5);

	//loadDDSs("Data/4kimg_JPG_DXT3_7.DDS");

	//compress_test("Data/4kimg.jpg","test.img");
	//oadTexture("test.img");
	//prepare_texture_ASTC();
	//loadDDS("Data/4kimg_JPG_DXT3_4.DDS");
	//texnum=texture_loadDDS("Data/4kimg_JPG_DXT5_3.DDS");
	//texnum = loadDDSs("Data/uvtemplate - 복사본.DDS");

	upload_TEST_Texture_ASTC(4);
	//upload_TEST_Texture_ASTC(5);
	//upload_TEST_Texture_ASTC(6);
	//upload_TEST_Texture_ASTC(8);
	//upload_TEST_Texture_ASTC(10);
	upload_TEST_Texture_ASTC(12);
	//upload_TEST_Texture_ASTC(112);

	compare_PSNR();
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
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
int main(int argc, char *argv[]) {
	// Phong Shading
	char program_name[64] = "Texture compress 에 따른 비교";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used: '0', '1', 't', 'f', 's', 'd', 'r', 'l', 'ESC'" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	// glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize((int)IMGSIZE*2, (int)IMGSIZE);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();

	return 0;
}