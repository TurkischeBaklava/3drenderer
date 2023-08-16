#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_timer.h>
#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS]; // 9x9x9 cube
vec2_t projected_points[N_POINTS];

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

	int point_count = 0;

	for (float x = -1; x <= 1; x += 0.25)
	{
		for (float y = -1; y <= 1; y += 0.25)
		{
			for (float z = -1; z <= 1 ; z += 0.25)
			{
				vec3_t new_point =  { .x = x, .y = y, .z = z };
				cube_points[point_count++] = new_point;
			}
		}
	}


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
	/*while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));*/

	previous_frame_time = SDL_GetTicks();

	int time_to_wait = (previous_frame_time + FRAME_TARGET_TIME) - SDL_GetTicks();
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		//Sleep for giving time
		SDL_Delay(time_to_wait);
	}


	
	////Camera position animated
	/*camera_position.z += 0.005;
	if (camera_position.z >= 5.0)camera_position.z = -5;*/

	cube_rotation.z += 0.008;
	cube_rotation.y += 0.008;
	cube_rotation.x += 0.008;


	for (int i = 0; i < N_POINTS; i++)
	{
		vec3_t point = cube_points[i];

		point = vec3_rotate_x(point, cube_rotation.x);
		point = vec3_rotate_y(point, cube_rotation.y);
		point = vec3_rotate_z(point, cube_rotation.z);

		point.z -= camera_position.z;


		//Project vec3_t cube points and save them into array
		projected_points[i] = project(point);
	
	}


}

void render(void) {

	//Do operations on colour buffer before rendering as a SDL texture
	draw_grid();

	//Loop all projected points and render here
	for (int i = 0; i < N_POINTS; i++) {
		draw_rect(
			projected_points[i].x + (window_width / 2),
			projected_points[i].y + (window_height / 2),
			4, 
			4, 
			0xffff00ff
		);
	}


	//Render here
	render_color_buffer();
	clear_color_buffer(0xFF333333);


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