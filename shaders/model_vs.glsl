#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coordinate;
layout (location = 3) in vec3 normal;

out vec2 texture_coordinates;
out vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 object_position;

void main(){
	gl_Position = projection * view * model * vec4(position + object_position, 1.0f);
	texture_coordinates = texture_coordinate;
	normals = normal;
}