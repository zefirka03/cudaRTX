#include "AIR_RTX.h"

#include "someFunctions.h"

Object3D obj; 
Render win(1000, 1000, "RTX");
Quad q(52, 52);
Camera cm;
bool fl = true;
bool cam_movement_update = true;
int quality = 1;
int render_type = RTX_RENDER_NORMALS;

void solve_console() {
	std::string com;
	while (std::cin >> com) {
		if (com == "size") {
			int _x, _y;
			std::cin >> com;
			_x = toInt(com);
			std::cin >> com;
			_y = toInt(com);
			std::cout << _x << " " << _y << '\n';
			RTX_Init(_x, _y, obj);
			q.setSize(_x, _y);
			fl = true;
		}
		else if (com == "pos") {
			double _x, _y, _z;
			std::cin >> com;
			_x = toDouble(com);
			std::cin >> com;
			_y = toDouble(com);
			std::cin >> com;
			_z = toDouble(com);
			cm.cam_pos = glm::vec3(_x,_y,_z);
			fl = true;
		}
		else if (com == "render") {
			std::cin >> com;
			if (com == "normals") render_type = RTX_RENDER_NORMALS;
			else if (com == "color") render_type = RTX_RENDER_COLOR;
			fl = true;
		}
		else if (com == "update") {
			std::cin >> com;
			if (com == "true") cam_movement_update = true;
			else if (com == "false") cam_movement_update = false;
		}
		else if (com == "qual") {
			std::cin >> com;
			quality = toInt(com);
			fl = true;
		}
		else if (com == "save_ppm") {
			savePPM("", q.getSize().y, q.getSize().x, q.getBuffer());
		}
		else {
			std::cout << "No such command\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

int main() {
	glfwSetInputMode(win.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetInputMode(win.getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	std::thread console_th(solve_console);
	console_th.detach();

	obj.loadFromObj("objects/Uchit.obj");

	Quad::init();
	
	RTX_Init(52, 52, obj);
	int speed = 10;
	while (!win.ShouldClose()) {
		win.clear();

		if (cam_movement_update) {
			if (cm.update(win.getGLFWWindow())) fl = true;
			if (glfwGetKey(win.getGLFWWindow(), GLFW_KEY_W)) { cm.cam_pos += cm.cam_front*glm::vec3(speed); fl = true; }
			if (glfwGetKey(win.getGLFWWindow(), GLFW_KEY_S)) { cm.cam_pos -= cm.cam_front*glm::vec3(speed); fl = true; }
			if (glfwGetKey(win.getGLFWWindow(), GLFW_KEY_D)) { cm.cam_pos += glm::cross(cm.cam_front, cm.cam_up)*glm::vec3(speed); fl = true; }
			if (glfwGetKey(win.getGLFWWindow(), GLFW_KEY_A)) { cm.cam_pos -= glm::cross(cm.cam_front, cm.cam_up)*glm::vec3(speed); fl = true; }
		}
		
		if (fl) {
			q.setTexture(RTXSolveScene(cm, render_type, quality));
			fl = false;
		}
		q.draw();

		win.display();
	}

	return 0;
}
