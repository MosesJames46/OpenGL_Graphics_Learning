#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colors;
layout (location = 2) in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 object_position;

out vec3 normal_outputs;
out vec3 fragment_positions;

void main(){
	gl_Position = projection * view * model * vec4(position + object_position, 1.0f);
	normal_outputs = normals;
	fragment_positions = position;
}