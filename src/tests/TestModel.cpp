#include "TestModel.h"

#include <imgui/imgui.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace test
{
    TestModel::TestModel(GLFWwindow* window)
        : m_Proj(glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f)),
        m_View(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f))),
        m_camera{ glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.1f }, m_controls(Control(SCR_WIDTH, SCR_HEIGHT)),
        m_window(window)
    {
        float positions[] = {
            //position             //normals           //texCoord
           //front
             -0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, //Left Top
             -0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //Left Bottom
              0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, //Right Bottom
              0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, //Right Top

           //top
             -0.5f,  0.5f,   0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //Left Back
             -0.5f,  0.5f,  -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, //Left Front
              0.5f,  0.5f,  -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, //Right Front
              0.5f,  0.5f,   0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, //Right Back

           //back
             -0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, //Left Top
             -0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, //Left Bottom
              0.5f, -0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, //Right Bottom
              0.5f,  0.5f,   0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, //Right Top

           //bottom
             -0.5f, -0.5f,   0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, //Left Back
             -0.5f, -0.5f,  -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, //Left Front
              0.5f, -0.5f,  -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, //Right Front
              0.5f, -0.5f,   0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, //Right Back

           //left
             -0.5f,  0.5f,   0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //Top Back
             -0.5f, -0.5f,   0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //Bottom Back
             -0.5f, -0.5f,  -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //Bottom Front
             -0.5f,  0.5f,  -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, //Top Front

           //right
              0.5f,  0.5f,   0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, //Top Back
              0.5f, -0.5f,   0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, //Bottom Back
              0.5f, -0.5f,  -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, //Bottom Front
              0.5f,  0.5f,  -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f //Top Front
        };

        unsigned int indices[]{
            0, 1, 2,
            0, 3, 2,
            4, 5, 6,
            4, 7, 6,
            8, 9, 10,
            8, 11, 10,
            12, 13, 14,
            12, 15, 14,
            16, 17, 18,
            16, 19, 18,
            20, 21, 22,
            20, 23, 22
        };

        // Light Object
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 4 * 6 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.shader");

        // Backpack Object
        //m_Model = std::make_unique<Model>("res/objects/city/uploads_files_2720101_BusGameMap.obj");
        m_Model = std::make_unique<Model>("res/objects/backpack/backpack.obj");
        m_Shader = std::make_unique<Shader>("res/shaders/Model.Shader");

        glfwSetWindowUserPointer(window, &m_controls);
        glfwSetCursorPosCallback(window, Control::handleMouse);
        glfwSetScrollCallback(window, Control::handleScroll);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GLCall(glEnable(GL_DEPTH_TEST));
        ImGui::SetWindowSize(ImVec2(480.0f, 250.0f));
    }

    TestModel::~TestModel()
    {
        glfwSetWindowUserPointer(m_window, nullptr);
    }

    void TestModel::OnUpdate(float deltaTime)
    {
    }

    void TestModel::OnRender()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        //clearColor = { 0.1f, 0.1f, 0.1f };
        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));

        Renderer renderer;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));


        glm::vec3 cameraDir{};
        cameraDir.x = cos(glm::radians(m_controls.getYaw())) * cos(glm::radians(m_controls.getPitch()));
        cameraDir.y = sin(glm::radians(m_controls.getPitch()));
        cameraDir.z = sin(glm::radians(m_controls.getYaw())) * cos(glm::radians(m_controls.getPitch()));
        m_camera.Front = glm::normalize(cameraDir);
        
        TestModel::m_camera.Position = ProcessInput(m_camera).Position;
        m_Proj = glm::perspective(glm::radians(m_controls.getFov()), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        m_View = glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up);

        //lightPos change;
        lightPos.x = 6 * std::sin(glfwGetTime());
        lightPos.z = 6 * std::cos(glfwGetTime());

        //Backpack Object
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Proj", m_Proj);
        m_Shader->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
        m_Shader->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_Shader->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_Shader->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_Shader->SetUniform1f("pointLight.constant", 1.0f);
        m_Shader->SetUniform1f("pointLight.linear", 0.09f);
        m_Shader->SetUniform1f("pointLight.quadratic", 0.032f);
        m_Shader->SetUniform3f("viewPos", m_camera.Position);
        //m_Shader->SetUniform3f("");
        m_Model->Draw(*m_Shader);

        // Light Object
        m_ShaderLight->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightPos.x, 0.0f, lightPos.z));
        model = glm::scale(model, glm::vec3(0.5f));
        m_ShaderLight->SetUniformMat4f("u_Model", model);
        m_ShaderLight->SetUniformMat4f("u_View", m_View);
        m_ShaderLight->SetUniformMat4f("u_Proj", m_Proj);
        m_ShaderLight->SetUniform3f("pointLightColor", glm::vec3(lightColor.r, lightColor.g, lightColor.b));

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);
    }

    void TestModel::OnImGuiRender()
    {
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::SliderFloat3("Clear Color", &clearColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Color", &lightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Position", &lightPos.x, -5.0f, 5.0f);
        ImGui::SliderFloat("DirLight Intensity", &dirlightIntensity, 0.0f, 2.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    test::Camera TestModel::ProcessInput(Camera f_camera) {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            isEscapeClicked = 1;

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            f_camera.Position += f_camera.Speed * f_camera.Front;
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            f_camera.Position -= f_camera.Speed * f_camera.Front;
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            f_camera.Position -= glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            f_camera.Position += glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            f_camera.Position += f_camera.Speed * f_camera.Up;
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            f_camera.Position -= f_camera.Speed * f_camera.Up;

        return f_camera;
    }

    /*void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstFrame)
        {
            lastX = xpos;
            lastY = ypos;
            firstFrame = false;
        }

        if (!ImGui::GetIO().WantCaptureMouse) {

            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;


            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }
        else {
            lastX = xpos;
            lastY = ypos;
        }
    }

    void TestModel::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
    void TestModel::StaticMouseCallback(GLFWwindow* window, double xpos, double ypos) {
        TestModel* instance = static_cast<TestModel*>(glfwGetWindowUserPointer(window));
        if (instance) {
            instance->HandleMouseMovement(xpos, ypos);
        }
    }

    // Static Scroll Callback Implementation (The "Receptionist")
    void TestModel::StaticScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        TestModel* instance = static_cast<TestModel*>(glfwGetWindowUserPointer(window));
        if (instance) {
            instance->HandleMouseScroll(xoffset, yoffset);
        }
    }

    // Non-static (Instance) Mouse Handler (The actual "Office Desk Phone" logic)
    void TestModel::HandleMouseMovement(double xpos, double ypos) {
        if (firstFrame) { // Use member m_firstFrame
            lastX = (float)xpos; // Use member m_lastX
            lastY = (float)ypos; // Use member m_lastY
            firstFrame = false;
        }

        if (!ImGui::GetIO().WantCaptureMouse) {
            float xoffset = (float)xpos - lastX;
            float yoffset = lastY - (float)ypos; // Reversed for typical camera controls
            lastX = (float)xpos;
            lastY = (float)ypos;

            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;   // Use member m_yaw
            pitch += yoffset; // Use member m_pitch

            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            // Update camera's Front vector based on new yaw and pitch
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            this->camera.Front = glm::normalize(front); // Update member camera
        }
        else {
            // If ImGui wants the mouse, we might still want to update lastX/lastY
            // to avoid a jump when ImGui releases mouse focus.
            lastX = (float)xpos;
            lastY = (float)ypos;
        }
    }

    // Non-static (Instance) Scroll Handler
    void TestModel::HandleMouseScroll(double xoffset, double yoffset) {
        if (!ImGui::GetIO().WantCaptureMouse) { // Good to check here too
            fov -= (float)yoffset; // Use member m_fov
            if (fov < 1.0f) fov = 1.0f;
            if (fov > 45.0f) fov = 45.0f; // Or a wider range if desired

            // Update projection matrix if FoV changes
            m_Proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        }
    }
    */
}