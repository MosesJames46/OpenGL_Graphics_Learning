#include "../headers/Mesh.h"
#include "../headers/Sphere.h"
#include "../headers/libs.h"
#include "../headers/Camera.h"
#include "../headers/Shader.h"

Mesh::Mesh(const std::string& name, shape_type shape, Camera& camera) : name(name), camera(camera), shape(shape) {
	id = mesh_number();
	/*
		Give the Mesh a unique address. This avoids name duplicates.
	*/
	const void* n = static_cast<const void*>(this);
	std::ostringstream ss;
	ss << n;
	this->name += std::string(" " +  ss.str());
	switch (shape) {
	case SPHERE: {
		Sphere sphere(name, 32, 32, 1);
		initialize_mesh(sphere);
		break;
		}	
	}
}

Mesh::Mesh(GLFWwindow* window, std::string file, const std::string& name, shape_type shape, Camera& camera): Mesh(window, name, shape, camera) {
	std::ifstream file_stream(file);
	if (!file_stream.is_open()) {
		std::cout << "Could not open file." << std::endl;
		return;
	}

	std::string line;
	int id = -1;
	while (!file_stream.eof()) {
		std::getline(file_stream, line);
		std::istringstream part(line);
		std::string l;
		//part << line;
		//std::cout << part.str() << std::endl;
		//std::cout << line << std::endl;

		if (line.compare(0, 2, "v ") == 0) {
			std::string x, y, z, w;
			part >> l;
			std::string::difference_type n = std::count(line.begin(), line.end(), ' ');
			int count = static_cast<int>(n);
			//std::cout << count << std::endl;

			if (count == 3) {
				part >> x >> y >> z;
				vertices.push_back(std::stof(x));
				vertices.push_back(std::stof(y));
				vertices.push_back(std::stof(z));
			}
			else {
				part >> x >> y >> z >> w;
				int fx, fy, fz, fw;
				fx = std::stof(x);
				fy = std::stof(y);
				fz = std::stof(z);
				fw = std::stof(w);
				if (fw != 0) {
					fx /= fw;
					fy /= fw;
					fz /= fw;
				}
				vertices.push_back(fx);
				vertices.push_back(fy);
				vertices.push_back(fz);
			}
			
			//glm::vec3 v(std::stof(x), std::stof(y), std::stof(z));
			
			//std::cout << x << " " << y << " " << z << std::endl;
			
			++id;
		}else if (line.compare(0, 3, "vn ") == 0) {
			part >> l;
			std::string x, y, z;
			part >> x >> y >> z;
			normals.push_back(std::stof(x));
			normals.push_back(std::stof(y));
			normals.push_back(std::stof(z));
			//std::cout << x << " " << y << " " << z << std::endl;
		} else if (line.compare(0, 2, "f ") == 0) {
			//std::cout << part.str() << " ";
			while (!part.eof()) {
				part >> l;
				//std::cout << part.str() << std::endl;
				//Find substring containing correct face index. Then shorten the string to fit.
				if (l.find('/') < std::string::npos) {
					//std::string num(l.begin() + 2, l.begin() + l.find('/'));
					l = l.substr(0, l.find('/'));
					indices.push_back(std::stoi(l) - 1);
					//std::cout << l;
				}
				else {
					//indices.push_back(std::stoi(l));
					if (!isalpha(l[0]))
						std::cout << std::stoi(l) << std::endl;
				}
				//std::cout << std::endl;
			}
		} else if (line.compare(0, 3, "vt ") == 0) {
			part >> l;
			std::string u, t;
			part >> u >> t;
			texture_coordinates.push_back(std::stof(u));
			texture_coordinates.push_back(std::stof(t));
		}
	}

	vertex_data.clear();

	for (int i = 0, j = 0; i < vertices.size() && i < normals.size(); i += 3, j += 2) {
		vertex_data.push_back(vertices[i]);
		vertex_data.push_back(vertices[i + 1]);
		vertex_data.push_back(vertices[i + 2]);

		vertex_data.push_back(0);
		vertex_data.push_back(0);
		vertex_data.push_back(0);

		vertex_data.push_back(normals[i]);
		vertex_data.push_back(normals[i + 1]);
		vertex_data.push_back(normals[i + 2]);

		if (j < texture_coordinates.size()) {
			vertex_data.push_back(texture_coordinates[j]);
			vertex_data.push_back(texture_coordinates[j + 1]);
		}
	}

	std::cout << "# v: " << vertices.size() / 3 << " f: " << indices.size() << "#vt: " << texture_coordinates.size() / 2  << "vn: " << normals.size()/3 << std::endl;
	//std::cout << "Vertices: " << vertex_data.size() << std::endl;
}

//Just so I can remember this is called delegating constructors.
Mesh::Mesh(GLFWwindow* window, const std::string& name, shape_type shape, Camera& camera) : Mesh(name, shape, camera) {
	this->window = window;
	//Further research is needed on this.
	if (!window) std::cout << "Window is dead" << std::endl;
	if (!this) std::cout << "The mesh is dead" << std::endl;
	glfwSetWindowUserPointer(window, this);
}


void Mesh::initialize_mesh(Sphere& sphere) {
	sphere.generate_mesh((*this), bounds);
}

void Mesh::get_screencoordiantes() {
	glfwGetCursorPos(window, &x_position, &y_position);
}

void Mesh::get_NDC() {
	/*
		https://community.khronos.org/t/how-to-get-screen-size-in-opengl/56802
		We can use an OpenGL directive to obtain the viewport since we set the callback in main.

		0 = x1 , 1 = y1, 2 =x2, 3 = y2;
	*/
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);

	x_NDC = ((2.f * x_position) / m_viewport[2]) - 1.f;
	y_NDC = 1.f - ((2.f * y_position) / m_viewport[3]);
	z_NDC = 1.f;
	ray_in_clipspace = glm::vec4{ x_NDC, y_NDC, 1.f, 1.f };
}

/*
	https://antongerdelan.net/opengl/raycasting.html
	Convert from clip space to world space by inversing our pipeline.
*/
void Mesh::clip_to_worldspace() {
	ray_in_eyespace = glm::inverse(camera.view) * ray_in_clipspace;
	ray_in_worldspace = glm::inverse(camera.projection) * glm::inverse(camera.view) * ray_in_clipspace;
	ray_direction = camera.camera_forward;
}


void Mesh::sphere_intersection_test() {

}

/*
	The equation for a point on a bounding box is:
		O + dt = x, where O is the origin from the object, d is the direction that ray is pointing, t is the scalar value that determines how many,
		points are on the ray, and x is the destination target on the plane.

		Therefore, we solve for t to get, (x - O) / d.
*/

bool Mesh::bounding_box_intersection_test() {
	glm::vec3 ray_origin = camera.camera_origin;
	glm::vec3 ray_direction = camera.camera_forward;
	glm::vec3 inverse_ray_direction{ 1 / ray_direction.x, 1 / ray_direction.y, 1 / ray_direction.z };

	float minimums[3]{ bounds.min_x, bounds.min_y, bounds.min_z };
	float maximums[3]{ bounds.max_x, bounds.max_y, bounds.max_z };


	//Perform line equation to determine the value needed to scale t. If t_min is < t_max then we are facing negative.
	float t_min; float t_max;

	t_min = (minimums[0] - ray_origin[0]) * ray_direction[0];
	t_max = (maximums[0] - ray_origin[0]) * ray_direction[0];

	if (t_min > t_max) std::swap(t_min, t_max);

	float ty_min = (minimums[1] - ray_origin[1]) * ray_direction[1];
	float ty_max = (maximums[1] - ray_origin[1]) * ray_direction[1];

	if (ty_min > ty_max) std::swap(ty_min, ty_max);

	//If the tx_0 > ty_1 || tx_1 < ty_0 return false;
	if ((t_min > ty_max) || (t_max < ty_min)) return false;

	if (ty_min > t_min) t_min = ty_min;
	if (ty_max < t_max) t_max = ty_max;

	float tz_min = (minimums[2] - ray_origin[2]) * inverse_ray_direction[2];
	float tz_max = (maximums[2] - ray_origin[2]) * inverse_ray_direction[2];

	if (tz_min > tz_max) std::swap(tz_min, tz_max);

	if ((t_min > tz_max) || (t_max < tz_min)) return false;

	if (tz_min > t_min) t_min = tz_min;
	if (tz_max < t_max) t_max = tz_max;

	return true;
}

void Mesh::UI_get_bounding_box() {
	
	ImGui::SeparatorText("Bounding Box");
	ImGui::Text("Bounding Box values x (%g, %g) , y (%g, %g), z (%g, %g)", bounds.min_x, bounds.max_x
		, bounds.min_y, bounds.max_y, bounds.min_z, bounds.max_z);
}

void Mesh::set_color() {
	//Use the starting position of the float pointer.
	ImGui::SeparatorText("Color");
	ImGui::ColorEdit3("##Color", glm::value_ptr(color));
	//We access the float values and cast them to a vec 3.
}

void Mesh::set_float() {
	//ImGui::DragFloat(float_name, &value, .1f, min, max);
	//shader.set_uniform_location(uniform_name, value);
}

void Mesh::set_ambient(){
	ImGui::SeparatorText("Ambient");
	ImGui::ColorEdit3("##Ambient", glm::value_ptr(ambient));
}

void Mesh::set_specular(){
	ImGui::SeparatorText("Specular");
	ImGui::ColorEdit3("##Specular", glm::value_ptr(specular));
}

void Mesh::set_shininess(){
	ImGui::Text("Shininess");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("Shininess: ", &shininess, slider_speed, .0001, 100.0f);
	ImGui::PopItemWidth();
}

void Mesh::UI_get_cursor_position() {
	static int show_cursor = 0;
	ImGuiIO& io = ImGui::GetIO();
	if (ImGui::IsKeyPressed(ImGuiKey_1)) ++show_cursor;
	get_screencoordiantes();
	get_NDC();
	clip_to_worldspace();

	if (show_cursor & 1) {
		//double x_position, y_position;
		//Use glfw for delta values.
		
		ImGui::SeparatorText("Cursor Position");
		ImGui::Text("Cursor Position (%g, %g)", x_position , y_position);
		ImGui::Text("Cursor in Worldspace (%.2f, %.2f, %.2f)", ray_in_worldspace.x, ray_in_worldspace.y, ray_in_worldspace.z);
		ImGui::Text("Cursor in View Space (%.2f, %.2f, %.2f,)", ray_in_eyespace.x, ray_in_eyespace.y, ray_in_eyespace.z);
		ImGui::Text("Cursor Direction (%.2f, %.2f, %.2f)", ray_direction.x, ray_direction.y, ray_direction.z);
		
	}

	if (bounding_box_intersection_test()) {
		ImGui::Text("Intersect Hit");
	}else {
		ImGui::Text("Intersect Miss");
	}
}