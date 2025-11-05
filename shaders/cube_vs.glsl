#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

out vec3 fragment_color;
out vec3 fragment_normal;
out vec3 fragment_position;

uniform float uniform_scale;

uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 translation;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float highlight_influence;
uniform bool using_highlights;

void main(){
	vec3 post_matrix_position = position * uniform_scale;
	if (using_highlights) post_matrix_position = post_matrix_position * highlight_influence;

	gl_Position = projection * view * model * translation * rotation * scale * vec4(post_matrix_position, 1.0f);
	fragment_color = color;
	fragment_normal = mat3(transpose(inverse(model))) * mat3(rotation) * normal;
	fragment_position = position;
}