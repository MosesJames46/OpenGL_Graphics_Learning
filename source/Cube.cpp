#include "../headers/Cube.h"
#include "../imgui/imgui.h"
#include "../headers/Shader.h"
#include "../headers/Camera.h"

Cube::Cube() {
	//[CUBE VERTEX DATA]
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	//[CUBE VERTEX DATA]

	//[QUAD VERTEX DATA]
	glGenVertexArrays(1, &quad_VAO);
	glGenBuffers(1, &quad_VBO);

	glBindVertexArray(quad_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	//[QUAD VERTEX DATA]

	//[REAR VIEW MIRROR DATA]
	glGenVertexArrays(1, &rear_VAO);
	glGenBuffers(1, &rear_VBO);

	glBindVertexArray(rear_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rear_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rear_view), rear_view, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//[REAR FRAMEBUFFER]
	//glGenFramebuffers(1, &rear_FBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, rear_FBO);


	glGenTextures(1, &rear_texture);
	glBindTexture(GL_TEXTURE_2D, rear_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, rear_texture, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//
	//glGenRenderbuffers(1, &rear_RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, rear_RBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "The rear_view framebuffer has failed" << std::endl;
	//[REAR VIEW MIRROR]



	//[FRAMEBUFFER]
	//Create a framebuffer and with it attatch a texture with colors and a renderbuffer with depth and stencil enabled. 
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, rear_texture, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	//What to store in the renderbuffer: in this case we want to store the depth and stencil since we won't be using them in the shader.
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERER, 0);
	//Attach to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	//Esnure the framebuffer has successfully attached everything.
	auto framebuffer_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "The framebuffer has failed to properly attatch items." << std::endl;
	//std::cout << framebuffer_status << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//[FRAMEBUFFER]

	//raw_address data to a string
	const void* raw_address_data = reinterpret_cast<const void*>(this);
	std::ostringstream stream_data;
	stream_data << raw_address_data;
	name = stream_data.str();
}

void Cube::draw_cube(Camera& camera) {

	if (show_imgui) {
		if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) slider_speed = .1f;
		else { slider_speed = .01f; }

		ImGui::Begin(name.c_str());

		ImGui::DragFloat("Uniform Scale", &uniform_scale, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
	   
		
		ImGui::DragFloat3("Scale Matrix", s, slider_speed, std::numeric_limits<float>::lowest(),
			std::numeric_limits<float>::max());
		scale[0][0] = s[0];
		scale[1][1] = s[1];
		scale[2][2] = s[2];

		glm::mat4 model = glm::mat4(1.0f);
		ImGui::DragFloat3("Rotation Matrix", rotate_scalars, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		rotate = glm::rotate(model, glm::radians(rotate_scalars[0]), glm::vec3(1, 0, 0));
		rotate = glm::rotate(rotate, glm::radians(rotate_scalars[1]), glm::vec3(0, 1, 0));
		rotate = glm::rotate(rotate, glm::radians(rotate_scalars[2]), glm::vec3(0, 0, 1));

		ImGui::DragFloat3("Translation Matrix", t, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		translate[3][0] = t[0];
		translate[3][1] = t[1];
		translate[3][2] = t[2];

		if (ImGui::BeginCombo("OpenGL Culling", opengl_cullface_string_options[cullface_index].c_str())){
			for (int i = 0; i < opengl_cullface_string_options.size(); ++i) {
				bool selected = (cullface_index == i);
				if (ImGui::Selectable(opengl_cullface_string_options[i].c_str(), &selected)) cullface_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Front Facing", opengl_frontface_string_options[frontface_index].c_str())) {
			for (int i = 0; i < opengl_frontface_string_options.size(); ++i) {
				bool selected = (frontface_index == i);
				if (ImGui::Selectable(opengl_frontface_string_options[i].c_str(), &selected)) frontface_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::Checkbox("Highlight Cube?", &using_highlight);

		if (using_highlight) ImGui::DragFloat("Highlight Influence", &highlight_influence, slider_speed, 
			std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		

		if (ImGui::BeginCombo("Shader", shaders[shader_index].c_str())) {
			
			for (int i = 0; i < shaders.size(); ++i) {
				bool selected = (shader_index == i);
				if (ImGui::Selectable(shaders[i].c_str(), &selected)) shader_index = i;
				if (selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Kernel Divisor", &kernel_divisor, slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());

		ImGui::SeparatorText("Kernel Values");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Top Left", &kernel[0], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine(); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Top Center", &kernel[1], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine(); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Top Right", &kernel[2], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Center Left", &kernel[3], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine(); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Center Center", &kernel[4], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine(); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Center Right", &kernel[5], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Bottom Left", &kernel[6], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine(); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Bottom Center", &kernel[7], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); ImGui::SameLine(); ImGui::PushItemWidth(100);
		ImGui::DragFloat("##Kernel Bottom Right", &kernel[8], slider_speed, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()); 

		ImGui::End();
	}
	/*
		All subsequent rendering operations will now render to the attachments of the currently bound framebuffer. 
		Since our framebuffer is not the default framebuffer, the rendering commands will have no impact on the 
		visual output of your window. For this reason it is called off-screen rendering when rendering to a 
		different framebuffer. 
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.1f, .1f, .1f, 1.f);
	glEnable(GL_DEPTH_TEST);

	//For Stencil Testing: Draw object with passing all stencil test only replacing when stencil and depth test pass. Then draw same object with stencil and depth test off.
	//[STENCIL DRAW]
	//glEnable(GL_STENCIL_TEST | GL_DEPTH_TEST);
	//glStencilFunc(GL_ALWAYS, 1, 0xff);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//[STENCIL DRAW]

	//[CULL FACE]
	/*
		When doing cull face, if the proper vertex order is set, then we can manipulate how we view the objects within a sense by culling faces.
		Faces are determined by whether or not they are front facing or back facing. This happens if vertex are set going clockwise or 
		counter-clockwise.

		https://learnopengl.com/Advanced-OpenGL/Face-culling
	*/

	

	//glEnable(GL_CULL_FACE);
	//glCullFace(opengl_cullface_options[cullface_index]);
	//glFrontFace(opengl_frontface_options[frontface_index]);


	//[FIRST OBJECT DRAWN TO THE FRAMEBUFFER]
	Shader cube_shader("shaders/cube_vs.glsl", "shaders/cube_fs.glsl");

	cube_shader.useProgram();

	cube_shader.set_uniform_location("uniform_scale", uniform_scale);

	cube_shader.set_uniform_location("scale", scale);
	cube_shader.set_uniform_location("rotation", rotate);
	cube_shader.set_uniform_location("translation", translate);

	cube_shader.set_uniform_location("model", camera.model);
	cube_shader.set_uniform_location("view", camera.view);
	cube_shader.set_uniform_location("projection", camera.projection);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//[FIRST OBJECT DRAWN TO THE FRAMEBUFFER]


	//[CULL FACE]

	////[STENCIL DRAW]
	////Redraw object but instead with stencil highlights
	//glDisable(GL_DEPTH_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//
	////Use highlighting shader.
	////Use highlighting shader.
	//Shader highlighted_cube("shaders/cube_vs.glsl", "shaders/cube_highlight_fs.glsl");
	//
	//highlighted_cube.useProgram();
	//
	//highlighted_cube.set_uniform_location("uniform_scale", uniform_scale);
	//
	//highlighted_cube.set_uniform_location("scale", scale);
	//highlighted_cube.set_uniform_location("rotation", rotate);
	//highlighted_cube.set_uniform_location("translation", translate);
	//
	//highlighted_cube.set_uniform_location("model", camera.model);
	//highlighted_cube.set_uniform_location("view", camera.view);
	//highlighted_cube.set_uniform_location("projection", camera.projection);
	//
	//highlighted_cube.set_uniform_location("highlight_influence", highlight_influence);
	//
	//highlighted_cube.set_uniform_location("using_highlights", using_highlight);
	//
	//
	//
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_STENCIL_TEST);
	////[STENCIL DRAW]

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(.1f, .1f, .1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//Shader cube_framebuffer("shaders/framebuffer/framebuffer_quad_vs.glsl", "shaders/framebuffer/framebuffer_quad_fs.glsl");
	//cube_framebuffer.useProgram();
	//cube_framebuffer.set_uniform_location("fragment_texture_value", 0);
	//cube_framebuffer.set_uniform_location("shader_option", (shader_options)shader_index);

	Shader cube_kernel_framebuffer("shaders/framebuffer/framebuffer_quad_vs.glsl", "shaders/framebuffer/kernel_fs.glsl");
	cube_kernel_framebuffer.useProgram();
	cube_kernel_framebuffer.set_uniform_location("fragment_texture_value", 0);
	GLint uniform = glGetUniformLocation(cube_kernel_framebuffer.programShaderID, "kernel");
	glUniform1fv(uniform, 9, kernel);
	cube_kernel_framebuffer.set_uniform_location("kernel_divisor", kernel_divisor);

	glBindVertexArray(quad_VAO);

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//Bind to the framebuffer again and clear the color and depth buffer to apply new values.
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 0.f, 1.f, 1.f);

	//[SECOND DRAW BUT WITH CAMERA REVERSED]
	cube_shader.useProgram();
	glm::vec3 forward = camera.camera_forward;
	//std::cout << camera.camera_forward.x << " " << camera.camera_forward.y << " " << camera.camera_forward.z << std::endl;
	camera.camera_forward = glm::rotate(glm::mat4(1.0f), glm::radians(180.f), camera.camera_up) * glm::vec4(camera.camera_forward, 1.0f);
	camera.view_through_camera();

	cube_shader.set_uniform_location("uniform_scale", uniform_scale);
	
	cube_shader.set_uniform_location("scale", scale);
	cube_shader.set_uniform_location("rotation", rotate);
	cube_shader.set_uniform_location("translation", translate);
	
	cube_shader.set_uniform_location("model", camera.model);
	cube_shader.set_uniform_location("view", camera.view);
	cube_shader.set_uniform_location("projection", camera.projection);
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	camera.camera_forward = forward;
	camera.view_through_camera();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.f, 0.f, .1f, 1.f);

	cube_kernel_framebuffer.useProgram();
	cube_kernel_framebuffer.set_uniform_location("fragment_texture_value", 0);
	uniform = glGetUniformLocation(cube_kernel_framebuffer.programShaderID, "kernel");
	glUniform1fv(uniform, 9, kernel);
	cube_kernel_framebuffer.set_uniform_location("kernel_divisor", kernel_divisor);

	glBindVertexArray(rear_VAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//[SECOND DRAW BUT WITH CAMERA REVERSED]
}