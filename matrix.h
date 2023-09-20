#pragma once

#include "vector.h"

typedef struct {
	float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);


vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t m1, mat4_t m2);

// Matrix scaling 
mat4_t mat4_make_scale(float sx, float sy, float sz);

// Matrix translation
mat4_t mat4_make_translation(float tx, float ty, float tz);

// Matrix rotation
mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);

// Perspective projection matrix 
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);

vec4_t mat4_mult_vec4_project(mat4_t project_matrix, vec4_t v);