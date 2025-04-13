#version 330 core

out vec4 fragments;
uniform vec3 object_color;

void main(){
	fragments = vec4(object_color, 1.0f);
}