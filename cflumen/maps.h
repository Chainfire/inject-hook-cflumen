/* Copyright (c) 2015, Jorrit 'Chainfire' Jongma
   See LICENSE file for details */

#ifndef __MAPS_H
#define __MAPS_H

#include <GLES2/gl2.h>

void mapsInit();

void addShader(GLuint key, GLuint value);
GLint getShader(GLuint key);

void addProgram(GLuint key, GLuint value);
GLint getProgram(GLuint key);

void addUniformLocation(GLuint program, GLint key, GLint value);
GLint getUniformLocation(GLuint program, GLint key);

#endif
