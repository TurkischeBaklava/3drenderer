#pragma once

#include <stdint.h>  // This library includes the fixed int types we need: uint16_t, uint32_t etc..
#include <stdbool.h> //Added this header file. Beacuse C lang does not have bool type originally. Instead we could use return int type and 1(true) or 0(false).
#include <SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000/FPS)


// Variable declarations
extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* colour_buffer;
extern SDL_Texture* colour_buffer_texture;

extern int window_width;
extern int window_height;



// Function prototypes

/*
	@brief Initialize SDL window
	@param void : We should use void as an argument when we have no args.
	@return  Can return bool as we have added the stdbool.h. Return true if everything is ok.
*/
bool initialize_window(void);

/*
	@brief Update SDL texture with colour buffer and copy to current rendering target.
*/
void render_color_buffer();

/*
* @brief Clear screen by painting every pixel(full screen) with given color.
*/
void clear_color_buffer(uint32_t color);

void destroy_window(void);

/*
	@brief Draw a pixel at given position on the screen in color.

	@param x X position
	@param y Y position
	@param color Color of the pixel
*/
void draw_pixel(int x, int y, uint32_t color);

/*
	@brief Draws a rectangle at given coordinates for the height and width in color

	@param x  X pos
	@param y  Y pos
	@param width Width of the rectangle
	@param height Width of the height
	@param color Which color to paint
*/
void draw_rect(int x, int y, int width, int height, uint32_t color);

/*
* @brief Draws a background grid that fills the entire window.
*/
void draw_grid(void);

/*
* @brief Draws a line between two coordinate points
* 
* @param Coordinates of the start and end points. 
*/
void draw_line(int x0, int y0, int x1, int y1, uint32_t Color);

/*
* @brief Draws a triangle between 3 points on the coordinate system.
*/
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t Color);