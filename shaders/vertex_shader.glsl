#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
//layout (location = 2) in vec2 texture_coordinate;

//out vec3 fragment_color;
//out vec2 texture_one_output;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * model * vec4(position, 1.0f);
	//fragment_color = color;
	//texture_one_output = texture_coordinate;
}
