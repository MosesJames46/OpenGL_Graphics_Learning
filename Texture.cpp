#include "headers/Texture.h"

void Texture::generate_and_bind_texture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::generate_and_bind_textures(int number_of_textures) {
	glGenTextures(number_of_textures, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::set_texture_2D_paramteri(GLenum s_wrap_option, GLenum t_wrap_option, GLenum min_distance_options, GLenum mag_distance_option) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap_option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap_option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_distance_options);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_distance_option);
}