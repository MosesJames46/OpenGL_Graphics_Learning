#include "../headers/Grass.h"
#include "../imgui/imgui.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"
#include "../headers/Texture.h"
#include "../headers/Image.h"

Grass::Grass() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_data.size() * sizeof(float), vertex_data.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), index_data.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	int width, height, color_channels;
	unsigned char* image_data;

	image_data = stbi_load("grass.png", &width, &height, &color_channels, 0);// 

	const void* address = reinterpret_cast<const void*>(this);
	std::ostringstream address_sstream;
	address_sstream << address;
	name = address_sstream.str();
	
	if (image_data) {
		//Always generate texture. Try to keep texture functionality as close as possible.
		glGenTextures(1, &texture);

		/*
			By clamping to edge, we remove the linear interpolation that comes from texels when using GL_REPEAT.
		*/
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format;

		switch (color_channels) { //Depending on color_channels value select proper format.
		case 1:
			format = GL_RED; break;
		case 3:
			format = GL_RGB; break;
		case 4:
			format = GL_RGBA; break;
		default:
			format = GL_RGB; break;
		}
		//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml -> documentation on glTexImage2D
		glBindTexture(GL_TEXTURE_2D, texture); //Always bind texture before using.
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image_data);
		stbi_set_flip_vertically_on_load(true);//Texture usually comes flipped.
	}
	else {
		std::cout << "Could not process image." << std::endl;
		stbi_image_free(image_data);
	}
};

void Grass::draw_plane(Camera& camera) {
	float slider_speed = 0.01f;
	if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) slider_speed = .1f;
	if (edit_object) {
		ImGui::Begin(name.c_str());
		//Scale
		ImGui::DragFloat3("Scale Array Values", scale_array, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");
		scale[0][0] = scale_array[0];
		scale[1][1] = scale_array[1];
		scale[2][2] = scale_array[2];

		ImGui::DragFloat("Uniform Scale Value", &uniform_scale, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");
		//Rotation
		ImGui::DragFloat3("Rotation Array Values", rotation_array, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");

		rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians(rotation_array[0]), glm::vec3{ 1.0f, 0.0f, 0.0f });
		rotation = glm::rotate(rotation, glm::radians(rotation_array[1]), glm::vec3{ 0.0f, 1.0f, 0.0f });
		rotation = glm::rotate(rotation, glm::radians(rotation_array[2]), glm::vec3{ 0.0f, 0.0f, 1.0f });

		//Translation
		ImGui::DragFloat3("Translation Array Values", translation_array, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), "%.2f");

		translation[3][0] = translation_array[0];
		translation[3][1] = translation_array[1];
		translation[3][2] = translation_array[2];

		ImGui::End();
	}
	

	Shader shader("shaders/grass_vs.glsl", "shaders/grass_fs.glsl");

	shader.useProgram();

	shader.set_uniform_location("model", camera.model);
	shader.set_uniform_location("view", camera.view);
	shader.set_uniform_location("projection", camera.projection);

	shader.set_uniform_location("scale", scale);
	shader.set_uniform_location("rotation", rotation);
	shader.set_uniform_location("translation", translation);

	shader.set_uniform_location("uniform_scale", uniform_scale);

	shader.set_uniform_location("grass_texture", 0);

	/*
		The process for using transparent textures is to process their image and ensure that their alpha is set to Clamp.
		Then we want to blend alpha values. The formula for alpha blending is as follows:
			- Final = S_c * S_i + D_c * D_i, where S is source and D is destination and c is the color and i is the intensity of the colors.
			S refers to the output of the fragment shader, so this could be better named FS and D is the color buffer which could be named CB.
	*/
	glEnable(GL_BLEND);

	/*
		The glBlendFunc is what controls the formula for blending alpha values. For each pixel we can specify the Source operation and Destination operation.
			- glBlendFunc(Src Operation, Dest Operation);

		The glBlendEquation has parameters that specify how openGL performs the glBlendFunc operations.
		The defualt is to add the values together, but below is a link to the documentation that has all options available.
	*/
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml -> See doc for details.
	glBlendEquation(GL_FUNC_ADD);//https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBlendEquation.xhtml -> See doc for details.

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, index_data.size(), GL_UNSIGNED_INT, 0);
}