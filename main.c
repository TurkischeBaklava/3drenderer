#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_timer.h>
#include "array.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "upng.h"
#include "matrix.h"
#include "light.h"


triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { 0,0,0 };
mat4_t perspective_projection_matrix;

bool is_running = false;
int vertex_size = 1;
uint32_t previous_frame_time = 0;

void bubble_sort(triangle_t*, int);

/*
	@brief Initialize everything before the game loop starts.
*/
void setup(void) {

	render_method = RENDER_FILL_TRIANGLE;
	cull_method = CULL_BACKFACE;
	light_method = FLAT_SHADING;


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
		SDL_PIXELFORMAT_RGBA32,//SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	// Initialize the perspective projection matrix
	float fov = 60.f; // radians: the same as 180/3, or 60 deg
	float aspect = (float)window_height / (float)window_width;
	float znear = 0.1f;
	float zfar = 100.0f;
	perspective_projection_matrix = mat4_make_perspective((M_PI / 180) * fov, aspect, znear, zfar);

	// Manually load the hardcoded texture data from the static array
	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;

	// Loads the cube values in the mesh data structure
	load_cube_mesh_data();
	//load_obj_file_data("./Resource/cube.obj");

	load_png_texture_data("./Resource/yoda.png");
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
		}
		else if (event.key.keysym.sym == SDLK_1)
		{
			// displays the wireframe and a small red dot for each triangle vertex
			render_method = RENDER_WIRE_VERTEX;
		}
		else if (event.key.keysym.sym == SDLK_2)
		{
			// displays only the wireframe lines
			render_method = RENDER_WIRE;
		}
		else if (event.key.keysym.sym == SDLK_3)
		{
			// displays filled triangles with a solid color
			render_method = RENDER_FILL_TRIANGLE;
		}
		else if (event.key.keysym.sym == SDLK_4)
		{
			// displays both filled triangles and wireframe lines
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		}
		else if (event.key.keysym.sym == SDLK_5)
		{
			render_method = RENDER_TEXTURED;
		}
		else if (event.key.keysym.sym == SDLK_6)
		{
			render_method = RENDER_TEXTURED_WIRE;
		}
		else if (event.key.keysym.sym == SDLK_c)
		{
			cull_method = CULL_BACKFACE;
		}
		else if (event.key.keysym.sym == SDLK_d)
		{
			cull_method = CULL_NONE;
		}
		else if (event.key.keysym.sym == SDLK_l)
		{
			light_method = light_method == FLAT_SHADING ? LIGHT_NONE : FLAT_SHADING;
		}
		break;

	default:
		break;
	}

}



void update(void) {


	previous_frame_time = SDL_GetTicks();

	int time_to_wait = (previous_frame_time + FRAME_TARGET_TIME) - SDL_GetTicks();
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		//Sleep for giving time
		SDL_Delay(time_to_wait);
	}

	
	triangles_to_render = NULL;


	
	// Apply transformations

	mesh.rotation.x += 0.008;
	mesh.rotation.y += 0.008;
	mesh.rotation.z += 0.010;

	/*mesh.scale.x += 0.002;
	mesh.scale.y += 0.001; */

	//mesh.translation.x += 0.01;
	mesh.translation.z = 5.0;

	// Create a scale matrix, rotation and translation that will be used to multiply the mesh vertices
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);


	//Loop all triangle faces
	int num_faces = array_length(mesh.faces);

	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		// Transformed position of face_vertices
		vec4_t transformed_vertices[3];

		for (int j = 0; j < 3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);


			// Create a World Matrix combining scale, rotation and translation matrices
			// Order matters: First scale, then rotate, then translate.
			// [T]*[R]*[S]*v
			mat4_t world_matrix = mat4_identity();
			world_matrix = mat4_mul_mat4(scale_matrix,world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x,world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y,world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z,world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix,world_matrix);


			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			transformed_vertices[j] = transformed_vertex;
		}



		vec3_t face_normal;

		if (cull_method == CULL_BACKFACE || light_method == FLAT_SHADING)
		{
			// Calculate the face normal
			face_normal = calc_face_normal(transformed_vertices);
		}

		// Apply lightning here
		if (light_method == FLAT_SHADING)
		{
			// Negate the light ray
			vec3_t light_ray = g_light.direction;
			vec3_normalize(&light_ray); //Normalizing the vector as we don't want brightness to explode

			float light_intensity_factor = -vec3_dot(light_ray, face_normal);

			mesh_face.color = light_apply_intensity(mesh_face.color, light_intensity_factor);
		}

		// Enable/disable back-face culling
		if (cull_method == CULL_BACKFACE)
		{
			// Find the vector between a point in the triangle and the camera origin
			vec3_t camera_ray = vec3_sub(camera_position, vec3_from_vec4(transformed_vertices[0]));

			// Calcute how aligned the camera ray is with the face normal
			float dot_normal_camera = vec3_dot(camera_ray, face_normal);

			// Bypass the triangles that are looking away from the camera
			if (dot_normal_camera < 0) continue;
		}


	
	
		//Triangle stores the actual projected point 
		//triangle_t projected_triangle;

		vec4_t projected_points[3];
		for (int j = 0; j < 3; j++) {

			//Project the current vertex
			//projected_points[j] = project(vec3_from_vec4(transformed_vertices[j])); We are not using the project function anymore

			// projection with matrix mult
			projected_points[j] = mat4_mult_vec4_project(perspective_projection_matrix, transformed_vertices[j]);

			//Scale
			projected_points[j].x *= window_width / 2.0f;
			projected_points[j].y *= window_height / 2.0f;

			// Invert the y value, because our y-axis in screen goes down
			projected_points[j].y *= -1;
			projected_points[j].x *= -1;

			//Translate 
			projected_points[j].x += (window_width / 2.0f);
			projected_points[j].y += (window_height / 2.0f);
		}

		// Calculate the average depth for each face based on the vertices after transformation
		// we are simply assuming each face has average depth
		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0f;


		triangle_t projected_triangle = {
			.points = {
				{projected_points[0].x,projected_points[0].y, projected_points[0].z, projected_points[0].w},
				{projected_points[1].x,projected_points[1].y, projected_points[1].z, projected_points[1].w},
				{projected_points[2].x,projected_points[2].y, projected_points[2].z, projected_points[2].w}
			},
			.texcoords = {
				{mesh_face.a_uv.u, mesh_face.a_uv.v},
				{mesh_face.b_uv.u, mesh_face.b_uv.v},
				{mesh_face.c_uv.u, mesh_face.c_uv.v}
			},
			.color = mesh_face.color,
			.avg_depth = avg_depth
		};

		array_push(triangles_to_render, projected_triangle);
	}



	// Sort the triangles to render by their avg_depth (buble sort)
	int arr_length = array_length(triangles_to_render);
	bubble_sort(triangles_to_render, arr_length);

}

void render(void) {

	//Do operations on colour buffer before rendering as a SDL texture
	//draw_grid();

	// Set vertex size here for RENDER_WIRE_VERTEX option
	vertex_size = render_method == RENDER_WIRE_VERTEX ? 4 : 1;
	


	///Loop all projected triangles and render here
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++) {

		triangle_t triangle = triangles_to_render[i];

		//Draw a rectangle for each vertices
		for (int j = 0; j < 3; j++)
		{
			draw_rect(
				triangle.points[j].x,
				triangle.points[j].y,
				vertex_size,
				vertex_size,
				0xFFFF0000
				);
		}

		// Fill inside the faces/triangles
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
		{
			draw_filled_triangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, triangle.color);
		}

		// Draw texture 
		if (render_method == RENDER_TEXTURED || render_method == RENDER_TEXTURED_WIRE)
		{
			draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,
				mesh_texture
			);

		}

		// Render the lines of triangle, render lines
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_TEXTURED_WIRE)
		{
			draw_triangle(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, 0xFF000000);
		}

	}



	//Clear the array of triangles to render every frame loop
	array_free(triangles_to_render);


	//Render here
	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	free(colour_buffer);
	upng_free(png_texture);
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

	//int arr_length = array_length(triangles_to_render); ???

	destroy_window();
	free_resources();

	return 0;
}

void swap_array(triangle_t* t1,triangle_t* t2) {
	triangle_t temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}

void bubble_sort(triangle_t* t_arr, int n) {

	bool swapped;
	for (int i = 0; i < n-1; i++)
	{
		swapped = false;
		for (int j = 0; j < n-i-1; j++)
		{
			if (t_arr[j].avg_depth < t_arr[j+1].avg_depth)
			{
				swap_array(&t_arr[j], &t_arr[j + 1]);
				swapped = true;
			}
		}

		if (swapped == false) break;
	}

}
