#include "../headers/Sphere.h"

void Sphere::initialize_mesh(float sectors, float stacks) {
	float pi = 3.14159;
	int sector_count = sectors;
	int stack_count = stacks;
	float sector_step = 2 * pi / sector_count;
	float stack_step = pi / stack_count;
	float xy, x, y, z;
	float s, t;
	vertices.clear();
	texture_coordinates.clear();
	sphere_indices.clear();
	

	for (int i = 0; i <= stack_count; i++) {
		float current_stack_angle = (pi / 2) - i * stack_step;
		xy = radius *  cosf(current_stack_angle);
		z = radius * sinf(current_stack_angle);
		for (int j = 0; j <= sector_count; j++) {
			float current_sector_angle = j * sector_step;
			x = xy * cosf(current_sector_angle);
			y = xy * sinf(current_sector_angle);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			s = (float)j / sector_count;
			t = (float)i / stack_count;
			texture_coordinates.push_back(s);
			texture_coordinates.push_back(t);
		}
	}

	sphere_mesh.load_vertices(vertices);
	sphere_mesh.load_textures(texture_coordinates);
	int k1, k2;
	for (int i = 0; i < stack_count; i++) {
		k1 = i * (sector_count + 1);
		k2 = k1 + sector_count + 1;
		for (int j = 0; j < sector_count; j++, k1++, k2++) {
			if (i != 0) {
				sphere_indices.push_back(k1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k1 + 1);
			}

			if (i != (stack_count)) {
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}

			if (i != stack_count - 1) {
				sphere_indices.push_back(k1 + 1);
				sphere_indices.push_back(k2);
				sphere_indices.push_back(k2 + 1);
			}

			sphere_indices.push_back(k1);
			sphere_indices.push_back(k2);

			if (i != 0) {
				sphere_indices.push_back(k1);
			}
		}
	}
}

void Sphere::set_radius(float radius_input) {
	radius = radius_input;
	initialize_mesh(sectors, stacks);
	ready_buffers();
	unbind_buffers_and_attribute_pointer();
}

void Sphere::ready_buffers() {
	gen_bind_format(sphere_mesh, sphere_indices, sphere_VAO, sphere_VBO, sphere_EBO);
	set_attributes(0, 3, 8, 0);
	set_attributes(1, 3, 8, 3);
	set_attributes(2, 2, 8, 6);
	unbind_buffers_and_attribute_pointer();
}