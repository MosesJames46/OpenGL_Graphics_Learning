#version 330 core

out vec4 fragments;
in vec3 fragment_color;

in vec2 texture_one_output;


uniform sampler2D texture_one;
uniform sampler2D texture_two;


void main(){
	fragments = mix(texture(texture_one, texture_one_output), texture(texture_two, texture_one_output), 0.5f);
}