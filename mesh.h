#pragma once

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) //6 cube face * 2 tri angles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

/// <summary>
/// Define a struct for dynamic size meshes, with array of vertices and faces
/// </summary>
typedef struct {
	vec3_t* vertices; //dynamic array of vertices
	face_t* faces;	  //dynamic array of faces
	vec3_t	rotation; //rotation of the mesh
	vec3_t	scale;	  //scale with x,y, and z values
	vec3_t	translation; //translation with x, y, and z values
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);

void load_obj_file_data(char* filename);