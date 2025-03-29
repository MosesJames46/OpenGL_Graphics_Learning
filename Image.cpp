#include "headers/Image.h"



unsigned char* Image::load_data_through_stbi(const char* file_path_to_image) {
	return stbi_load(file_path_to_image, &width, &height, &color_channels, 0);
}