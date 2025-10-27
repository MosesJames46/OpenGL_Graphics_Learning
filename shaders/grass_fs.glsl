#version 330 core
out vec4 fragment_color;

in vec2 texture_coordinates;

uniform sampler2D grass_texture;

void main(){
	//Discard texture if it's alpha value is less than .1
	vec4 texture_value = texture(grass_texture, texture_coordinates);

	/*
	Whether discard “cancels execution” depends entirely on the hardware. In general, execution will only be canceled if an entire pixel group 
	(the size of which depends on hardware) all execute a discard. So if you’re trying to discard to avoid expensive computations, it may or may not help,
	depending on the spatial locality of the discards.
	*/

	/*
	"discard" is bad for every mainstream graphics acceleration technique - IMR, TBR, TBDR. This is because visibility of a fragment (and hence depth) 
	is only determinable after fragment processing and not during Early-Z 
	*/
	if (texture_value.a < .1f) discard;
	fragment_color =  texture_value;
}