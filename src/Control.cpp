#include "Control.h"
#include <imgui/imgui.h>
#include <iostream>

Control::Control(float SCR_WIDTH, float SCR_HEIGHT)
    : firstFrame(1), yaw(-90.0f), pitch(0.0f),
    lastX(SCR_WIDTH / 2.0), lastY(SCR_HEIGHT / 2.0), fov(45.0f)
    //deltaTime(0.0f), lastFrame(0.0f)
{
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
    Control* instance = GetControlInstance(window);
    if (!instance) {
        std::cout << "Control::handleMouse: no instance pointer" << std::endl;
        return;
    }

    if (instance->firstFrame)
    {
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstFrame = false;
    }

    if (!ImGui::GetIO().WantCaptureMouse) {

        float xoffset = xpos - instance->lastX;
        float yoffset = instance->lastY - ypos;
        instance->lastX = xpos;
        instance->lastY = ypos;


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
    else {
        instance->lastX = xpos;
        instance->lastY = ypos;
    }
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
