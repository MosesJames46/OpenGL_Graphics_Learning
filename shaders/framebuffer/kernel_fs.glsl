#version 330 core

out vec4 fragment_color;

in vec2 fragment_texture_coordinate;

uniform sampler2D fragment_texture_value;

uniform float kernel[9];

uniform float kernel_divisor = 1.f;

void main(){

	float offset_value = 1.f / 300.f;

	vec2 texture_matrix[9] = vec2[](
		vec2(-offset_value, offset_value), vec2(0, offset_value), vec2(offset_value, offset_value),
		vec2(-offset_value, 0), vec2(0, 0), vec2(offset_value, 0),
		vec2(-offset_value, -offset_value), vec2(0, -offset_value), vec2(offset_value, -offset_value)
	);

	vec3 sample_textures[9];
	for (int i = 0; i < 9; ++i){
		sample_textures[i] = vec3(texture(fragment_texture_value, fragment_texture_coordinate.st + texture_matrix[i]));
	}

	vec3 color = vec3(0.f, 0.f, 0.f);
	for (int i = 0; i < 9; ++i){
		color += sample_textures[i] * (kernel[i] / kernel_divisor);
	}

	fragment_color = vec4(color, 1.0f);
}