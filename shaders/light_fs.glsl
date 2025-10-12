#version 330 core

out vec4 fragments;
uniform vec3 light_color;
in vec3 normals;
void main(){
	fragments = vec4(normalize(normals), 1.0f);
}