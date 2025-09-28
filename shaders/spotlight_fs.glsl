#version 330 core
out vec4 fragment_color;
/*
	This shader is to perform calculations of a spotlight similar to what a street light would look like on 
	the road.

	There are four necessary components to calculating the spotlight:
		1. Direction of the light direction from the fragment to the light source.
		2. The direction the spotlight is facing.
		3. The cutoff angle that determines the radius of the spotlight.
		4. The angle from the light direction to the spot light direction.
*/


//If we want to mimic a flashlight, we can have the camera's position and direction be the corresponding values for
// light position and direction.
struct Light{
	vec3 position;
	vec3 direction;
	/*
		The reason the cutoff is a float value is because it is easier to calculate the cosine of our angle and the 
		dot product of their results. Otherwise we'd have to calculate the inverse cosine of our values which is 
		expensive.

		Basically if the dot product of the light direction and spotlight direction are within range of the cutoff,
		we can render the fragments as lit.
	*/
	float cutoff;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Light light;

struct Material{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};
uniform Material material;

in vec3 fragment_position;
in vec3 fragment_normal;

uniform vec3 view_position;

void main(){
	//Calculate the direction from every fragment.
	vec3 light_direction = normalize(light.position - fragment_position);
	
	
	float theta = dot(light_direction, normalize(-light.direction));

	if (theta > light.cutoff){
		//ambient
		vec3 ambient = light.ambient * material.ambient;

		//diffuse
		vec3 normal = normalize(fragment_normal);
		float diff = max(dot(normal, -light_direction), 0.0f);
		vec3 diffuse = diff * light.diffuse * material.diffuse;
		
		//specular
		vec3 view_direction = normalize(view_position - fragment_position);
		vec3 reflect_direction = reflect(-light.direction, normal);  
		float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
		vec3 specular = light.specular * material.specular * spec;

		//attenuation
		float attenuation_distance = length(light.position - fragment_position);
		float attenuation = 1.f / (light.constant + light.linear * attenuation_distance + light.quadratic * (attenuation_distance * attenuation_distance));

		vec3 result = (diffuse + ambient + specular) * attenuation;

		fragment_color = vec4(result, 1.0f);
	}else{
		fragment_color = vec4(light.ambient * material.diffuse, 1.0f);
	}
}