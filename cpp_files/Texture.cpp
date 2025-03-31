#include "../headers/Texture.h"

void Texture::generate_and_bind_texture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::generate_and_bind_texture(int number_of_textures) {
	glGenTextures(number_of_textures, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::set_texture_2D_paramteri(GLenum s_wrap_option, GLenum t_wrap_option, GLenum min_distance_options, GLenum mag_distance_option) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_wrap_option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_wrap_option);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_distance_options);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_distance_option);
}

void Texture::apply_texture(const char* image_path) {
	Image image(image_path);
	image.load_data_through_stbi(image_path);
	if (image.image_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, get_color_channel(image), image.width, image.height, 0, get_color_channel(image), GL_UNSIGNED_BYTE, image.image_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture at. " << image_path << "\n";;
	}
	stbi_image_free(image.image_data);
}

void Texture::create_texture(const char* image_path) {
	generate_and_bind_texture();
	set_texture_2D_paramteri(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	apply_texture(image_path);
}

int Texture::get_color_channel(Image& image) {
	if (image.color_channels == 3) return GL_RGB;
	if (image.color_channels == 4) return GL_RGBA;
}