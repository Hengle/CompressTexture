#pragma once
/*
�̹��� ������ �ε��Ͽ� �ؽ�óȭ�ϴ� �Լ�
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


GLuint load_unpack_image(const char *filename);
float load_unpack_image_checktime(const char *filename);

GLuint load_astc_image(const char *filename);
float load_astc_image_checktime(const char *filename);

GLuint load_dds_image(const char * imagepath);
float load_dds_image_checktime(const char * imagepath);

double getPSNR(const char* fileName1, const char* fileName2);