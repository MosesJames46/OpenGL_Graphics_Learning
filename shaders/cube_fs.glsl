#version 330 core


in vec3 fragment_color;
in vec3 fragment_normal;
in vec3 fragment_position;

out vec4 pixel_color;

uniform bool has_normals;

uniform vec3 camera_position;

uniform samplerCube skybox;

uniform int rr_setting;
uniform float ratio;

void main(){
	vec3 final_color;
	if (has_normals) {
		vec3 direction = normalize(fragment_position - camera_position);
		vec3 I;
		switch(rr_setting){
		case 0:
			I = reflect(direction, fragment_normal);
			break;
		case 1:
			{
			float r = 1.f / ratio;
			I = refract(direction, fragment_normal, r); 
			break;
			}
		}

		final_color = texture(skybox, I).rgb;
	}else{
		final_color = fragment_color;
	}

	pixel_color = vec4(final_color, 1.0f);
}