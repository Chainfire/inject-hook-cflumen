/* Copyright (c) 2015, Jorrit 'Chainfire' Jongma
   See LICENSE file for details */

#include <GLES2/gl2.h>

#define LOG_TAG "CFLumen"
#include "ndklog.h"

#define BITS_SHADERS 9
#define MAX_SHADERS (8 << BITS_SHADERS)

#define BITS_PROGRAMS 8
#define MAX_PROGRAMS (1 << BITS_PROGRAMS)

#define BITS_UNIFORMS 4
#define BITS_PROGRAM_UNIFORMS (BITS_PROGRAMS + BITS_UNIFORMS)
#define MAX_PROGRAM_UNIFORMS (1 << BITS_PROGRAM_UNIFORMS)

GLint shaders[MAX_SHADERS];
GLint programs[MAX_PROGRAMS];
GLint uniforms[MAX_PROGRAM_UNIFORMS];

void mapsInit() {
    for (int i = 0; i < MAX_SHADERS; i++) shaders[i] = -1;
    for (int i = 0; i < MAX_PROGRAMS; i++) programs[i] = -1;
    for (int i = 0; i < MAX_PROGRAM_UNIFORMS; i++) uniforms[i] = -1;
}

GLint getShader(GLuint key) {
    return shaders[key];
}

void addShader(GLuint key, GLuint value) {
    shaders[key] = (GLint)value;
}

GLint getProgram(GLuint key) {
    return programs[key];
}

void addProgram(GLuint key, GLuint value) {
    programs[key] = (GLint)value;
}

GLint getUniformLocation(GLuint program, GLint key) {
    return uniforms[(program << BITS_UNIFORMS) + (GLuint)key];
}

void addUniformLocation(GLuint program, GLint key, GLint value) {
    uniforms[(program << BITS_UNIFORMS) + (GLuint)key] = value;
}
