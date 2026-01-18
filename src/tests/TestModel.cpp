#include "TestModel.h"

#include <imgui/imgui.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

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
        0.1f }, m_controls(window, SCR_WIDTH, SCR_HEIGHT, m_camera),
        m_window(window), currentIndex(2), modelIndex(0), m_animationTime(0.0f), m_lastTime(glfwGetTime())
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

        float square[] = {
            -0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, //Left Top
            -0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, //Left Bottom
             0.5f, -0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, //Right Bottom
             0.5f,  0.5f,  -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f //Right Top
        };

        unsigned int squareIndices[] = {
            0, 1, 2,
            2, 3, 0
        };

        /*unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));*/

        m_VAO = std::make_unique<VertexArray>();
        m_VAO_Square = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, static_cast<unsigned int>(8 * 4 * 6 * sizeof(float)));
        m_VertexBuffer_Square = std::make_unique<VertexBuffer>(square, static_cast<unsigned int>(8 * 4 * sizeof(float)));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_VAO_Square->AddBuffer(*m_VertexBuffer_Square, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_Shader = std::make_unique<Shader>("res/shaders/Model.Shader");
        m_ShaderLand = std::make_unique<Shader>("res/shaders/Model.Shader");
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.Shader");
        m_ShaderBorder = std::make_unique<Shader>("res/shaders/Border.Shader");
        m_ShaderTransparent = std::make_unique<Shader>("res/shaders/Transparent.Shader");
        m_ShaderSquare = std::make_unique<Shader>("res/shaders/Square.Shader");

        m_Model_Land = std::make_unique<Model>("res/objects/mountain/mount.blend1.obj");
        m_TextureGrass = std::make_unique<Texture>("res/textures/grass.png");
        m_TextureWindow = std::make_unique<Texture>("res/textures/blending_transparent_window.png");

        glfwSetWindowUserPointer(window, &m_controls);
        glfwSetKeyCallback(window, Control::handleKeyboard);
        glfwSetCursorPosCallback(window, Control::handleMouse);
        glfwSetScrollCallback(window, Control::handleScroll);

        //glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        //GLCall(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO));
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::SetWindowSize(ImVec2(480.0f, 250.0f));
    }

    TestModel::~TestModel()
    {
        isPauseClicked = 0;
        glfwSetWindowUserPointer(m_window, nullptr);
    }

    void TestModel::OnUpdate(float curTime)
    {
        float deltaTime = curTime - m_lastTime;
        m_lastTime = curTime;

        if (!test::isPauseClicked) {
            m_animationTime += deltaTime;
        }

        lightPos.x = 6 * std::sin(m_animationTime);
        lightPos.z = 6 * std::cos(m_animationTime);
    }

    void TestModel::OnRender()
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));

        GLCall(glStencilMask(0x00));

        Renderer renderer;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

        glm::vec3 cameraDir{};
        cameraDir.x = cos(glm::radians(m_controls.getYaw())) * cos(glm::radians(m_controls.getPitch()));
        cameraDir.y = sin(glm::radians(m_controls.getPitch()));
        cameraDir.z = sin(glm::radians(m_controls.getYaw())) * cos(glm::radians(m_controls.getPitch()));
        m_camera.Front = glm::normalize(cameraDir);

        if (getControls().getCameraMode())
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        
        ProcessInput(m_camera);
        m_Proj = glm::perspective(glm::radians(m_controls.getFov()), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        m_View = glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up);

        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));

        // Grass Object
        /*
        m_ShaderTransparent->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        m_ShaderTransparent->SetUniformMat4f("u_Model", model);
        m_ShaderTransparent->SetUniformMat4f("u_View", m_View);
        m_ShaderTransparent->SetUniformMat4f("u_Proj", m_Proj);
        m_TextureGrass->Bind(0);
        m_ShaderTransparent->SetUniform1i("u_Texture", 0);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderTransparent);
        
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        m_ShaderTransparent->SetUniformMat4f("u_Model", model);
        renderer.Draw(*m_VAO_Square, *m_IndexBuffer, *m_ShaderTransparent);
        */

        //Window
        /*
        m_ShaderTransparent->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        m_ShaderTransparent->SetUniformMat4f("u_Model", model);
        m_ShaderTransparent->SetUniformMat4f("u_View", m_View);
        m_ShaderTransparent->SetUniformMat4f("u_Proj", m_Proj);
        m_TextureWindow->Bind(0);
        m_ShaderTransparent->SetUniform1i("u_Texture", 0);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderTransparent);

        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        m_ShaderTransparent->SetUniformMat4f("u_Model", model);
        renderer.Draw(*m_VAO_Square, *m_IndexBuffer, *m_ShaderTransparent);
        */

        // Squares
        /*
        m_ShaderSquare->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        m_ShaderSquare->SetUniformMat4f("u_Model", model);
        m_ShaderSquare->SetUniformMat4f("u_View", m_View);
        m_ShaderSquare->SetUniformMat4f("u_Proj", m_Proj);
        m_ShaderSquare->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
        renderer.Draw(*m_VAO_Square, *m_IndexBuffer, *m_ShaderSquare);

        model = glm::translate(model, glm::vec3(-0.25f, -0.25f, 0.01f));
        m_ShaderSquare->SetUniformMat4f("u_Model", model);
        m_ShaderSquare->SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 0.6f);
        renderer.Draw(*m_VAO_Square, *m_IndexBuffer, *m_ShaderSquare);
        */

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

        GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
        GLCall(glStencilMask(0x00));
        GLCall(glDisable(GL_DEPTH_TEST));
        //model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f));
        model = glm::scale(model, glm::vec3(1.1f));
        m_ShaderBorder->Bind();
        m_ShaderBorder->SetUniformMat4f("u_Model", model);
        m_ShaderBorder->SetUniformMat4f("u_View", m_View);
        m_ShaderBorder->SetUniformMat4f("u_Proj", m_Proj);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderBorder);
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));
        GLCall(glEnable(GL_DEPTH_TEST));

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        m_ShaderLand->Bind();
        m_ShaderLand->SetUniformMat4f("u_Model", model);
        m_ShaderLand->SetUniformMat4f("u_View", m_View);
        m_ShaderLand->SetUniformMat4f("u_Proj", m_Proj);
        m_ShaderLand->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
        m_ShaderLand->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_ShaderLand->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_ShaderLand->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_ShaderLand->SetUniform1f("dirLight.intensity", 1.0f);
        m_ShaderLand->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
        m_ShaderLand->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_ShaderLand->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_ShaderLand->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_ShaderLand->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderLand->SetUniform1f("pointLight.linear", 0.09f);
        m_ShaderLand->SetUniform1f("pointLight.quadratic", 0.032f);
        m_ShaderLand->SetUniform3f("viewPos", m_camera.Position);
        m_Model_Land->Draw(*m_ShaderLand);

        switch (modelIndex) {
        case 0:
            break;
        case 1:
            if (!m_Model || m_Model->directory != "res/objects/dice") {
                this->m_Model = std::make_unique<Model>("res/objects/dice/dice.obj");
            }
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.2f));
            //Backpack Object
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniformMat4f("u_View", m_View);
            m_Shader->SetUniformMat4f("u_Proj", m_Proj);
            m_Shader->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
            m_Shader->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("dirLight.intensity", 1.0f);
            m_Shader->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
            m_Shader->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("pointLight.constant", 1.0f);
            m_Shader->SetUniform1f("pointLight.linear", 0.09f);
            m_Shader->SetUniform1f("pointLight.quadratic", 0.032f);
            m_Shader->SetUniform3f("viewPos", m_camera.Position);
            m_Model->Draw(*m_Shader);
            break;
        case 2:
            if (!m_Model || m_Model->directory != "res/objects/gojo") {
                this->m_Model = std::make_unique<Model>("res/objects/gojo/gojo.obj");
            }
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.01f));
            //Backpack Object
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniformMat4f("u_View", m_View);
            m_Shader->SetUniformMat4f("u_Proj", m_Proj);
            m_Shader->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
            m_Shader->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("dirLight.intensity", 1.0f);
            m_Shader->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
            m_Shader->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("pointLight.constant", 1.0f);
            m_Shader->SetUniform1f("pointLight.linear", 0.09f);
            m_Shader->SetUniform1f("pointLight.quadratic", 0.032f);
            m_Shader->SetUniform3f("viewPos", m_camera.Position);
            m_Model->Draw(*m_Shader);
            break;
        case 3:
            if (!m_Model || m_Model->directory != "res/objects/backpack") {
                this->m_Model = std::make_unique<Model>("res/objects/backpack/backpack.obj");
            }
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(1.0f));
            //Backpack Object
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniformMat4f("u_View", m_View);
            m_Shader->SetUniformMat4f("u_Proj", m_Proj);
            m_Shader->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
            m_Shader->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("dirLight.intensity", 1.0f);
            m_Shader->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
            m_Shader->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("pointLight.constant", 1.0f);
            m_Shader->SetUniform1f("pointLight.linear", 0.09f);
            m_Shader->SetUniform1f("pointLight.quadratic", 0.032f);
            m_Shader->SetUniform3f("viewPos", m_camera.Position);
            m_Model->Draw(*m_Shader);
            break;
        case 4:
            if (!m_Model || m_Model->directory != "res/objects/sponza"){
                this->m_Model = std::make_unique<Model>("res/objects/sponza/sponza.obj");
            }
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(0.02f));
            //Backpack Object
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_Model", model);
            m_Shader->SetUniformMat4f("u_View", m_View);
            m_Shader->SetUniformMat4f("u_Proj", m_Proj);
            m_Shader->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
            m_Shader->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("dirLight.intensity", 1.0f);
            m_Shader->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
            m_Shader->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            m_Shader->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            m_Shader->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            m_Shader->SetUniform1f("pointLight.constant", 1.0f);
            m_Shader->SetUniform1f("pointLight.linear", 0.09f);
            m_Shader->SetUniform1f("pointLight.quadratic", 0.032f);
            m_Shader->SetUniform3f("viewPos", m_camera.Position);
            m_Model->Draw(*m_Shader);
            break;
        }

        //Windows
        std::vector<glm::vec3> windows;
        windows.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        windows.push_back(glm::vec3(5.0f, 0.0f, 0.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 2.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 3.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 4.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 5.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 6.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 7.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 8.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 9.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
        windows.push_back(glm::vec3(0.0f, 0.0f, 11.0f));

        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windows.size(); i++) {
            float distance = glm::length(m_camera.Position - windows[i]);
            sorted[distance] = windows[i];
        }

        m_ShaderTransparent->Bind();
        m_ShaderTransparent->SetUniformMat4f("u_View", m_View);
        m_ShaderTransparent->SetUniformMat4f("u_Proj", m_Proj);
        m_TextureWindow->Bind(0);
        m_ShaderTransparent->SetUniform1i("u_Texture", 0);

        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            m_ShaderTransparent->SetUniformMat4f("u_Model", model);
            renderer.Draw(*m_VAO_Square, *m_IndexBuffer, *m_ShaderTransparent);
        }
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        //model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f));
        model = glm::scale(model, glm::vec3(1.1f));
        m_ShaderBorder->Bind();
        m_ShaderBorder->SetUniformMat4f("u_Model", model);
        m_ShaderBorder->SetUniformMat4f("u_View", m_View);
        m_ShaderBorder->SetUniformMat4f("u_Proj", m_Proj);
        //renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderBorder);
        if (m_Model) {
            m_Model->Draw(*m_ShaderBorder);
        }

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
    }

    void TestModel::OnImGuiRender()
    {
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::SliderFloat3("Clear Color", &clearColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Color", &lightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Position", &lightPos.x, -5.0f, 5.0f);
        ImGui::SliderFloat("DirLight Intensity", &dirlightIntensity, 0.0f, 2.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 463.0f));
        ImGui::SetNextWindowPos(ImVec2(0.0f, 462.0f));

        ImGui::Begin("Object Settings");
        if (ImGui::BeginCombo("Model", modelNames[modelIndex])) {
            for (int n = 0; n < IM_ARRAYSIZE(modelNames); n++)
            {
                bool is_selected = (modelIndex == n);
                if (ImGui::Selectable(modelNames[n], is_selected))
                    modelIndex = n;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::End();

        if (isPauseClicked) {
            ImGui::SetNextWindowSize(ImVec2(250.0f, 150.0f));
            ImGui::SetNextWindowPos(ImVec2(650.0f, 350.0f));
            ImGui::Begin("Pause");
            ImGui::Text("Pause");
            ImGui::End();
        }
    }

    void TestModel::ProcessInput(Camera &f_camera) {
        auto& keys = getControls().getKeyMap();
        if (keys[GLFW_KEY_ESCAPE].isDown)
            isEscapeClicked = 1;
        if (keys[GLFW_KEY_P].isDown && !keys[GLFW_KEY_P].wasDown) {
            isPauseClicked = !isPauseClicked;
            getControls().getCameraMode() = !getControls().getCameraMode();
        }

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (keys[GLFW_KEY_W].isDown)
                f_camera.Position += f_camera.Speed * f_camera.Front;
            if (keys[GLFW_KEY_S].isDown)
                f_camera.Position -= f_camera.Speed * f_camera.Front;
            if (keys[GLFW_KEY_A].isDown)
                f_camera.Position -= glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
            if (keys[GLFW_KEY_D].isDown)
                f_camera.Position += glm::normalize(glm::cross(f_camera.Front, f_camera.Up)) * f_camera.Speed;
            if (keys[GLFW_KEY_LEFT_SHIFT].isDown)
                f_camera.Position += f_camera.Speed * f_camera.Up;
            if (keys[GLFW_KEY_Z].isDown)
                f_camera.Position -= f_camera.Speed * f_camera.Up;
        }

        for (auto& [k, st] : keys)
            st.wasDown = st.isDown;
    }
}