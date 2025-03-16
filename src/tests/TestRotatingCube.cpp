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

    float dirlightIntensity = 1.0f;

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

        float land[] = {
            -1.0f, -0.5f, -1.0f, 0.0f,  0.5f,  0.0f, 0.0f, 0.0f, //Left Back
             1.0f, -0.5f, -1.0f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f, //Right Back
             1.0f, -0.5f,  1.0f, 0.0f,  0.5f,  0.0f, 1.0f, 1.0f, //Right Front
            -1.0f, -0.5f,  1.0f, 0.0f,  0.5f,  0.0f, 0.0f, 1.0f  //Left Front
        };

        unsigned int landIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        m_VAO = std::make_unique<VertexArray>();
        m_VAO_Land = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 4 * 6 * sizeof(float));
        m_VertexBuffer_Land = std::make_unique<VertexBuffer>(land, 8 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        VertexBufferLayout layoutLand;
        layoutLand.Push<float>(3);
        layoutLand.Push<float>(3);
        layoutLand.Push<float>(2);
        m_VAO_Land->AddBuffer(*m_VertexBuffer_Land, layoutLand);

        TestRotatingCube::window = window;
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_Shader = std::make_unique<Shader>("res/shaders/Cube.shader");
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.shader");
        m_Texture = std::make_unique<Texture>("res/textures/container.png");
        m_Texture_Specular = std::make_unique<Texture>("res/textures/container_specular.png");
        m_Texture_Matrix = std::make_unique<Texture>("res/textures/matrix.jpg");
        m_Texture_Light = std::make_unique<Texture>("res/textures/glowstone.jpg");

        m_IndexBuffer_Land = std::make_unique<IndexBuffer>(landIndices, 3 * 2);
        m_Shader_Land = std::make_unique<Shader>("res/shaders/Land.shader");
        m_Texture_Land = std::make_unique<Texture>("res/textures/dirt.jpg");
        m_Texture_Spotlight = std::make_unique<Texture>("res/textures/smile.png");
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GLCall(glEnable(GL_DEPTH_TEST));

        ImGui::SetWindowSize(ImVec2(480.0f, 250.0f));
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
        //clearColor = { 0.1f, 0.1f, 0.1f };
        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));

        Renderer renderer;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 model_Light = glm::mat4(1.0f);
        glm::mat4 model_Land = glm::mat4(1.0f);

        glm::vec3 lightPos = glm::vec3(5.0f, 5.0f, 2.0f);

        model_Light = glm::translate(model_Light, glm::vec3(3.0f, 6.0f, -3.0f));
        model_Light = glm::scale(model_Light, glm::vec3(1.0f));
        model_Land = glm::translate(model_Land, glm::vec3(0.0f, -3.0f, 0.0f));
        model_Land = glm::scale(model_Land, glm::vec3(10.0f));

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

        glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3(0.0f,  0.0f, -3.0f)
        };

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Proj", m_Proj);
        m_Shader->SetUniform3f("viewPos", camera.Position);
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("material.diffuse", 0);
        m_Texture_Specular->Bind(1);
        m_Shader->SetUniform1i("material.specular", 1);
        m_Shader->SetUniform1f("material.shininess", shininess);
        m_Shader->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
        m_Shader->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_Shader->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_Shader->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_Shader->SetUniform1f("dirLight.intensity", dirlightIntensity);
        m_Shader->SetUniform3f("pointLightColor", glm::vec3(lightColor.r, lightColor.g, lightColor.b));
        for (int i = 0; i < 4; i++) {
            m_Shader->SetUniform3f("pointLight[" + std::to_string(i) + "].position", pointLightPositions[i]);
            m_Shader->SetUniform3f("pointLight[" + std::to_string(i) + "].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("pointLight[" + std::to_string(i) + "].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("pointLight[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("pointLight[" + std::to_string(i) + "].constant", 1.0f);
            m_Shader->SetUniform1f("pointLight[" + std::to_string(i) + "].linear", 0.09f);
            m_Shader->SetUniform1f("pointLight[" + std::to_string(i) + "].quadratic", 0.032f);
        }
        m_Shader->SetUniform3f("spotLight.position", camera.Position);
        m_Shader->SetUniform3f("spotLight.direction", camera.Front);
        m_Shader->SetUniform1f("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        m_Shader->SetUniform1f("spotLight.outercutOff", glm::cos(glm::radians(15.0f)));
        m_Shader->SetUniform3f("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_Shader->SetUniform3f("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_Shader->SetUniform3f("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_Shader->SetUniform1f("spotLight.constant", 1.0f);
        m_Shader->SetUniform1f("spotLight.linear", 0.09f);
        m_Shader->SetUniform1f("spotLight.quadratic", 0.032f);

        for (unsigned int i = 0; i < 4; i++) {
            m_ShaderLight->Bind();
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f));
            m_ShaderLight->SetUniformMat4f("u_Model", model);
            m_ShaderLight->SetUniformMat4f("u_View", m_View);
            m_ShaderLight->SetUniformMat4f("u_Proj", m_Proj);
            m_ShaderLight->SetUniform3f("pointLightColor", glm::vec3(lightColor.r, lightColor.g, lightColor.b));

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);
        }

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f,  0.0f,  0.0f),
            glm::vec3(2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f,  2.0f, -2.5f),
            glm::vec3(1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniform3f("viewPos", camera.Position);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        m_Shader_Land->Bind();
        m_Shader_Land->SetUniformMat4f("u_Model", model_Land);
        m_Shader_Land->SetUniformMat4f("u_View", m_View);
        m_Shader_Land->SetUniformMat4f("u_Proj", m_Proj);
        m_Shader_Land->SetUniform3f("viewPos", camera.Position);
        m_Shader_Land->SetUniform3f("light.position", lightPos);
        m_Shader_Land->SetUniform3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_Shader_Land->SetUniform3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_Shader_Land->SetUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_Texture_Land->Bind(3);
        m_Shader_Land->SetUniform1i("u_Texture", 3);

        renderer.Draw(*m_VAO_Land, *m_IndexBuffer_Land, *m_Shader_Land);
    }

    void TestRotatingCube::OnImGuiRender()
    {
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::SliderFloat3("Clear Color", &clearColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Color", &lightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat("DirLight Intensity", &dirlightIntensity, 0.0f, 2.0f);
        ImGui::Text("Desert BgColor: (0.75, 0.55, 0.3); LColor: (1.0, 1.0, 1.0)");
        ImGui::Text("Factory Color: (0.1, 0.1, 0.15); LColor: (0.2, 0.2, 0.6)");
        ImGui::Text("Horror Color: (0.6, 0.5, 0.2); LColor: (x.x, x.x, x.x)");
        ImGui::Text("Biochemical Lab Color: (0.6, 0.5, 0.2); LColor: (x.x, x.x, x.x)");
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

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        fov -= (float)yoffset;
        if (fov < 1.0f)
            fov = 1.0f;
        if (fov > 45.0f)
            fov = 45.0f;
    }
}