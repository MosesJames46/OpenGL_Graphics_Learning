#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 scale;
uniform mat4 translate;

out vec3 fragment_positions;
out vec3 normal_output;

uniform float scalar;


void main(){
	gl_Position = projection * view * model * translate * scale * vec4(position * scalar, 1.0f);
	normal_output = mat3(transpose(inverse(model))) * normal;
	fragment_positions = vec3(model * vec4(position + object_position, 1.0f));
}