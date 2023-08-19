#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_timer.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };



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

	////Camera position animated
	//camera_position.z += 0.005;
	/*if (camera_position.z >= 5.0)camera_position.z = -5;*/

	cube_rotation.z += 0.008;
	cube_rotation.y += 0.008;
	cube_rotation.x += 0.008;

	//Loop all triangle faces
	for (int i = 0; i < N_MESH_FACES; i++)
	{
		face_t mesh_face = mesh_faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++)
		{
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

			transformed_vertex.z -= camera_position.z;

			//Project the current vertex
			vec2_t projected_point = project(transformed_vertex);

			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
		}

		triangles_to_render[i] = projected_triangle;
		
	}





}

void render(void) {

	//Do operations on colour buffer before rendering as a SDL texture
	draw_grid();

	
	///Loop all projected triangles and render here
	for (int i = 0; i < N_MESH_FACES; i++) {
		for (int j = 0; j < 3; j++)
		{
			draw_rect(
				triangles_to_render[i].points[j].x,
				triangles_to_render[i].points[j].y,
				4,
				4,
				0xFFFFFFFF
				);
		}
	}


	//Render here
	render_color_buffer();
	clear_color_buffer(0xFF333300);


	SDL_RenderPresent(renderer);
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

	return 0;
}