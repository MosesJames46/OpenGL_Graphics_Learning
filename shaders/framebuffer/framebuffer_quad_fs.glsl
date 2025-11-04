#version 330 core

out vec4 fragment_color;

in vec2 fragment_texture_coordinate;

uniform sampler2D fragment_texture_value;

uniform int shader_option;
					
void main(){
	vec4 result;

	switch(shader_option){
		case 0: 
			result = vec4(vec3(1.f - texture(fragment_texture_value, fragment_texture_coordinate)), 1.0f); 
			break;
		case 1: 
			result = texture(fragment_texture_value, fragment_texture_coordinate);
			float average = (result.r + result.g + result.b) / 3;
			result = vec4(average, average, average, 1.f);
			break;
		case 2: {
			result = texture(fragment_texture_value, fragment_texture_coordinate);
			//This grayscale operates in this manner because vertain values of color are stornger for the human eye. 
			//It would be better if we named out values based on their corresponding rgb intensities, but this usffices for now.
			float average = (result.r * 0.2126f + result.g * 0.7152f + result.b * 0.0722f) / 3.f;
			result = vec4( average, average, average, 1.0f);
			break;
			}
		default:
			result = texture(fragment_texture_value, fragment_texture_coordinate);
		
	}
	
	fragment_color = result;
}