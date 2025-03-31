#pragma once
#include "libs.h"
#include "stb_image.h"

class Image {
public:
	Image(const char* file_path_to_image) {
		image_data = load_data_through_stbi(file_path_to_image);
	}

	unsigned char* load_data_through_stbi(const char* file_path_to_image);
	int width, height, color_channels;
	unsigned char* image_data;
};