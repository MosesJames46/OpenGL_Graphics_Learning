#version 330 core

out vec4 fragment_colors;

in vec3 normal_outputs;
in vec3 fragment_positions;

uniform vec3 light_color;
uniform vec3 light_position;

uniform vec3 object_color;

void main(){
	vec3 norm = normalize(normal_outputs);
	vec3 ambient = 0.4f * light_color;
	vec3 light_direction = normalize(light_position - fragment_positions);
	float diffuse = max(dot(norm, light_direction), 0.0f);
	vec3 diff = diffuse * light_color;
	vec3 result = (ambient + diff) * object_color;
	fragment_colors = vec4(result, 1.0f);
}