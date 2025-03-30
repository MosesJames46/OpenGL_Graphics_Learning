#pragma once
#include "libs.h"

class Texture {
public:
	void generate_and_bind_texture();
	void generate_and_bind_textures(int number_of_textures);
	void set_texture_2D_paramteri(GLenum s_wrap_option, GLenum t_wrap_option, GLenum min_distance_options, GLenum mag_distance_option);
	template <typename Image>
	void apply_texture(Image image);

private:
	unsigned int texture;
};