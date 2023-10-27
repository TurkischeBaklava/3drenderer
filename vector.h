#pragma once

typedef struct {
	float x, y;
} vec2_t;

typedef struct {
	float x, y, z;
} vec3_t;

typedef struct {
	float x, y, z, w;
} vec4_t;

//Vector 2D Functions

float vec2_length(vec2_t v);

vec2_t vec2_add(vec2_t v1, vec2_t v2);

vec2_t vec2_sub(vec2_t v1, vec2_t v2);

vec2_t vec2_mul(vec2_t v, float factor);

vec2_t vec2_div(vec2_t v, float factor);

float vec2_dot(vec2_t v1, vec2_t v2);

void vec2_normalize(vec2_t* v);

vec2_t vec2_from_vec4(vec4_t v);
 
//Vector 3D Functions

float vec3_length(vec3_t v);

vec3_t vec3_add(vec3_t v1, vec3_t v2);

vec3_t vec3_sub(vec3_t v1, vec3_t v2);

vec3_t vec3_mul(vec3_t v, float factor);

vec3_t vec3_div(vec3_t v, float factor);

vec3_t vec3_cross(vec3_t v1, vec3_t v2);

vec3_t negate(vec3_t v);

float vec3_dot(vec3_t v1, vec3_t v2);

void vec3_normalize(vec3_t* v);

//Rotate around X axis
vec3_t vec3_rotate_x(vec3_t v, float angle);

//Rotate around Y axis
vec3_t vec3_rotate_y(vec3_t v, float angle);

//Rotate around Z axis
vec3_t vec3_rotate_z(vec3_t v, float angle);


//Vector conversion Functions

vec4_t vec4_from_vec3(vec3_t v);

vec3_t vec3_from_vec4(vec4_t v);

//Find face normal

vec3_t calc_face_normal(vec4_t* vertices);


//Barycentric_weights
 
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

