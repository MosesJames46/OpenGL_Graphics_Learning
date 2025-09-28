#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 texture_coordinate;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 object_position;

out vec3 fragment_normal;
out vec3 fragment_position;

void main(){
	gl_Position = projection * view * model * vec4(position + object_position, 1.0f);
	fragment_normal = mat3(transpose(inverse(model))) * normal;;
	fragment_position = position + object_position;
}

