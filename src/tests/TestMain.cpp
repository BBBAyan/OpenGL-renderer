#include "TestMain.h"

#include <imgui/imgui.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

namespace test
{
    TestMain::TestMain(GLFWwindow* window)
        : m_Proj(glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f)),
        m_Proj2D(glm::ortho(-SCR_WIDTH / SCR_HEIGHT, SCR_WIDTH / SCR_HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f)),
        m_View(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f))),
        m_camera{ glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        0.1f }, m_controls(window, SCR_WIDTH, SCR_HEIGHT, m_camera), draw_cubemap(0), draw_house(0), draw_quads(0),
        m_window(window), currentIndex(2), modelIndex(0), m_animationTime(0.0f), m_lastTime(float(glfwGetTime()))
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
                       0.5f,  0.5f,  -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f  //Top Front				
        };

        unsigned int indices[]{
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            11, 10, 9,
            11, 9, 8,
            13, 12, 15,
            13, 15, 14,
            16, 17, 18,
            16, 18, 19,
            23, 22, 21,
            23, 21, 20
        };
        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        float points[] = {
            -0.25f,  0.25f, 1.0f, 0.0f, 0.0f, //top left
             0.25f,  0.25f, 0.0f, 1.0f, 0.0f, //top right
             0.25f, -0.25f, 0.0f, 0.0f, 1.0f, //bottom right
            -0.25f, -0.25f, 1.0f, 1.0f, 0.0f  //bottom left
        };

        float quadVertices[] = {
            // positions     // color
            -0.05f,  0.05f, 1.0f, 0.0f, 0.0f,
            -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,
             0.05f, -0.05f, 0.0f, 1.0f, 0.0f,

             0.05f,  0.05f, 1.0f, 1.0f, 0.0f,
            -0.05f,  0.05f, 1.0f, 0.0f, 0.0f,
             0.05f, -0.05f, 0.0f, 1.0f, 0.0f
        };

        glm::vec2 translations[100];
        int index = 0;
        float offset = 0.1f;
        for (int i = -10; i < 10; i += 2) {
            for (int j = -10; j < 10; j += 2) {
                glm::vec2 translation = glm::vec2(-i * offset, 0.1f + j * offset);
                translations[index++] = translation;
            }
        }
        GLCall(glGenBuffers(1, &instanceVBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        planetPos = glm::vec3(15.0f, 0.0f, 0.0f);
        rockAmount = 100000;
        modelMatrices = new glm::mat4[rockAmount];
        srand(glfwGetTime());
        float radius = 50.0;
        float height = 8.0f;
        for (unsigned int i = 0; i < rockAmount; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            /*
            float angle = rand() % 360;
            float distance = rand() % 4 + 7.0f;
            float distance_x = cos(angle) * distance;
            float distance_y = (rand() % int(height * 100)) / 100.0f;
            float distance_z = sin(angle) * distance;
            model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f) + glm::vec3(distance_x, distance_y, distance_z));
            */
            float angle = (float)i / (float)rockAmount * 360.0f;
            float displacement = (rand() % (int)(2 * height * 100)) / 100.0f - height;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * height * 100)) / 100.0f - height;
            float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
            displacement = (rand() % (int)(2 * height * 100)) / 100.0f - height;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, planetPos + glm::vec3(x, y, z));
            float scale = (rand() % 20) / 100 + 0.05;
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            float rotAngle = rand() % 360;
            model = glm::rotate(model, rotAngle, glm::vec3(0.3f, 0.5f, 0.2f));

            modelMatrices[i] = model;
        }

        GLCall(glGenVertexArrays(1, &cubemapVAO));
        GLCall(glGenBuffers(1, &cubemapVBO));

        GLCall(glBindVertexArray(cubemapVAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

        GLCall(glGenBuffers(1, &ubo));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
        GLCall(glBufferData(GL_UNIFORM_BUFFER, 128, NULL, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo));

        m_VAO = std::make_unique<VertexArray>();
        m_VAO_Point = std::make_unique<VertexArray>();
        m_VAO_Quad = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        m_VertexBuffer_Point = std::make_unique<VertexBuffer>(points, sizeof(points));
        m_VertexBuffer_Quad = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_Framebuffer = std::make_unique<Framebuffer>(SCR_WIDTH, SCR_HEIGHT);
        m_FramebufferMultisample = std::make_unique<Framebuffer>(SCR_WIDTH, SCR_HEIGHT, 4);

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        VertexBufferLayout layoutPoint;
        layoutPoint.Push<float>(2);
        layoutPoint.Push<float>(3);
        m_VAO_Point->AddBuffer(*m_VertexBuffer_Point, layoutPoint);

        VertexBufferLayout layoutQuad;
        layoutQuad.Push<float>(2);
        layoutQuad.Push<float>(3);
        m_VAO_Quad->AddBuffer(*m_VertexBuffer_Quad, layoutQuad);

        GLCall(glEnableVertexAttribArray(1));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glVertexAttribDivisor(1, 1));

        m_ShaderCubemap = std::make_unique<Shader>("res/shaders/Cubemap.Shader");
        m_ShaderExplode = std::make_unique<Shader>("res/shaders/ModelExplode.Shader");
        m_ShaderFramebuffer = std::make_unique<Shader>("res/shaders/FramebufferScreen.Shader");
        m_ShaderGeometry = std::make_unique<Shader>("res/shaders/Geometry.Shader");
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.Shader");
        m_ShaderModel = std::make_unique<Shader>("res/shaders/Model.Shader");
        m_ShaderModelAsteroid = std::make_unique<Shader>("res/shaders/ModelAsteroid.Shader");
        m_ShaderNormal = std::make_unique<Shader>("res/shaders/ModelNormal.Shader");
        m_ShaderReflectiveCube = std::make_unique<Shader>("res/shaders/ReflectiveCube.Shader");
        m_ShaderQuad = std::make_unique<Shader>("res/shaders/Quad.Shader");

        m_Texture = std::make_unique<Texture>("res/textures/container.png");
        m_TextureSpecular = std::make_unique<Texture>("res/textures/container_specular.png");
        m_TextureGrass = std::make_unique<Texture>("res/textures/grass.png");
        m_TextureWindow = std::make_unique<Texture>("res/textures/blending_transparent_window.png");

        m_ModelPlanet = std::make_unique<Model>("res/objects/planet/planet.obj");
        m_ModelAsteroid = std::make_unique<Model>("res/objects/rock/rock.obj");

        GLCall(glGenBuffers(1, &instanceRockVBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceRockVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * rockAmount, &modelMatrices[0], GL_STATIC_DRAW));
        for (unsigned int i = 0; i < m_ModelAsteroid->meshes.size(); i++) {
            unsigned int VAO = m_ModelAsteroid->meshes[i].VAO;
            glBindVertexArray(VAO);
            std::size_t vec4Size = sizeof(glm::vec4);
            glEnableVertexAttribArray(3);
            GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0));
            glEnableVertexAttribArray(4);
            GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size)));
            glEnableVertexAttribArray(5);
            GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size)));
            glEnableVertexAttribArray(6);
            GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size)));
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }

        std::vector<std::string> textures_faces{
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
        };

        m_TextureCubemap = std::make_unique<TextureCubemap>(textures_faces);

        glfwSetWindowUserPointer(window, &m_controls);
        glfwSetKeyCallback(window, Control::handleKeyboard);
        glfwSetCursorPosCallback(window, Control::handleMouse);
        glfwSetScrollCallback(window, Control::handleScroll);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glEnable(GL_MULTISAMPLE));
        GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
        GLCall(glEnable(GL_BLEND));
        GLCall(glDepthFunc(GL_LEQUAL));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        ImGui::SetWindowSize(ImVec2(480.0f, 250.0f));
    }

    TestMain::~TestMain()
    {
        isPauseClicked = 0;
        glfwSetWindowUserPointer(m_window, nullptr);
        //glfwSetCursorPosCallback(m_window, nullptr);
        glfwSetKeyCallback(m_window, nullptr);
        glfwSetScrollCallback(m_window, nullptr);

        glUseProgram(0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
        GLCall(glDisable(GL_MULTISAMPLE));
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
    }

    void TestMain::OnUpdate(float curTime)
    {
        float deltaTime = curTime - m_lastTime;
        m_lastTime = curTime;

        if (!test::isPauseClicked) {
            m_animationTime += deltaTime;
        }
        lightPos.x = 6 * std::sin(m_animationTime);
        lightPos.z = 6 * std::cos(m_animationTime);
    }

    void TestMain::OnRender()
    {
        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));
        m_FramebufferMultisample->Bind();

        GLCall(glEnable(GL_DEPTH_TEST));

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

        if (getControls().getCameraMode()) {
            GLCall(glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
        }
        else {
            GLCall(glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
        }

        ProcessInput(m_camera);
        m_Proj = glm::perspective(glm::radians(m_controls.getFov()), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
        m_View = glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up);

        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, ubo));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, &m_Proj));
        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, &m_View));
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glFrontFace(GL_CW));

        //Reflective Cube
        m_ShaderReflectiveCube->Bind();
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
        m_ShaderReflectiveCube->SetUniformMat4f("u_Model", model);
        m_ShaderReflectiveCube->SetUniform3f("viewPos", m_camera.Position);
        m_TextureCubemap->Bind(0);
        m_ShaderReflectiveCube->SetUniform1i("skybox", 0);
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderReflectiveCube);

        // Light Object
        m_ShaderLight->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightPos.x, 0.0f, lightPos.z));
        model = glm::scale(model, glm::vec3(0.5f));
        m_ShaderLight->SetUniformMat4f("u_Model", model);
        m_ShaderLight->SetUniform3f("pointLightColor", glm::vec3(lightColor.r, lightColor.g, lightColor.b));
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);

        GLCall(glDisable(GL_CULL_FACE));
        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));

        //Geometry
        if (draw_house)
        {
            m_ShaderGeometry->Bind();
            model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
            m_VAO_Point->Bind();
            m_VertexBuffer_Point->Bind();
            GLCall(glDrawArrays(GL_POINTS, 0, 4));
        }

        //100 quads
        if (draw_quads) {
            m_ShaderQuad->Bind();
            m_ShaderQuad->SetUniformMat4f("u_Proj", m_Proj2D);
            m_VAO_Quad->Bind();
            m_VertexBuffer_Quad->Bind();
            GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
        }

        //Planet
        m_ShaderModel->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, planetPos);
        model = glm::scale(model, glm::vec3(3.0f));
        m_ShaderModel->SetUniformMat4f("u_Model", model);
        m_ShaderModel->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
        m_ShaderModel->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_ShaderModel->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_ShaderModel->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_ShaderModel->SetUniform1f("dirLight.intensity", 1.0f);
        m_ShaderModel->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
        m_ShaderModel->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_ShaderModel->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_ShaderModel->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_ShaderModel->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderModel->SetUniform1f("pointLight.linear", 0.09f);
        m_ShaderModel->SetUniform1f("pointLight.quadratic", 0.032f);
        m_ShaderModel->SetUniform3f("viewPos", m_camera.Position);
        m_ModelPlanet->Draw(*m_ShaderModel);

        //Asteroid
        m_ShaderModelAsteroid->Bind();
        m_ShaderModelAsteroid->SetUniform3f("dirLight.direction", glm::normalize(glm::vec3(0.1f, -1.0f, 0.0f)));
        m_ShaderModelAsteroid->SetUniform3f("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_ShaderModelAsteroid->SetUniform3f("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_ShaderModelAsteroid->SetUniform3f("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_ShaderModelAsteroid->SetUniform1f("dirLight.intensity", 1.0f);
        m_ShaderModelAsteroid->SetUniform3f("pointLight.position", glm::vec3(lightPos.x, lightPos.y, lightPos.z));
        m_ShaderModelAsteroid->SetUniform3f("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        m_ShaderModelAsteroid->SetUniform3f("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        m_ShaderModelAsteroid->SetUniform3f("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        m_ShaderModelAsteroid->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderModelAsteroid->SetUniform1f("pointLight.linear", 0.09f);
        m_ShaderModelAsteroid->SetUniform1f("pointLight.quadratic", 0.032f);
        for (unsigned int i = 0; i < m_ModelAsteroid->meshes.size(); i++) {
            m_ShaderModelAsteroid->SetUniform3f("viewPos", m_camera.Position);
            glBindVertexArray(m_ModelAsteroid->meshes[i].VAO);
            GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_ModelAsteroid->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, rockAmount));
        }

        //skybox
        if (draw_cubemap == true)
        {
            m_ShaderCubemap->Bind();
            glm::mat4 cubemapView = glm::mat4(glm::mat3(glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up)));
            m_ShaderCubemap->SetUniformMat4f("u_Proj", m_Proj);
            m_ShaderCubemap->SetUniformMat4f("u_View", cubemapView);
            GLCall(glBindVertexArray(cubemapVAO));
            m_TextureCubemap->Bind(0);
            m_ShaderCubemap->SetUniform1i("skybox", 0);
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        }

        GLCall(glDisable(GL_DEPTH_TEST));

        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferMultisample->getFBO()))
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer->getFBO()));
        GLCall(glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST));

        m_FramebufferMultisample->Unbind();
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        m_ShaderFramebuffer->Bind();
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->getTextureColorBuffer()));
        m_Framebuffer->Draw();
        GLCall(glBindVertexArray(0));
    }

    void TestMain::OnImGuiRender()
    {
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::SliderFloat3("Clear Color", &clearColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Color", &lightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Position", &lightPos.x, -5.0f, 5.0f);
        ImGui::SliderFloat("DirLight Intensity", &dirlightIntensity, 0.0f, 2.0f);
        ImGui::Checkbox("Draw a Cubemap", &draw_cubemap);
        ImGui::Checkbox("Draw a House", &draw_house);
        ImGui::Checkbox("Draw 100 Quads", &draw_quads);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        if (isPauseClicked) {
            ImGui::SetNextWindowSize(ImVec2(250.0f, 150.0f));
            ImGui::SetNextWindowPos(ImVec2(650.0f, 350.0f));
            ImGui::Begin("Pause");
            ImGui::Text("Pause");
            ImGui::End();
        }
    }

    void TestMain::ProcessInput(Camera& f_camera) {
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