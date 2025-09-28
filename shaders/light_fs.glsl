#version 330 core

out vec4 fragments;
uniform vec3 light_color;

void main(){
	fragments = vec4(light_color, 1.0f);
}