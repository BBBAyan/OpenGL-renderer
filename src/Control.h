#pragma once
#include <GLFW/glfw3.h>

class Control {
private:
	bool firstFrame;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;

	//float deltaTime;
	//float lastFrame;
public:
    Control(float SCR_WIDTH, float SCR_HEIGHT);
	float getYaw() const;
	float getPitch() const;
	float getFov() const;

	static Control* GetControlInstance(GLFWwindow* window);

    static void handleMouse(GLFWwindow* window, double xpos, double ypos);
	static void handleScroll(GLFWwindow* window, double xoffset, double yoffset);
};