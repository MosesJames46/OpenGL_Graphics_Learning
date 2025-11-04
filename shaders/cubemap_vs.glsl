#version 330 core

layout (location = 0) in vec3 position;

out vec3 texture_coordinate;

uniform mat4 view;
uniform mat4 projection;

void main(){
	vec4 position_output = projection * view * vec4(position , 1.0f);
	gl_Position = position_output.xyww;
	texture_coordinate = position;
}