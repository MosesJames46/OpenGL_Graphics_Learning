#version 330 core

out vec4 fragments;
uniform vec3 fragment_color;

void main(){
	fragments = vec4(fragment_color, 1.0f);
}