#include "TestRotatingCube.h"

#include <imgui/imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const unsigned int SCR_WIDTH = 1290;
const unsigned int SCR_HEIGHT = 913;

namespace test
{
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    bool firstFrame = 1;
    float yaw = -90.0f, pitch = 0.0f;
    float lastX = SCR_WIDTH / 2.0;
    float lastY = SCR_HEIGHT / 2.0;
    float fov = 45.0f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    TestRotatingCube::TestRotatingCube(GLFWwindow* window)
        : m_Proj(glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)),
        m_View(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f))),
        camera{ glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.1f }
    {
        float positions[] = {
            //front
              -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.5f, 0.0f, 1.0f, //Left Top
              -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.5f, 0.0f, 0.0f, //Left Bottom
               0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.5f, 1.0f, 0.0f, //Right Bottom
               0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  0.5f, 1.0f, 1.0f, //Right Top

            //top
              -0.5f,  0.5f,  0.5f,  0.0f,  0.5f,  0.0f, 0.0f, 1.0f, //Left Back
              -0.5f,  0.5f, -0.5f,  0.0f,  0.5f,  0.0f, 0.0f, 0.0f, //Left Front
               0.5f,  0.5f, -0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 0.0f, //Right Front
               0.5f,  0.5f,  0.5f,  0.0f,  0.5f,  0.0f, 1.0f, 1.0f, //Right Back

            //back
              -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -0.5f, 0.0f, 1.0f, //Left Top
              -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -0.5f, 0.0f, 0.0f, //Left Bottom
               0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -0.5f, 1.0f, 0.0f, //Right Bottom
               0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -0.5f, 1.0f, 1.0f, //Right Top

            //bottom
              -0.5f, -0.5f,  0.5f,  0.0f, -0.5f, 0.0f, 0.0f, 1.0f, //Left Back
              -0.5f, -0.5f, -0.5f,  0.0f, -0.5f, 0.0f, 0.0f, 0.0f, //Left Front
               0.5f, -0.5f, -0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 0.0f, //Right Front
               0.5f, -0.5f,  0.5f,  0.0f, -0.5f, 0.0f, 1.0f, 1.0f, //Right Back

            //left
              -0.5f,  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, //Top Back
              -0.5f, -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 0.0f, 0.0f, //Bottom Back
              -0.5f, -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f, //Bottom Front
              -0.5f,  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 1.0f, //Top Front

            //right
               0.5f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, //Top Back
               0.5f, -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 0.0f, 0.0f, //Bottom Back
               0.5f, -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, //Bottom Front
               0.5f,  0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f  //Top Front
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

        TestRotatingCube::window = window;
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_Shader = std::make_unique<Shader>("res/shaders/Cube.shader");
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.shader");
        m_Texture = std::make_unique<Texture>("res/textures/container.png");
        m_Texture_Specular = std::make_unique<Texture>("res/textures/container_specular.png");
        m_Texture_Matrix = std::make_unique<Texture>("res/textures/matrix.jpg");

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    TestRotatingCube::~TestRotatingCube()
    {
    }

    void TestRotatingCube::OnUpdate(float deltaTime)
    {
    }

    void TestRotatingCube::OnRender()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCall(glClearColor(0.2f, 0.2f, 0.1f, 1.0f));

        Renderer renderer;
        struct Light {
            glm::vec3 position;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };
        struct Light light;
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model_light = glm::mat4(1.0f);

        light.position = glm::vec3(1.0f, 1.0f, 1.0f);

        model_light = glm::translate(model_light, light.position);
        model_light = glm::scale(model_light, glm::vec3(0.2f));

        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        light.diffuse = lightColor * glm::vec3(0.5f);
        light.ambient = light.diffuse * glm::vec3(0.5f);
        light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::vec3 objectColor = glm::vec3(0.5f, 0.2f, 0.0f);

        glm::vec3 cameraDir{};
        cameraDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraDir.y = sin(glm::radians(pitch));
        cameraDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera.Front = glm::normalize(cameraDir);

        float shininess = 51.2f;

        float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

        TestRotatingCube::camera.Position = ProcessInput(camera).Position;
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        m_Proj = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        m_View = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Proj", m_Proj);
        m_Shader->SetUniform3f("viewPos", camera.Position);
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("material.diffuse", 0);
        m_Texture_Specular->Bind(1);
        m_Shader->SetUniform1i("material.specular", 1);
        m_Texture_Matrix->Bind(2);
        m_Shader->SetUniform1i("material.emission", 2);
        m_Shader->SetUniform1f("time", glfwGetTime());
        m_Shader->SetUniform1f("material.shininess", shininess);
        m_Shader->SetUniform3f("light.position", light.position);
        m_Shader->SetUniform3f("light.ambient", light.ambient);
        m_Shader->SetUniform3f("light.diffuse", light.diffuse);
        m_Shader->SetUniform3f("light.specular", light.specular);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

        m_ShaderLight->Bind();
        m_ShaderLight->SetUniformMat4f("u_Model", model_light);
        m_ShaderLight->SetUniformMat4f("u_View", m_View);
        m_ShaderLight->SetUniformMat4f("u_Proj", m_Proj);

        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);
    }

    void TestRotatingCube::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    test::Camera TestRotatingCube::ProcessInput(Camera f_camera) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            isEscapeClicked = 1;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            f_camera.Position += f_camera.Speed * f_camera.Front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            f_camera.Position -= f_camera.Speed * f_camera.Front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            f_camera.Position -= glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            f_camera.Position += glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            f_camera.Position += f_camera.Speed * f_camera.Up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            f_camera.Position -= f_camera.Speed * f_camera.Up;

        return f_camera;
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (firstFrame)
        {
            lastX = xpos;
            lastY = ypos;
            firstFrame = false;
        }

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

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
}