#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_timer.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"


triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { 0,0,0 };
float fov_factor = 640;

bool is_running = false;
uint32_t previous_frame_time = 0;

/*
	@brief Initialize everything before the game loop starts.
*/
void setup(void) {

	// Allocate the required memory in bytes to hold the color buffer
	colour_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	
	// Check if the memory allocation operation for given size and type was successul. Otherwise malloc will return NULL ptr.
	if (!colour_buffer) {
		fprintf(stderr, "Error when allocating memory for color buffer.\n");
		return;
	}
	
	// Creating a SDL texture that is used to display the color buffer
	colour_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	// Loads the cube values in the mesh data structure
	//load_cube_mesh_data();
	load_obj_file_data("./Resource/cube.obj");
} 

/*
	

*/
void process_input(void) {
	 
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:		//This event is triggered when we click on the X(close) button.
		is_running = false;
		break;

	case SDL_KEYDOWN:	//Check the pressed key
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			is_running = false;
			break;
		}

	default:
		break;
	}

}

vec2_t project(vec3_t point) {

	vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	 return projected_point;
}



void update(void) {

	//Naive way to implement delaying
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));

	previous_frame_time = SDL_GetTicks();

	int time_to_wait = (previous_frame_time + FRAME_TARGET_TIME) - SDL_GetTicks();
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		//Sleep for giving time
		SDL_Delay(time_to_wait);
	}

	// Initalize the array of triangle array
	triangles_to_render = NULL;


	////Camera position animated
	//camera_position.z += 0.005;
	/*if (camera_position.z >= 5.0)camera_position.z = -5;*/

	
	mesh.rotation.x += 0.008;
	mesh.rotation.y += 0.008;
	//mesh.rotation.z = 180;

	//Loop all triangle faces
	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec3_t transformed_vertices[3];

		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += 5;

			transformed_vertices[j] = transformed_vertex;
		}

		//TODO: Back face culling
		vec3_t vector_a = transformed_vertices[0]; /*  A  */
		vec3_t vector_b = transformed_vertices[1]; /* / \ */
		vec3_t vector_c = transformed_vertices[2]; /*C---B*/

		// Get the vector substraction of B-A and C-A
		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);
		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);

		// Compute the face normal (using cross product to find perpendicular)
		vec3_t normal = vec3_cross(vector_ab,vector_ac);

		// Normalize the face normal vector
		vec3_normalize(&normal);

		// Find the vector between a point in the triangle and the camera origin
		vec3_t camera_ray = vec3_sub(camera_position, vector_a);
		
		// Calcute how aligned the camera ray is with the face normal
		float dot_normal_camera = vec3_dot(camera_ray, normal);
		
		// Bypass the triangles that are looking away from the camera
		if (dot_normal_camera < 0) continue;


		//Triangle stores the actual projected point 
		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++) {

			//Project the current vertex
			vec2_t projected_point = project(transformed_vertices[j]);

			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
		}

		//triangles_to_render[i] = projected_triangle;
		array_push(triangles_to_render, projected_triangle);
	}

}

void render(void) {

	//Do operations on colour buffer before rendering as a SDL texture
	draw_grid();

	
	///Loop all projected triangles and render here
	int num_triangles = array_length(triangles_to_render);

	for (int i = 0; i < num_triangles; i++) {

		triangle_t triangle = triangles_to_render[i];

		for (int j = 0; j < 3; j++)
		{
			draw_rect(
				triangle.points[j].x,
				triangle.points[j].y,
				1,
				1,
				0xFFFFFF99
				);
		}

		draw_triangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xFFFF);
	}

	//Clear the array of triangles to render every frame loop
	array_free(triangles_to_render);


	//Render here
	render_color_buffer();
	clear_color_buffer(0xFF333300);

	SDL_RenderPresent(renderer);
}


// Free the memory that was dynamically allocated by the person
void free_resources(void) {
	free(colour_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
}


int main(int argc, char* args[]) {
	
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}


	destroy_window();
	free_resources();

	return 0;
}