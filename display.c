#include <stdio.h>
#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* colour_buffer = NULL;
SDL_Texture* colour_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;


bool initialize_window(void) {

	//Check if not equals zero. Returning 0 means there were no errors.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {

		//Different call of printf. We can give extra arguments.
		fprintf(stderr, "Error initalizing SDL.\n");
		return false;
	}

	//Get the current display mode and assign new height and width 
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_height = display_mode.h;
	window_width = display_mode.w;


	//Create a SDL window 
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	//Create a SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer. \n");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void render_color_buffer() {

	SDL_UpdateTexture(
		colour_buffer_texture,
		NULL,
		colour_buffer,
		(int)window_width * sizeof(uint32_t)
	);

	SDL_RenderCopy(
		renderer,
		colour_buffer_texture,
		NULL,
		NULL
	);
}

void clear_color_buffer(uint32_t color) {

	for (int x = 0; x < window_width * window_height; x++)
	{
		colour_buffer[x] = color;
	}
}

void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height)
	{
		colour_buffer[(window_width * y) + x] = color;
	}

}

void destroy_window(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw_grid(void) {

	for (int y = 0; y < window_height; y += 10)
	{
		for (int x = 0; x < window_width; x += 10)
		{
			colour_buffer[(window_width * y) + x] = 0xffff00ff;
		}
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {

	for (int i = 0; i < width; i++)
	{
		int current_x = i + x;
		
		for (int j = 0; j < height; j++)
		{
			int current_y = j + y;

			draw_pixel(current_x,current_y,color);
		}
	}

}

void draw_line(int x0, int y0, int x1, int y1, uint32_t Color)
{
	//Get the x and y delta
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	//Run
	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	//increment rate for each axis, slope
	float inc_x = delta_x / (float)side_length;
	float inc_y = delta_y / (float)side_length;

	float current_x = x0;
	float current_y = y0;

	//Draw pixel
	for (int i = 0; i <= side_length; i++)
	{
		draw_pixel(
		round(current_x),
		round(current_y),
		Color);
		
		current_x += inc_x;
		current_y += inc_y;
	}

}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t Color) {
	draw_line(x0, y0, x1, y1, Color);
	draw_line(x1, y1, x2, y2, Color);
	draw_line(x2, y2, x0, y0, Color);
}
