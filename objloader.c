#include "objloader.h"
#include <stdio.h>
#include "array.h"


bool load_obj(char* pathname) {


	//Open file 
	FILE* file = fopen(pathname, "r");
	if (!file)
	{
		perror("fopen() failed");
		return false;
	}
	printf("Opened successfully: %s", pathname);


	//Go through all characters and find the vector points and face indices
	char buffer[255];
	//Loop untill fgets returns a NULL
	while (fgets(buffer,255,file)) //Return until find a new line or eof
	{	
		if (buffer[0] == 'v' && buffer[1] == ' ')
		{
			float vec_x, vec_y, vec_z;
			//Using * convention specification. function will assign the result of the conversion to any receiving argument
			int ret = sscanf(buffer, "%*c %f%f%f",&vec_x, &vec_y, &vec_z);
			vec3_t obj_vertex = { .x= vec_x, .y= vec_y, .z= vec_z};
			array_push(mesh.vertices, obj_vertex);
		}	

		if (buffer[0] == 'f' && buffer[1] == ' ')
		{
			int a, b, c;
			int ret = sscanf(buffer, "%*c %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &a, &b, &c);
			face_t obj_face = {.a = a, .b = b, .c = c, .color = 0xffffffff};
			array_push(mesh.faces, obj_face)
		}
	}

	//Print error and eof logs
	if (ferror(file))
	{
		perror("I / O error when reading");
	}
	else if (feof(file))
	{
		puts("End of file reached successfully");
	}

	//Close file before return
	fclose(file);
	return true;
}
