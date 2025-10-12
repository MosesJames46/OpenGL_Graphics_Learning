#version 330 core

struct Material{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light{
	vec3 position;

	/*
		Light simulation using the Phong Lighting Model.
	*/
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	//Variables used for calculating attenuation.
	float constant;
	float quadratic;
	float linear;
	
};

uniform Light light;

out vec4 fragment_colors;

in vec3 fragment_positions;
in vec3 normal_output;

uniform vec3 view_position;

void main(){
	//Ambient
	vec3 ambient = light.ambient * material.ambient;

	//Diffuse
	vec3 light_direction = normalize(fragment_positions - light.position);
	vec3 norm = normalize(normal_output);
	float diff = max(dot(norm, light_direction), 0.0f);
	vec3 diffuse = diff * material.diffuse * light.diffuse;

	//Specular
	vec3 view_direction = normalize(view_position - fragment_positions);
	vec3 reflect_direction = reflect(light_direction, norm);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular = material.specular * spec;

	//Attenuation
	float dist = length(fragment_positions - light.position);
	float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 result = (ambient + diffuse + specular) * attenuation;

	fragment_colors = vec4(result, 1.0f);
}