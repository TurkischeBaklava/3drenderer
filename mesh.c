#include "mesh.h"
#include "array.h"
#include <stdio.h>
#include "objloader.h"


mesh_t mesh = {
	.vertices = (void*)0,
	.faces = (void*)0,
	.rotation = {0,0,0},
	.scale = {1.0, 1.0, 1.0},
	.translation = {0,0,0}
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1}, //1
	{.x = -1, .y =  1, .z = -1}, //2
	{.x =  1, .y =  1, .z = -1}, //3
	{.x =  1, .y = -1, .z = -1}, //4
	{.x =  1, .y =  1, .z =  1}, //5
	{.x =  1, .y = -1, .z =  1}, //6
	{.x = -1, .y =  1, .z =  1}, //7
	{.x = -1, .y = -1, .z =  1}, //8
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{.a = 1, .b = 2, .c = 3, .a_uv = {0,1}, .b_uv = {0,0}, .c_uv = {1,0}, .color = 0xffff0000},
	{.a = 1, .b = 3, .c = 4, .a_uv = {0,1}, .b_uv = {1,0}, .c_uv = {1,1}, .color = 0xffff0000},
	// right
	{.a = 4, .b = 3, .c = 5, .a_uv = {0,1}, .b_uv = {0,0}, .c_uv = {1,0}, .color = 0xffff0000},
	{.a = 4, .b = 5, .c = 6, .a_uv = {0,1}, .b_uv = {1,0}, .c_uv = {1,1}, .color = 0xffff0000},
	// back
	{.a = 6, .b = 5, .c = 7, .a_uv = {0,1}, .b_uv = {0,0}, .c_uv = {1,0}, .color = 0xffff0000},
	{.a = 6, .b = 7, .c = 8, .a_uv = {0,1}, .b_uv = {1,0}, .c_uv = {1,1}, .color = 0xffff0000},
	// left
	{.a = 8, .b = 7, .c = 2, .a_uv = {0,1}, .b_uv = {0,0}, .c_uv = {1,0}, .color = 0xffff0000},
	{.a = 8, .b = 2, .c = 1, .a_uv = {0,1}, .b_uv = {1,0}, .c_uv = {1,1}, .color = 0xffff0000},
	// top
	{.a = 2, .b = 7, .c = 5, .a_uv = {0,1}, .b_uv = {0,0}, .c_uv = {1,0}, .color = 0xffff0000},
	{.a = 2, .b = 5, .c = 3, .a_uv = {0,1}, .b_uv = {1,0}, .c_uv = {1,1}, .color = 0xffff0000},
	// bottom
	{.a = 6, .b = 8, .c = 1, .a_uv = {0,1}, .b_uv = {0,0}, .c_uv = {1,0}, .color = 0xffff0000},
	{.a = 6, .b = 1, .c = 4, .a_uv = {0,1}, .b_uv = {1,0}, .c_uv = {1,1}, .color = 0xffff0000},
};


void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++)
	{
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}
	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

void load_obj_file_data(char* filename) {
	load_obj(filename);
}