#version 330 core

/*
	- The directional lighting scheme takes a material object and only uses the direction of an object to do lighting 
	calculations. It does not take into consideration the objects position, just direction.

	- Currently not outputting proper color. Fix.
*/


in vec3 fragment_normal;
in vec3 fragment_position;


//Direction lights only need direction vectors because they are far enough that position is irrelevant.
struct Light{
	vec3 direction;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	//Variables used for calculating attenuation.
	float constant;
	float quadratic;
	float linear;
};

struct Material{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};

uniform Light light;
uniform Material material;

uniform vec3 view_position;

out vec4 fragment_colors;

void main(){
	/* The direction of the light will most likely be from the fragment toward the light so we negate the 
	light direction*/
	vec3 ambient = light.ambient * material.ambient;
	
	//Diffuse
	vec3 light_direction = normalize(-light.direction);
	float dist = length(fragment_position - light_direction);
	//Add attenuation later.
	float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 normals = normalize(fragment_normal);
	float diff = max(dot(normals, light_direction), 0.0f);
	vec3 diffuse = light.diffuse * material.diffuse * diff;

	//Reflection
	vec3 view_direction = normalize(view_position - fragment_position);
	vec3 reflection_direction = reflect(light_direction, normals);
	float specular_value = pow(max(dot(view_direction, reflection_direction), 0.0f), material.shininess);
	//Add specular later
	vec3 specular = material.specular * specular_value;

	vec3 result = ambient * diffuse;

	fragment_colors = vec4(result, 1.0f);
}