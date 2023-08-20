#pragma once

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;


//Rotate around X axis
vec3_t vec3_rotate_x(vec3_t v, float angle);

//Rotate around Y axis
vec3_t vec3_rotate_y(vec3_t v, float angle);

//Rotate around Z axis
vec3_t vec3_rotate_z(vec3_t v, float angle);

