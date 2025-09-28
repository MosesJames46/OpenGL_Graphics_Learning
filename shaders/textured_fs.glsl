#version 330 core

out vec4 fragment_colors;

/*
	The Material struct is used to hold data for an object that gets effected by light. 
	
	The diffuse can be a number of things, but the current setting is a texture. 
	We cann use basic colors, such as diffuse, but we will go with a sampler2D for out Material object.

	Most materials in the real world have the basic properties of:
		1. Diffuse
		2. Ambient
		3. Specular 
		4. Shininess 

	There are more, but currently these are all that's mentioned so far.
*/

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
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

in vec3 normal_outputs;
in vec3 fragment_positions;
in vec2 texture_coordinates;

//The sole purpose of the view position is to resemble specluar reflection of a shiny object.
uniform vec3 view_position;


void main(){
	vec3 norm = normalize(normal_outputs);
	vec3 light_direction = normalize(fragment_positions - light.position);


	//Ambient is the most basic calculation being a constant value of light and material color.
	//The texture function takes in as arguments the uniform location and the texture_coordinates.

	float dist = length(light.position - fragment_positions);
	float attenuation = 1.f / (light.constant + light.linear * dist +  light.quadratic *  (dist * dist));

	vec3 ambient = vec3(texture(material.diffuse, texture_coordinates)) * light.ambient * attenuation;

	/*
		We use the normals and light_direction to calculate the diffuse values.

		The calculation requires the normals of the fragments tested against the normals of the light.
		The dot product yields values that resemble light on an object.
	*/ 
	float diffuse = max(dot(norm, light_direction), 0.0f);

	//The texture function takes in as arguments the uniform location and the texture_coordinates.
	vec3 diff = diffuse * vec3(texture(material.diffuse, texture_coordinates)) * light.diffuse  * attenuation;


	//There is a documentation of reflection online.
	vec3 view_direction = normalize(view_position - fragment_positions);
	vec3 reflect_direction = reflect(light_direction, norm);  

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, texture_coordinates)) * spec * light.specular  * attenuation; 

	

	vec3 result = ambient + diff + specular;

	fragment_colors = vec4(result, 1.0f);
}