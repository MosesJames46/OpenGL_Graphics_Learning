#include "../headers/Sphere.h"
#include "../headers/Mesh.h"

constexpr const float pi = 3.1415926;

void Sphere::generate_vertices(std::vector<float>& vertices) {
	//Adding the top vertex of our sphere.
	vertices.push_back(0.f); 
	vertices.push_back(1.f);
	vertices.push_back(0.f);

	/*
	float current_stack_angle = half_pi - i * stack_step;

	Obtains a percentage of half a circle.
	when i is 0 only have influence in the y.
	When i is half of our sector count, we have both x and y distances equal to unit 1.
	Only when i is equal to the stack count do we enter -half_pi which gives us influence
	in the -y direction. This effectively acts as a force from pi/2 to -pi / 2;
	*/
	 
	for (int i = 0; i < stacks - 1; i++) {
		//Calculates the percentage from top to bottom.
		float phi = pi * static_cast<float>(i + 1) / static_cast<float>(stacks);
		for (int j = 0; j < slices; j++) {
			// 2 * pi is used because we need a full sphere. 
			float theta = 2.f * pi * static_cast<float>(j) / static_cast<float>(slices);
			float x = std::sin(phi) * std::cos(theta);
			float y = std::cos(phi);
			float z = std::sin(phi) * std::sin(theta);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	//Adding the bottom vertex of our sphere.
	vertices.push_back(0.f);
	vertices.push_back(-1.f);
	vertices.push_back(0.f);
}

void Sphere::generate_indices(std::vector<unsigned int>& indices) {

	// generate CCW index list of sphere triangles
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1

	int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (slices + 1);     // beginning of current stack
		k2 = k1 + slices + 1;      // beginning of next stack

		for (int j = 0; j < stacks; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

		}
	}
}

void Sphere::generate_mesh(Mesh& mesh, BoundingBox& bounds) {
	mesh.vertex_data.clear();
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * pi / slices;
	float stackStep = pi / stacks;
	float sectorAngle, stackAngle;



	for (int i = 0; i <= stacks; ++i)
	{
		stackAngle = (pi / 2.f) - i * stackStep;        // starting from pi/2 to -pi/2
		xy = cosf(stackAngle);							// r * cos(u)
		z =  sinf(stackAngle);							// r * sin(u)

		// add (sectorCount+1) vertices per stack
		// first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= slices; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			mesh.vertex_data.push_back(x);
			mesh.vertex_data.push_back(y);
			mesh.vertex_data.push_back(z);

			

			glm::vec3 vertex{ x, y, z };
			mesh.create_bounding_box(vertex);

			// vertex color
			mesh.vertex_data.push_back(mesh.color[0]);
			mesh.vertex_data.push_back(mesh.color[1]);
			mesh.vertex_data.push_back(mesh.color[2]);

			// vertex tex coord (s, t) range between [0, 1]
			s = static_cast<float>(j) / slices;
			t = static_cast<float>(i) / stacks;
			mesh.vertex_data.push_back(s);
			mesh.vertex_data.push_back(t);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			mesh.vertex_data.push_back(nx);
			mesh.vertex_data.push_back(ny);
			mesh.vertex_data.push_back(nz);
		}
	}

	//bottom back left
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_z);

	//top back left
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_z);

	//bottom back right
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_z);

	//top back right
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_z);

	//bottom front left
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_z);

	//top front left
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_z);

	//bottom front right
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().min_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_z);

	//top front right
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_x);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_y);
	mesh.bounding_box_data.push_back(mesh.get_bounds().max_z);

	for (int i = 0, j = 0; i < mesh.bounding_box_data.size(); ++i, ++j) {
		if (j == 3) {
			j = 0;
			std::cout << std::endl;
		}
		std::cout << mesh.bounding_box_data[i] << " ";
	}

	generate_indices(mesh.indices);
}