#include "tests/Test.h"
#include "Control.h"
#include <imgui/imgui.h>
#include <iostream>


Control::Control(GLFWwindow* window, float SCR_WIDTH, float SCR_HEIGHT, test::Camera& camera)
    : firstFrame(1), yaw(-90.0f), pitch(0.0f)/*,
    lastX(SCR_WIDTH / 2), lastY(SCR_HEIGHT / 2)*/, fov(45.0f), m_camera(camera), isCameraMode(true)
    //deltaTime(0.0f), lastFrame(0.0f)
{
    glfwSetCursorPos(window, (double)SCR_WIDTH / 2, (double)SCR_HEIGHT / 2);
}

std::map<int, Control::KeyState>& Control::getKeyMap()
{
    return m_keymap;
}

bool& Control::getCameraMode()
{
    return isCameraMode;
}

float Control::getYaw() const
{
    return yaw;
}

float Control::getPitch() const
{
    return pitch;
}

float Control::getFov() const
{
    return fov;
}

Control* Control::GetControlInstance(GLFWwindow* window)
{
    return static_cast<Control*>(glfwGetWindowUserPointer(window));
}

void Control::handleMouse(GLFWwindow* window, double xpos, double ypos)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent((float)xpos, (float)ypos);


    Control* instance = GetControlInstance(window);
    if (!instance) {
        //std::cout << "Control::handleMouse: no instance pointer" << std::endl;
        return;
    }


    if (ImGui::GetIO().WantCaptureMouse || !instance->isCameraMode) {
        return;
    }

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetCursorPos(window, (double)width / 2, (double)height / 2);

    float xoffset = xpos - width/2;
    float yoffset = height/2 - ypos;
    //instance->lastX = xpos;
    //instance->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    instance->yaw += xoffset;
    instance->pitch += yoffset;

    if (instance->pitch > 89.0f)
        instance->pitch = 89.0f;
    if (instance->pitch < -89.0f)
        instance->pitch = -89.0f;

}

void Control::handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Control* instance = GetControlInstance(window);
    if (!instance) {
        std::cout << "Control::handleKeyboard: no instance pointer" << std::endl;
        return;
    }

    if (action == GLFW_PRESS) {
        instance->m_keymap[key].isDown = true;
    }
    else if (action == GLFW_RELEASE)
        instance->m_keymap[key].isDown = false;
}

void Control::handleScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    Control* instance = GetControlInstance(window);
    if (!instance) {
        std::cout << "Control::handleScroll: no instance pointer" << std::endl;
        return;
    }

    instance->fov -= (float)yoffset;
    if (instance->fov < 1.0f)
        instance->fov = 1.0f;
    if (instance->fov > 45.0f)
        instance->fov = 45.0f;
}
