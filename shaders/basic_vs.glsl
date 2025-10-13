#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coordinate;
layout (location = 3) in vec3 normal;

out vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 translation;
uniform mat4 scale;
uniform mat4 rotation;

uniform float scalar;
void main(){
	normals = normal;
	//By implementing the translation matrix we removed the bug of our stencil highlighting algorithim not working.
	gl_Position = projection * view * model * translation * rotation * scale * vec4((position * scalar) * 1.02 , 1.0);
}