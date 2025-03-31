#pragma once
#include "Shapes.h"


class Cube : public Shape{
public:
	Cube() {
		initialize_mesh();
		ready_buffers();
	}

	void ready_buffers();
	void initialize_mesh();

	unsigned int cube_VAO, cube_VBO, cube_EBO;
	Mesh cube_mesh;
	std::vector<float> cube_vertices{
		// Front face
		-0.5f, -0.5f,  0.5f,    // Bottom-left
		 0.5f, -0.5f,  0.5f,    // Bottom-right
		 0.5f,  0.5f,  0.5f,    // Top-right
		-0.5f,  0.5f,  0.5f,    // Top-left

		// Back face
		-0.5f, -0.5f, -0.5f,   
		 0.5f, -0.5f, -0.5f,   
		 0.5f,  0.5f, -0.5f,   
		-0.5f,  0.5f, -0.5f,   

		// Left face
		-0.5f, -0.5f, -0.5f,   
		-0.5f, -0.5f,  0.5f,   
		-0.5f,  0.5f,  0.5f,   
		-0.5f,  0.5f, -0.5f,   

		// Right face
		 0.5f, -0.5f, -0.5f,   
		 0.5f, -0.5f,  0.5f,   
		 0.5f,  0.5f,  0.5f,   
		 0.5f,  0.5f, -0.5f,   

		 // Top face
		 -0.5f,  0.5f, -0.5f,  
		  0.5f,  0.5f, -0.5f,  
		  0.5f,  0.5f,  0.5f,  
		 -0.5f,  0.5f,  0.5f,  

		 // Bottom face
		 -0.5f, -0.5f, -0.5f,  
		  0.5f, -0.5f, -0.5f,  
		  0.5f, -0.5f,  0.5f,  
		 -0.5f, -0.5f,  0.5f  
	};

	std::vector<float> cube_texture_coords{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	std::vector<unsigned int> cube_indices{
		0, 1, 2,  2, 3, 0,  // Front
		4, 5, 6,  6, 7, 4,  // Back
		8, 9, 10, 10, 11, 8,  // Left
		12, 13, 14, 14, 15, 12,  // Right
		16, 17, 18, 18,19, 16,  // Top
		20, 21, 22, 22, 23, 20   // Bottom
	};	
	std::vector<Texture> cube_textures;
};		
		
		
		
		
		
		
		
		
		