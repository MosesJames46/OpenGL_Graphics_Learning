#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;

uniform float scalar;

void main(){
	gl_Position = projection * view * model * translate * rotate * scale * vec4(position * scalar, 1.0f);
}