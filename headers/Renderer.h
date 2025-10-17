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
class Shader;

class Light_Mesh;
class Spotlight_Mesh;
class Complex_Mesh;
class Texture_Mesh;

class Renderer {
public:
	Renderer(std::unique_ptr<Mesh> mesh, std::unique_ptr<Material> material, Camera& camera);


	~Renderer(){}

	std::unique_ptr<Mesh> mesh;
	std::unique_ptr<Material> material;
	Camera& camera;

	void initiate_renderer();

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
	void generate_and_bind_buffers(Mesh& mesh);

	/*
		The format buffer function uses the glBufferData function and can take at most 2 types of construction.
			1. Using vertices.
			2. Using indices.
		Each corresponds to the GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER buffer respectively.

		The data corresponds to the buffer type listed above, the size in bytes of the buffer, a pointer to the begining of the buffer,
		and the GL_ENUM draw type.
	*/
	void format_buffer(Mesh& mesh, GLenum draw_type);

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
	virtual void gen_bind_format(Mesh& mesh);

	virtual void ready_buffers() {};
	
	void attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>& color);
	void attach_uniform(Shader& shader, const char* uniform_name, std::vector<float>&& color);

	void bind_textures(Shader& shader, std::vector<const char*>&& uniform_names);
	void bind_textures(Shader& shader, std::vector<const char*>& uniform_names);

	void add_textures(Shader& shader, std::vector<const char*> file_paths, std::vector<const char*> uniform_locations);

	void set_MVP(Shader& shader, Camera& camera);

	

	virtual void draw(const char* ImGui_object_name) {};
	/*
		The draw function handles draw calls to the gpu. It requires a shader, VAO, and the type of drawing using.

		The structure is as follows:
			use shader program;
			glBindVertexArray(VAO);
			glDrawElements(Primitive type, amount of vertices/indices to draw, GL_UNSIGNED_BYTE, 0);
	*/

	void draw(bool renderer_gui = false);
	void draw_highlights(bool renderer_gui = false);

	void apply_shaders(Material& material, bool render);

	void redraw();

	const char* get_shader_type(fragment_shader_type);
	
	std::vector<Texture> textures;
private:

	//The stencil highlighting technique is to write during the first draw call of your object and always pass the stencil test. Next turn off writing and fail values not
	//equal to the stencil buffer. This should highlight values of that model that are not exactly where the bits are placed within the stencil buffer.
	void stencil_data_for_highlighting() {
		//glStencilOp: What actions to take if stencil test passes or fails and if depth test passes or fails.
		//glStencilOp: sfail, dpfail, dppass -> stencil fail, stencil pass but depth fails, and both pass.
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glClear(GL_STENCIL_BUFFER_BIT);
		//glStencilFunc sets parameters that determine if the stencil test will pass.
		//glStencilFunc: enum, ref, mask -> the enum is the requirement necessary to pass a stencil test. See https://learnopengl.com/Advanced-OpenGL/Stencil-testing
		//-> ref is the value that will be and against the mask. Mask are the values set to the stencil buffer.
		//The glStencilMask function tells the stencil to start processing request if 0xFF is passed and vice versa if 0x0 is passed.

		//Draw after enabling writing to stencil buffer.
	}

	void disable_stencil_write() {
		//Before we draw the object again, we need to disable the stencil buffer.

		//After the draw call, use the glStnecilFunc to check if any values are not equal.
		//Depth testing is disabled to that our newly rendered object is behind our current object.

		//This tells OpenGL that whenever the stencil value of a fragment is equal (GL_NOTEQUAL) to the reference value 1, 
		// the fragment passes the test and is drawn, otherwise discarded - LearnOpenGl. 
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); //Bit in the stencil buffer becomes zero (disables writing).
		glDisable(GL_DEPTH_TEST);
	}

	void enable_stencil_write_and_depth() {
		//At this point we want to enable writing to overwrite the values that are zero.
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

	}
};