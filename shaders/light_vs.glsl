#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 texture_coordinate;
layout (location = 3) in vec3 normal;

uniform float scalar;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 scale;
uniform mat4 translation;
uniform mat4 rotation;

out vec3 normals;


//Take note of the order that we perform transformations. 
void main(){
	normals = vec3(rotation * vec4(normal, 0.0f));
	gl_Position = projection * view * model * translation * rotation * scale * vec4(position * scalar, 1.0f);
}
