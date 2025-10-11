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

		https://learnopengl.com/Lighting/Light-casters
	*/
	float cutoff;
	float outerCutoff;
	float radiusMultiplier;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
uniform Light light;

struct Material{
	sampler2D diffuse_texture;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};
uniform Material material;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 texture_coordinates;

uniform vec3 view_position;

//Check during creation if the material is textured or not.
uniform bool is_textured;

vec3 get_ambient();
vec3 get_diffuse(vec3 normal);
vec3 get_specular(vec3 normal);
float get_attenuation();
float get_radius_intensity(float theta);

vec3 smooth_edge_lighting(float intensity);
vec3 hard_edge_lighting(float theta, float intensity);

void main(){
	//Calculate the direction from every fragment.
	vec3 light_direction = normalize(light.position - fragment_position);
	
	
	//spotlight
	float theta = dot(light_direction, normalize(-light.direction));
	float intensity = get_radius_intensity(theta);

	fragment_color = vec4(smooth_edge_lighting(intensity), 1.0f);
}

vec3 get_ambient(){
	if(is_textured){
		return light.ambient * texture(material.diffuse_texture, texture_coordinates).rgb;
	}

	return light.ambient * material.ambient;
}

vec3 get_diffuse(vec3 normal){
	vec3 light_direction = normalize(light.position - fragment_position);
	float diff = max(dot(normal, -light_direction), 0.0f);

	if (is_textured) {
		return light.diffuse * texture(material.diffuse_texture, texture_coordinates).rgb * diff;
	}

	return light.diffuse * material.diffuse * diff;
}

vec3 get_specular(vec3 normal){
	vec3 view_direction = normalize(view_position - fragment_position);
	vec3 reflect_direction = reflect(light.direction, normal);  
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	return light.specular * material.specular * spec;
}

float get_attenuation(){
	float attenuation_distance = length(light.position - fragment_position);
	return 1.f / (light.constant + light.linear * attenuation_distance + light.quadratic * (attenuation_distance * attenuation_distance));
}

float get_radius_intensity(float theta){
	/*
		The distance between the outer and cutoff is always the same.

		There is a linear interpolation calculater between these values for where the fragments lie.
	*/
	float epsilon = light.cutoff - light.outerCutoff;
	return smoothstep(0.0f, 1.0f, (theta - light.outerCutoff) / epsilon);
}

vec3 hard_edge_lighting(float theta, float intensity){
	if (theta > light.cutoff){
		return smooth_edge_lighting(intensity);
	}
}


/*
	Does not use a hard cutoff, instead, measures intensity based off our epsilon from get_spotlight.
*/
vec3 smooth_edge_lighting(float intensity){
	vec3 normal = normalize(fragment_normal);

	//ambient
	vec3 ambient = get_ambient();

	//diffuse
	vec3 diffuse = get_diffuse(normal) * intensity;
		
	//specular
	vec3 specular = get_specular(normal) * intensity;

	//attenuation
	float attenuation = get_attenuation();

	vec3 result = (ambient + diffuse + specular) * attenuation;
	return result;
}