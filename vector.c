#include <math.h>
#include "vector.h"

//VECTOR 2 OPERATIONS

float vec2_length(vec2_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t v1, vec2_t v2) {
	vec2_t result = {
		.x = v1.x + v2.x, 
		.y = v1.y + v2.y };
	return result;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2) {
	vec2_t result = { 
		.x = v1.x - v2.x, 
		.y = v1.y - v2.y };
	return result;
}

vec2_t vec2_mul(vec2_t v, float factor) {
	vec2_t result = { 
		.x = v.x * factor, 
		.y = v.y * factor };
	return result;
}

vec2_t vec2_div(vec2_t v, float factor) {
	vec2_t result = { 
		.x = v.x / factor, 
		.y = v.y / factor };
	return result;
}

float vec2_dot(vec2_t v1, vec2_t v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

void vec2_normalize(vec2_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y);
	
	// Same thing
	v->x = v->x / length;
	v->y /= length;
}


//VECTOR 3 OPERATIONS


/// <summary>
/// Calculates the length of a vector 3.
/// </summary>
/// <param name="v">: Vector to find the length of. </param>
/// <returns> Vector length in float. </returns>
float vec3_length(vec3_t v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t v1, vec3_t v2) {
	vec3_t result = { 
		.x = v1.x + v2.x, 
		.y = v1.y + v2.y, 
		.z = v1.z + v2.z};
	return result;
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2) {
	vec3_t result = { 
		.x = v1.x - v2.x, 
		.y = v1.y - v2.y, 
		.z = v1.z - v2.z };
	return result;
}

vec3_t vec3_mul(vec3_t v, float factor) {
	vec3_t result = { 
		.x = v.x * factor, 
		.y = v.y * factor, 
		.z = v.z * factor };
	return result;
}

vec3_t vec3_div(vec3_t v, float factor) {
	vec3_t result = { 
		.x = v.x / factor, 
		.y = v.y / factor, 
		.z = v.z / factor };
	return result;
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2) {
	vec3_t result = {
		.x = v1.y* v2.z - v1.z* v2.y,
		.y = v1.z* v2.x - v1.x* v2.z,
		.z = v1.x* v2.y - v1.y* v2.x 
	};
	return result;
}

vec3_t negate(vec3_t v)
{
	vec3_t negated_v = {
		.x = -v.x,
		.y = -v.y,
		.z = -v.z
	};
	return negated_v;
}

float vec3_dot(vec3_t v1, vec3_t v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

void vec3_normalize(vec3_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);

	// Same thing
	v->x = v->x / length;
	v->y /= length;
	v->z /= length;
}


vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
	return rotated_vector;
}


vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

// Vector conversion functions

vec4_t vec4_from_vec3(vec3_t v)
{
	vec4_t result = {.x= v.x, .y= v.y, .z= v.z, .w= 1};
	return result;
}

vec3_t vec3_from_vec4(vec4_t v)
{
	vec3_t result = { .x = v.x, .y = v.y, .z = v.z };
	return result;
}




vec3_t calc_face_normal(vec4_t* vertices)
{
	//TODO: Back face culling
	vec3_t vector_a = vec3_from_vec4(vertices[0]); /*  A  */
	vec3_t vector_b = vec3_from_vec4(vertices[1]); /* / \ */
	vec3_t vector_c = vec3_from_vec4(vertices[2]); /*C---B*/

	// Get the vector substraction of B-A and C-A
	vec3_t vector_ab = vec3_sub(vector_b, vector_a);
	vec3_t vector_ac = vec3_sub(vector_c, vector_a);
	vec3_normalize(&vector_ab);
	vec3_normalize(&vector_ac);

	// Compute the face normal (using cross product to find perpendicular)
	vec3_t normal = vec3_cross(vector_ab, vector_ac);
	
	// Normalize the face normal vector
	vec3_normalize(&normal);

	return normal;
}
