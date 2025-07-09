#pragma once
#pragma once
#include "libs.h"
#include <iostream>
#include <vector>


enum fragment_shader_type {
	STANDARD_SHADER, LIGHT_SHADER
};

class Camera;
class Mesh;
class Material;
class Texture;

class Renderer {
public:
	Renderer(Material& material, Camera& camera, Mesh& mesh) : material(material), camera(camera), mesh(mesh) {
		gen_bind_format();
		set_attributes(0, 3, 9, 0);
		//set_attributes(1, 6, 11, 3);
		set_attributes(1, 3, 9, 3);
		set_attributes(2, 3, 9, 6);
		unbind_buffers_and_attribute_pointer();
	}
	void unbind_buffers_and_attribute_pointer();

	/*
		Calls the glGen* and glBind* functions.

		1. glGenVertexArrays is the vertices array that attaches to the VBO and EBO.
		2. The VBO contains data for vertices
		3. The EBO contains data for indices.

		Can generate multiple VAOs, VBOs, and EBOs.

		Do not unbind the EBO.

		The order is always:
		1. Generate
		2. Bind.

		Most importantly generate the VAO first.
	*/
	void generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO, unsigned int& uninitialized_EBO);
	void generate_and_bind_buffers(unsigned int& uninitialized_VAO, unsigned int& uninitialized_VBO);
	void generate_and_bind_buffers();

	/*
		The format buffer function uses the glBufferData function and can take at most 2 types of construction.
			1. Using vertices.
			2. Using indices.
		Each corresponds to the GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER buffer respectively.

		The data corresponds to the buffer type listed above, the size in bytes of the buffer, a pointer to the begining of the buffer,
		and the GL_ENUM draw type.
	*/
	void format_buffer(GLenum draw_type);

	/*
		The set_attributes function is used to simplify vertex data to be sent to the gpu.

		The glVertexAttribPointer function takes data that is formatted to a vertex. The data expected is the size of each attribute where
		attributes are usually vertices, normals, colors, textures and etc that describe a vertex. The order specified is:
		1. The position of the expected attribute component as is expected to be sent to the gpu.
		2. The length of each attribute that describes a vertex.
		3. the GLenum type which is usually GL_FLOAT.
		4. The GLboolean to determine normalized. Usually false.
		5. The distance in bytes from the current component that describes a specific attribute of a vertex to the next of equivalent
		description. (e.g. if currently on attribute that describes color, what is distance from next color description?)
		5. The offset from the very first attribute that describes a vertex.
	*/
	void set_attributes(int attribute_position, int attribute_size, GLenum numerical_type, GLboolean normalized, int stride_size, int offset);
	void set_attributes(int attribute_position, int attribute_size, int stried_size, int offset);

	//Calls the generate_and_bind_buffers and the format_buffer functions in one call.
	virtual void gen_bind_format();

	virtual void ready_buffers() {};
	
	void attach_uniform(const char* uniform_name, std::vector<float>& color);
	void attach_uniform(const char* uniform_name, std::vector<float>&& color);

	void bind_textures(std::vector<const char*>&& uniform_names, std::vector<Texture>& texture_vector);
	void bind_textures(std::vector<const char*>& uniform_names, std::vector<Texture>& texture_vector);

	void add_textures(std::vector<const char*> file_paths, std::vector<Texture>& texture_vector);

	void set_MVP();

	void set_object_values(Mesh& object, Mesh& other);

	virtual void draw(const char* ImGui_object_name) {};
	/*
		The draw function handles draw calls to the gpu. It requires a shader, VAO, and the type of drawing using.

		The structure is as follows:
			use shader program;
			glBindVertexArray(VAO);
			glDrawElements(Primitive type, amount of vertices/indices to draw, GL_UNSIGNED_BYTE, 0);
	*/
	void draw();
	void redraw();

	const char* get_shader_type(fragment_shader_type);
	
	Mesh& mesh;
	Material& material;
	Camera& camera;
};