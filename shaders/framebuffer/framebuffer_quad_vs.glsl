#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coordinate;

out vec2 fragment_texture_coordinate;

void main(){
	gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);

	fragment_texture_coordinate = texture_coordinate;
}