#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coordinate;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 object_position;
uniform float scale;
uniform mat4 scale_matrix;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec2 texture_coordinates;

void main(){
	//World space position
	vec4 world_space_position =  model * scale_matrix * vec4((position + object_position) * scale, 1.0f);;

	fragment_position = vec3( world_space_position);
	
	fragment_normal = normal;

	texture_coordinates = texture_coordinate;
	
	gl_Position = projection * view * world_space_position;
}
