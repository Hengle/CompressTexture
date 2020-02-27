//#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"
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
GLint loc_base_texture, loc_original_texture, loc_flag_texture_mapping, loc_flag_texture_diffrence, loc_flag_fog;



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
#define N_NORMAL_TEXTURES_USED 10

#define TEXTURE_INDEX_ORIGINAL 0
#define TEXTURE_INDEX_TEST 1
#define TEXTURE_INDEX_COMPRESS_ASTC4X4 2
#define TEXTURE_INDEX_COMPRESS_ASTC5X5 3
#define TEXTURE_INDEX_COMPRESS_ASTC6X6 4
#define TEXTURE_INDEX_COMPRESS_ASTC8X8 5
#define TEXTURE_INDEX_COMPRESS_ASTC10X10 6
#define TEXTURE_INDEX_COMPRESS_ASTC12X12 7
#define TEXTURE_INDEX_COMPRESS_ASTC12X12_LDR_SRGB 8
#define TEXTURE_INDEX_COMPRESS_ASTC12X12_LDR_LINEAR 9
//#define TEXTURE_INDEX_TIGER 1
//#define TEXTURE_INDEX_SHADOW 2


GLuint texture_names[N_NORMAL_TEXTURES_USED];

char titleTex[N_NORMAL_TEXTURES_USED][30] = {
	"ORIGINAL",
	  "TEST",
	  "ASTC4X4",
	  "ASTC5X5",
	  "ASTC6X6",
	  "ASTC8X8",
	  "ASTC10X10",
	  "ASTC12X12",
	  "ASTC12X12_LDR_SRGB",
	  "ASTC12X12_LDR_LINEAR"
};


void My_glTexImage2D_from_file(const char *filename) {
	FREE_IMAGE_FORMAT tx_file_format;
	int tx_bits_per_pixel;
	FIBITMAP *tx_pixmap, *tx_pixmap_32;

	int width, height;
	GLvoid *data;

	tx_file_format = FreeImage_GetFileType(filename, 0);
	// assume everything is fine with reading texture from file: no error checking
	tx_pixmap = FreeImage_Load(tx_file_format, filename);
	tx_bits_per_pixel = FreeImage_GetBPP(tx_pixmap);

	fprintf(stdout, " * A %d-bit texture was read from %s.\n", tx_bits_per_pixel, filename);
	if (tx_bits_per_pixel == 32)
		tx_pixmap_32 = tx_pixmap;
	else {
		fprintf(stdout, " * Converting texture from %d bits to 32 bits...\n", tx_bits_per_pixel);
		tx_pixmap_32 = FreeImage_ConvertTo32Bits(tx_pixmap);
	}

	width = FreeImage_GetWidth(tx_pixmap_32);
	height = FreeImage_GetHeight(tx_pixmap_32);
	data = FreeImage_GetBits(tx_pixmap_32);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	fprintf(stdout, " * Loaded %dx%d RGBA texture into graphics memory.\n\n", width, height);

	FreeImage_Unload(tx_pixmap_32);
	if (tx_bits_per_pixel != 32)
		FreeImage_Unload(tx_pixmap);
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

void load_astc_file(const char *filename)
{
	int x, y, z;
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

	uint8_t *buffer = (uint8_t *)malloc(xblocks * yblocks * zblocks * 16);
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
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_12x12_KHR, xsize, ysize, 0, xblocks * yblocks * zblocks * 16, buffer);
	}
	else if (xdim == 10 && ydim == 10) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_10x10_KHR, xsize, ysize, 0, xblocks * yblocks * zblocks * 16, buffer);
	}
	else if (xdim == 8 && ydim == 8) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_8x8_KHR, xsize, ysize, 0, xblocks * yblocks * zblocks * 16, buffer);
	}
	else if (xdim == 6 && ydim == 6) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_6x6_KHR, xsize, ysize, 0, xblocks * yblocks * zblocks * 16, buffer);
	}
	else if (xdim == 5 && ydim == 5) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_5x5_KHR, xsize, ysize, 0, xblocks * yblocks * zblocks * 16, buffer);
	}
	else if (xdim == 4 && ydim == 4) {
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_ASTC_4x4_KHR, xsize, ysize, 0, xblocks * yblocks * zblocks * 16, buffer);
	}




	free(buffer);

}

//GLuint loadDDS(const char * imagepath) {
//
//	unsigned char header[124];
//
//	FILE *fp;
//
//	/* 파일 열기 시도 */
//	fp = fopen(imagepath, "rb");
//	if (fp == NULL)
//		return 0;
//
//	/* 파일 타입 체크 */
//	char filecode[4];
//	fread(filecode, 1, 4, fp);
//	if (strncmp(filecode, "DDS ", 4) != 0) {
//		fclose(fp);
//		return 0;
//	}
//
//	/* 이미지의 정보를 긁어옵니다.  */
//	fread(&header, 124, 1, fp);
//
//	unsigned int height = *(unsigned int*)&(header[8]);
//	unsigned int width = *(unsigned int*)&(header[12]);
//	unsigned int linearSize = *(unsigned int*)&(header[16]);
//	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
//	unsigned int fourCC = *(unsigned int*)&(header[80]);
//
//	unsigned char * buffer;
//	unsigned int bufsize;
//	/* 모든 밉맵을 포함하면 얼마나 크나요? */
//	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
//	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
//	fread(buffer, 1, bufsize, fp);
//	/* 파일 포인터 닫기  */
//	fclose(fp);
//
//	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
//	unsigned int format;
//	switch (fourCC)
//	{
//	case FOURCC_DXT1:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
//		break;
//	case FOURCC_DXT3:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
//		break;
//	case FOURCC_DXT5:
//		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
//		break;
//	default:
//		free(buffer);
//		return 0;
//	}
//
//	// Create one OpenGL texture
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//
//	// 새롭게 생성된 텍스처를 "Bind"합니다. : 이제 앞으로 모든 Texutre 관련 함수는 이 친구를 건듭니다. 
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
//	unsigned int offset = 0;
//
//	/* load the mipmaps */
//	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
//	{
//		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
//		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
//			0, size, buffer + offset);
//
//		offset += size;
//		width /= 2;
//		height /= 2;
//	}
//	free(buffer);
//
//	return textureID;
//
//}

// for tiger animation
//int cur_frame_tiger = 0;
//float rotation_angle_tiger = 0.0f;
//int rotation_speed_tiger = 100;

#include "Objects.h"

#define IMGSIZE 1024.0f

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

	//set_material_floor();
	glUniform1i(loc_base_texture, TEXTURE_INDEX_ORIGINAL);
	glUniform1i(loc_original_texture, TEXTURE_INDEX_TEST);

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

GLuint texnum = 2;

void draw_compressed_texture() {
	glm::mat4 ModelMatrix;



	glUseProgram(h_ShaderProgram_TXPS);
	//glUniform1i(loc_shadow_texture, ShadowMapping.texture_unit);

	//set_material_floor();
	glUniform1i(loc_base_texture, texnum);
	glUniform1i(loc_original_texture, TEXTURE_INDEX_ORIGINAL);
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

// callbacks
void display(void) {


	glClearColor(1, 0, 0, 0);
	glViewport(0, 0, WINDOW_param.width, WINDOW_param.height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_original_texture();
	draw_compressed_texture();

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
	int dist = 50.0f;
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
	case 'm':
		//printf("log, texnum : %d\n",texnum);
		texnum++;
		if (texnum >= N_NORMAL_TEXTURES_USED) {
			texnum = TEXTURE_INDEX_COMPRESS_ASTC4X4;
		}
		glutSetWindowTitle(titleTex[texnum]);
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
	prepare_texture_original();
	prepare_texture_test();
	prepare_texture_ASTC();
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
	glutInitWindowSize(IMGSIZE*2, IMGSIZE);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();

	return 0;
}