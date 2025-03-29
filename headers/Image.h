#pragma once
#include "headers/libs.h"

class Image {
public:
	unsigned char* load_data_through_stbi(const char* file_path_to_image);

	Image(const char* file_path_to_image) {
		image_data = load_data_through_stbi(file_path_to_image);
	}

private:
	int width, height, color_channels;
	unsigned char* image_data;
};