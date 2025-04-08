#version 330 core

out vec4 fragments;
uniform vec3 fragment_color;

//in vec2 texture_one_output;


//uniform sampler2D texture_one;
//uniform sampler2D texture_two;


void main(){
	fragments = vec4(fragment_color, 1.0f);
}