#version 330 core

out vec4 fragment_colors;


struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};
uniform Light light;
in vec3 normal_outputs;
in vec3 fragment_positions;


uniform vec3 view_position;


void main(){
	vec3 norm = normalize(normal_outputs);

	vec3 ambient = material.ambient * light.ambient;

	vec3 light_direction = normalize(light.position - fragment_positions);

	float diffuse = max(dot(norm, light_direction), 0.0f);
	vec3 diff = diffuse * material.diffuse * light.diffuse;

	vec3 view_direction = normalize(view_position - fragment_positions);
	vec3 reflect_direction = reflect(-light_direction, norm);  

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular = material.specular * spec * light.specular; 

	vec3 result = ambient + diff + specular;

	fragment_colors = vec4(result, 1.0f);
}