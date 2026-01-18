#pragma once
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <map>
//#include <tests/Test.h>

class Control {
private:
	bool isCameraMode;
	bool firstFrame;
	float yaw;
	float pitch;
	//float lastX;
	//float lastY;
	float fov;
	test::Camera& m_camera;
	struct KeyState {
		bool isDown = false; bool wasDown = false;
	};
	std::map<int, KeyState> m_keymap;

	//float deltaTime;
	//float lastFrame;
public:
    Control(GLFWwindow* window, float SCR_WIDTH, float SCR_HEIGHT, test::Camera &camera);
	bool& getCameraMode();
	std::map<int, KeyState>& getKeyMap();
	float getYaw() const;
	float getPitch() const;
	float getFov() const;

	static Control* GetControlInstance(GLFWwindow* window);

    static void handleMouse(GLFWwindow* window, double xpos, double ypos);
	static void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void handleScroll(GLFWwindow* window, double xoffset, double yoffset);
};