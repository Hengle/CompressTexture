#pragma once
/*
이미지 파일을 로드하여 텍스처화하는 함수
*/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FreeImage/FreeImage.h>

#include "Shaders/LoadShaders.h"
#include "My_Shading.h"
#include <time.h>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.
#include <glm/gtc/matrix_inverse.hpp> // inverseTranspose, etc.


GLuint load_unpack_image(const char *filename, GLuint ReadRGBAType);
float load_unpack_image_checktime(const char *filename, GLuint ReadRGBAType);
float load_unpack_image_16bit_checktime(const char *filename, GLuint ReadRGBAType);

GLuint load_astc_image(const char *filename);
float load_astc_image_checktime(const char *filename);

GLuint load_dds_image(const char * imagepath);
float load_dds_image_checktime(const char * imagepath);

double getPSNR(const char* fileName1, const char* fileName2);
void test_loadimg(const char *filename_ori, const char *filename_upper, const char *filename_lower); 
void test_loadimg_diff(const char *filename_ori, const char *filename_comp1);
void test_loadimg_diff_bptc(const char *filename_ori, const char *filename_comp1, const char *filename_comp2);
void combine_2File(const char *filename_comp1, const char *filename_comp2);