#pragma once
#include "libs.h"
#include "Image.h"
#include <iostream>

class Texture {
public:
	void generate_and_bind_texture();
	void generate_and_bind_texture(int number_of_textures);
	void set_texture_2D_paramteri(GLenum s_wrap_option, GLenum t_wrap_option, GLenum min_distance_options, GLenum mag_distance_option);
	void apply_texture(const char* image_path);
	void create_texture(const char* image_path);
	int get_color_channel(Image& image);
	unsigned int texture;
	int texture_unit;
};