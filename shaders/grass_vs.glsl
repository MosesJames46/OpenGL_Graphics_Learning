#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 translation;

out vec2 texture_coordinates;

uniform float uniform_scale;
void main(){
	gl_Position = projection * view * model * translation * rotation * scale * vec4(position * uniform_scale, 1.0f);

	texture_coordinates = texture_coordinate;
}
