#version 330 core

out vec4 fragment_colors;

uniform vec3 color;

void main(){
	fragment_colors = vec4(color, 1.0f);
}