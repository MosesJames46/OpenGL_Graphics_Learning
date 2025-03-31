#include "../headers/Cube.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/gtx/string_cast.hpp"

void Cube::ready_buffers() {
	gen_bind_format(cube_mesh, cube_indices, cube_VAO, cube_VBO, cube_EBO);
	set_attributes(0, 3, 8, 0);
	set_attributes(1, 3, 8, 3);
	set_attributes(2, 2, 8, 6);
}

void Cube::initialize_mesh() {
	cube_mesh.load_vertices(cube_vertices);
	cube_mesh.load_textures(cube_texture_coords);
}