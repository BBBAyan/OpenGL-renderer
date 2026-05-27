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
        0.1f }, m_controls(window, SCR_WIDTH, SCR_HEIGHT, m_camera), draw_cubemap(1), blinn(1), SHADOW_WIDTH(1024), SHADOW_HEIGHT(1024),
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

        float floor[] = {
            -5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //Left Top
            -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //Left Bottom
             5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //Right Bottom
             5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f  //Right Top
        };

        unsigned int floorIndices[] = {
            0, 1, 2,
            0, 2, 3
        };

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

        // pos
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f); glm::vec3 pos2(-1.0f, -1.0f, 0.0f); glm::vec3 pos3(1.0f, -1.0f, 0.0f); glm::vec3 pos4(1.0f, 1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f); glm::vec2 uv2(0.0f, 0.0f); glm::vec2 uv3(1.0f, 0.0f); glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        glm::vec3 edge1 = pos2 - pos1; glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1; glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        edge1 = pos3 - pos1; edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1; deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        float squareVertices[] = {
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };

        unsigned int squareIndices2[] = {
            0, 1, 2,
            3, 4, 5
        };
 
        m_VAO = std::make_unique<VertexArray>();
        m_VAO_Point = std::make_unique<VertexArray>();
        m_VAO_Floor = std::make_unique<VertexArray>();
        m_VAO_Square = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        m_VertexBuffer_Point = std::make_unique<VertexBuffer>(points, sizeof(points));
        m_VertexBuffer_Floor = std::make_unique<VertexBuffer>(floor, sizeof(floor));
        m_VertexBuffer_Square = std::make_unique<VertexBuffer>(squareVertices, sizeof(squareVertices));
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3 * 2 * 6);
        m_IndexBufferFloor = std::make_unique<IndexBuffer>(floorIndices, 3 * 2);
		m_IndexBufferSquare = std::make_unique<IndexBuffer>(squareIndices2, 3 * 2);
        m_Framebuffer = std::make_unique<Framebuffer>(SCR_WIDTH, SCR_HEIGHT);
        m_FramebufferDirShadow = std::make_unique<Framebuffer>(SHADOW_WIDTH, SHADOW_HEIGHT, 0, 1);
        m_FramebufferPointShadow = std::make_unique<Framebuffer>(SHADOW_WIDTH, SHADOW_HEIGHT, 0, 2);
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

        VertexBufferLayout layoutFloor;
        layoutFloor.Push<float>(3);
        layoutFloor.Push<float>(3);
        layoutFloor.Push<float>(2);
        m_VAO_Floor->AddBuffer(*m_VertexBuffer_Floor, layoutFloor);

        VertexBufferLayout layoutSquare;
        layoutSquare.Push<float>(3);
        layoutSquare.Push<float>(3);
        layoutSquare.Push<float>(2);
        layoutSquare.Push<float>(3);
        layoutSquare.Push<float>(3);
        m_VAO_Square->AddBuffer(*m_VertexBuffer_Square, layoutSquare);

        m_ShaderCube = std::make_unique<Shader>("res/shaders/CubeShadow.Shader");
        m_ShaderCubemap = std::make_unique<Shader>("res/shaders/Cubemap.Shader");
        m_ShaderExplode = std::make_unique<Shader>("res/shaders/ModelExplode.Shader");
        m_ShaderFloor = std::make_unique<Shader>("res/shaders/Floor.Shader");
        m_ShaderWall = std::make_unique<Shader>("res/shaders/Wall.Shader");
        m_ShaderWallParallax = std::make_unique<Shader>("res/shaders/WallParallax.Shader");
        m_ShaderFramebuffer = std::make_unique<Shader>("res/shaders/FramebufferScreen.Shader");
        m_ShaderGeometry = std::make_unique<Shader>("res/shaders/Geometry.Shader");
        m_ShaderLight = std::make_unique<Shader>("res/shaders/Lighting.Shader");
        m_ShaderModel = std::make_unique<Shader>("res/shaders/ModelNormalMapping.Shader");
        m_ShaderModelAsteroid = std::make_unique<Shader>("res/shaders/ModelAsteroid.Shader");
        m_ShaderNormal = std::make_unique<Shader>("res/shaders/ModelNormal.Shader");
        m_ShaderReflectiveCube = std::make_unique<Shader>("res/shaders/ReflectiveCube.Shader");
        m_ShaderDirShadow = std::make_unique<Shader>("res/shaders_shadow/DirShadow.Shader");
        m_ShaderPointShadow = std::make_unique<Shader>("res/shaders_shadow/PointShadow.Shader");

        m_Texture = std::make_unique<Texture>("res/textures/container.png", 1);
        m_TextureSpecular = std::make_unique<Texture>("res/textures/container_specular.png", 1);
        m_TextureGrass = std::make_unique<Texture>("res/textures/grass.png", 1);
        m_TextureWindow = std::make_unique<Texture>("res/textures/blending_transparent_window.png", 1);
        m_TextureFloor = std::make_unique<Texture>("res/textures/floor.jpg", 1);
        m_TextureBrick = std::make_unique<Texture>("res/textures/bricks2.jpg", 1);
        m_TextureBrickNormal = std::make_unique<Texture>("res/textures/bricks2_normal.jpg", 1);
		m_TextureBrickDisplacement = std::make_unique<Texture>("res/textures/bricks2_disp.jpg", 1);

        //m_ModelPlanet = std::make_unique<Model>("res/objects/planet/planet.obj");
        m_ModelPlanet = std::make_unique<Model>("res/objects/earth/Earth 2K.obj");
        //m_ModelAsteroid = std::make_unique<Model>("res/objects/rock/rock.obj");
        std::vector<std::string> textures_faces{

        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
        };

        m_TextureCubemap = std::make_unique<TextureCubemap>(textures_faces);

		//Directed Light Shadow Mapping
        float size = 10.0f;
        near_plane = 1.0f; far_plane = 50.0f;
        m_Proj = glm::ortho(-size, size, -size, size, near_plane, far_plane);
        dirLight = glm::vec3(-4.0f, -8.0f, 2.0f);
        m_View = glm::lookAt(-dirLight,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = m_Proj * m_View;

		//Point Light Shadow Mapping
        lightPos = { -4.0f, 6.0f, 6.0f };

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
        boxModels.push_back(model);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, 30.0f, glm::vec3(-1.0f, 3.5f, -1.0f));
        model = glm::translate(model, glm::vec3(-1.0f, 3.5f, 1.0f));
        boxModels.push_back(model);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 1.0f));
        boxModels.push_back(model);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.5f, 3.0f));
        boxModels.push_back(model);

        floorModel = glm::mat4(1.0f);
        floorModel = glm::translate(floorModel, glm::vec3(0.0f, -2.0f, 0.0f));
        floorModel = glm::scale(floorModel, glm::vec3(3.0f));

		normalMatrixFloor = glm::transpose(glm::inverse(glm::mat3(floorModel)));

        wallModel = glm::mat4(1.0f);
		wallModel = glm::translate(wallModel, glm::vec3(-4.0f, 1.0f, -15.0f));
        wallModel = glm::scale(wallModel, glm::vec3(3.0f));

        wallModelParallax = glm::mat4(1.0f);
        wallModelParallax = glm::translate(wallModelParallax, glm::vec3(4.0f, 1.0f, -15.0f));
        wallModelParallax = glm::scale(wallModelParallax, glm::vec3(3.0f));

        objectModel = glm::mat4(1.0f);
        objectModel = glm::translate(objectModel, glm::vec3(0.0f, 3.0f, 0.0f));
        objectModel = glm::scale(objectModel, glm::vec3(0.5f));
		objectModel = glm::rotate(objectModel, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));

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
        GLCall(glFrontFace(GL_CW));
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
        /*
        float deltaTime = curTime - m_lastTime;
        m_lastTime = curTime;

        if (!test::isPauseClicked) {
            m_animationTime += deltaTime;
        }
        lightPos.x = 6 * std::sin(m_animationTime);
        lightPos.z = 6 * std::cos(m_animationTime);
        */
    }

    void TestMain::OnRender()
    {
        GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
        m_FramebufferDirShadow->Bind();
        GLCall(glClear(GL_DEPTH_BUFFER_BIT));
        glEnable(GL_DEPTH_TEST);

        Renderer renderer;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));

        m_ShaderDirShadow->Bind();
        m_ShaderDirShadow->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        for (unsigned int i = 0; i < boxModels.size(); i++) {
            m_ShaderDirShadow->SetUniformMat4f("u_Model", boxModels[i]);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderDirShadow);
        }
        m_FramebufferDirShadow->Unbind();

		m_FramebufferPointShadow->Bind();
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		pointShadowMatrices.clear();
        m_Proj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        pointShadowMatrices.push_back(m_Proj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        pointShadowMatrices.push_back(m_Proj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        pointShadowMatrices.push_back(m_Proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        pointShadowMatrices.push_back(m_Proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        pointShadowMatrices.push_back(m_Proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        pointShadowMatrices.push_back(m_Proj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

        m_ShaderPointShadow->Bind();
	    m_ShaderPointShadow->SetUniformMat4fv("lightSpaceMatrix", pointShadowMatrices.size(), pointShadowMatrices.data());
        m_ShaderPointShadow->SetUniform3f("lightPos", lightPos);
        m_ShaderPointShadow->SetUniform1f("near_plane", near_plane);
        m_ShaderPointShadow->SetUniform1f("far_plane", far_plane);
        for (unsigned int i = 0; i < boxModels.size(); i++) {
            m_ShaderPointShadow->SetUniformMat4f("u_Model", boxModels[i]);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderPointShadow);
        }
		m_ShaderPointShadow->SetUniformMat4f("u_Model", objectModel);
		m_ModelPlanet->Draw(*m_ShaderPointShadow);
		m_FramebufferPointShadow->Unbind();

        //////////////////////////////////////////////////////////////////////////
        GLCall(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));

        m_FramebufferMultisample->Bind();
        GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f));
        //GLCall(glActiveTexture(GL_TEXTURE1));
        //GLCall(glBindTexture(GL_TEXTURE_2D, m_FramebufferShadow->getTextureBuffer()));

        GLCall(glStencilMask(0x00));
		GLCall(glCullFace(GL_BACK));
        //lightPos.y = 5.0f;

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

        //GLCall(glFrontFace(GL_CW));

        // Light Object
        m_ShaderLight->Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightPos));
        model = glm::scale(model, glm::vec3(0.5f));
        m_ShaderLight->SetUniformMat4f("u_Model", model);
        m_ShaderLight->SetUniform3f("pointLightColor", glm::vec3(lightColor.r, lightColor.g, lightColor.b));
        renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderLight);

        m_ShaderCube->Bind();
        m_ShaderCube->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        m_TextureFloor->Bind(0);
        m_ShaderCube->SetUniform1i("material.diffuse", 0);
		GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_FramebufferDirShadow->getTextureBuffer()));
        m_ShaderCube->SetUniform1i("shadowMap", 1);
        GLCall(glActiveTexture(GL_TEXTURE2));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_FramebufferPointShadow->getTextureBuffer()));
        m_ShaderCube->SetUniform1i("pointDepthMap", 2);
        m_ShaderCube->SetUniform1f("material.shininess", shininess);
        m_ShaderCube->SetUniform3f("dirLight.direction", glm::normalize(dirLight));
        m_ShaderCube->SetUniform3f("dirLight.ambient", glm::vec3(0.05f));
        m_ShaderCube->SetUniform3f("dirLight.diffuse", glm::vec3(0.3f));
        m_ShaderCube->SetUniform3f("dirLight.specular", glm::vec3(0.3f));
        m_ShaderCube->SetUniform1f("dirLight.intensity", dirlightIntensity);
        m_ShaderCube->SetUniform3f("pointLight.position", glm::vec3(lightPos));
        m_ShaderCube->SetUniform3f("pointLight.ambient", glm::vec3(0.05f));
        m_ShaderCube->SetUniform3f("pointLight.diffuse", glm::vec3(1.0f));
        m_ShaderCube->SetUniform3f("pointLight.specular", glm::vec3(0.5f));
        m_ShaderCube->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderCube->SetUniform1f("pointLight.linear", 0.02f);
        m_ShaderCube->SetUniform1f("pointLight.quadratic", 0.005f);
        m_ShaderCube->SetUniform1f("far_plane", far_plane);
        m_ShaderCube->SetUniform1f("near_plane", near_plane);
        for (unsigned int i = 0; i < boxModels.size(); i++) {
            m_ShaderCube->SetUniformMat4f("u_Model", boxModels[i]);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_ShaderCube);
        }

        GLCall(glDisable(GL_CULL_FACE));
        //Floor
        m_ShaderFloor->Bind();
        m_ShaderFloor->SetUniformMat4f("u_Model", floorModel);
        m_ShaderFloor->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		m_ShaderFloor->SetUniformMat3f("normalMatrix", normalMatrixFloor);
        m_ShaderFloor->SetUniform3f("viewPos", glm::vec3(m_camera.Position));
        m_TextureFloor->Bind(0);
        m_ShaderFloor->SetUniform1i("material.diffuse", 0);
        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_FramebufferDirShadow->getTextureBuffer()));
        m_ShaderFloor->SetUniform1i("shadowMap", 1);
		GLCall(glActiveTexture(GL_TEXTURE2));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_FramebufferPointShadow->getTextureBuffer()));
        m_ShaderFloor->SetUniform1i("pointDepthMap", 2);
        m_ShaderFloor->SetUniform1f("material.shininess", shininess);
        m_ShaderFloor->SetUniform3f("dirLight.direction", glm::normalize(dirLight));
        m_ShaderFloor->SetUniform3f("dirLight.ambient", glm::vec3(0.05f));
        m_ShaderFloor->SetUniform3f("dirLight.diffuse", glm::vec3(0.3f));
        m_ShaderFloor->SetUniform3f("dirLight.specular", glm::vec3(0.3f));
        m_ShaderFloor->SetUniform1f("dirLight.intensity", dirlightIntensity);
        m_ShaderFloor->SetUniform3f("pointLight.position", lightPos);
        m_ShaderFloor->SetUniform3f("pointLight.ambient", glm::vec3(0.05f));
        m_ShaderFloor->SetUniform3f("pointLight.diffuse", glm::vec3(1.0f));
        m_ShaderFloor->SetUniform3f("pointLight.specular", glm::vec3(0.5f));
        m_ShaderFloor->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderFloor->SetUniform1f("pointLight.linear", 0.005f);
        m_ShaderFloor->SetUniform1f("pointLight.quadratic", 0.0005f);
        m_ShaderFloor->SetUniform1i("blinn", blinn);
        m_ShaderFloor->SetUniform1f("far_plane", far_plane);
        m_ShaderFloor->SetUniform1f("near_plane", near_plane);
        m_ShaderFloor->SetUniform1f("repeatFactor", 4.0f);
        renderer.Draw(*m_VAO_Floor, *m_IndexBufferFloor, *m_ShaderFloor);

		m_ShaderWall->Bind();
		m_ShaderWall->SetUniformMat4f("u_Model", wallModel);
        m_ShaderWall->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        m_ShaderWall->SetUniform3f("lightPos", lightPos);
        m_ShaderWall->SetUniform3f("lightDir", glm::normalize(dirLight));
        m_ShaderWall->SetUniform3f("viewPos", m_camera.Position);
        m_TextureBrick->Bind(0);
        m_ShaderWall->SetUniform1i("material.diffuse", 0);
        m_TextureBrickNormal->Bind(1);
        m_ShaderWall->SetUniform1i("material.normal", 1);
        GLCall(glActiveTexture(GL_TEXTURE2));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_FramebufferDirShadow->getTextureBuffer()));
        m_ShaderWall->SetUniform1i("shadowMap", 2);
        GLCall(glActiveTexture(GL_TEXTURE3));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_FramebufferPointShadow->getTextureBuffer()));
        m_ShaderWall->SetUniform1i("pointDepthMap", 3);
        m_ShaderWall->SetUniform1f("material.shininess", shininess);
        m_ShaderWall->SetUniform3f("dirLight.ambient", glm::vec3(0.05f));
        m_ShaderWall->SetUniform3f("dirLight.diffuse", glm::vec3(0.3f));
        m_ShaderWall->SetUniform3f("dirLight.specular", glm::vec3(0.3f));
        m_ShaderWall->SetUniform1f("dirLight.intensity", dirlightIntensity);
        m_ShaderWall->SetUniform3f("pointLight.ambient", glm::vec3(0.05f));
        m_ShaderWall->SetUniform3f("pointLight.diffuse", glm::vec3(1.0f));
        m_ShaderWall->SetUniform3f("pointLight.specular", glm::vec3(0.5f));
        m_ShaderWall->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderWall->SetUniform1f("pointLight.linear", 0.005f);
        m_ShaderWall->SetUniform1f("pointLight.quadratic", 0.0005f);
        m_ShaderWall->SetUniform1i("blinn", blinn);
        m_ShaderWall->SetUniform1f("far_plane", far_plane);
        m_ShaderWall->SetUniform1f("near_plane", near_plane);
        m_ShaderWall->SetUniform1f("repeatFactor", 1.0f);
        renderer.Draw(*m_VAO_Square, *m_IndexBufferSquare, *m_ShaderWall);

        m_ShaderWallParallax->Bind();
        m_ShaderWallParallax->SetUniformMat4f("u_Model", wallModelParallax);
        m_ShaderWallParallax->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        m_ShaderWallParallax->SetUniform3f("lightPos", lightPos);
        m_ShaderWallParallax->SetUniform3f("lightDir", glm::normalize(dirLight));
        m_ShaderWallParallax->SetUniform3f("viewPos", m_camera.Position);
        m_TextureBrick->Bind(0);
        m_ShaderWallParallax->SetUniform1i("material.diffuse", 0);
        m_TextureBrickNormal->Bind(1);
        m_ShaderWallParallax->SetUniform1i("material.normal", 1);
        m_TextureBrickDisplacement->Bind(2);
        m_ShaderWallParallax->SetUniform1i("material.displacement", 2);
        GLCall(glActiveTexture(GL_TEXTURE3));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_FramebufferDirShadow->getTextureBuffer()));
        m_ShaderWallParallax->SetUniform1i("shadowMap", 3);
        GLCall(glActiveTexture(GL_TEXTURE4));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_FramebufferPointShadow->getTextureBuffer()));
        m_ShaderWallParallax->SetUniform1i("pointDepthMap", 4);
        m_ShaderWallParallax->SetUniform1f("material.shininess", shininess);
        m_ShaderWallParallax->SetUniform3f("dirLight.ambient", glm::vec3(0.05f));
        m_ShaderWallParallax->SetUniform3f("dirLight.diffuse", glm::vec3(0.3f));
        m_ShaderWallParallax->SetUniform3f("dirLight.specular", glm::vec3(0.3f));
        m_ShaderWallParallax->SetUniform1f("dirLight.intensity", dirlightIntensity);
        m_ShaderWallParallax->SetUniform3f("pointLight.ambient", glm::vec3(0.05f));
        m_ShaderWallParallax->SetUniform3f("pointLight.diffuse", glm::vec3(1.0f));
        m_ShaderWallParallax->SetUniform3f("pointLight.specular", glm::vec3(0.5f));
        m_ShaderWallParallax->SetUniform1f("pointLight.constant", 1.0f);
        m_ShaderWallParallax->SetUniform1f("pointLight.linear", 0.005f);
        m_ShaderWallParallax->SetUniform1f("pointLight.quadratic", 0.0005f);
        m_ShaderWallParallax->SetUniform1i("blinn", blinn);
        m_ShaderWallParallax->SetUniform1f("far_plane", far_plane);
        m_ShaderWallParallax->SetUniform1f("near_plane", near_plane);
        m_ShaderWallParallax->SetUniform1f("repeatFactor", 1.0f);
        renderer.Draw(*m_VAO_Square, *m_IndexBufferSquare, *m_ShaderWallParallax);

		m_ShaderModel->Bind();
        m_ShaderModel->SetUniformMat4f("u_Model", objectModel);
        m_ShaderModel->SetUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
        m_ShaderModel->SetUniform3f("viewPos", m_camera.Position);
        m_ShaderModel->SetUniform3f("lightPos", lightPos);
        m_ShaderModel->SetUniform3f("lightDir", glm::normalize(dirLight));
        GLCall(glActiveTexture(GL_TEXTURE10));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_FramebufferDirShadow->getTextureBuffer()));
        m_ShaderModel->SetUniform1i("shadowMap", 10);
        GLCall(glActiveTexture(GL_TEXTURE11));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_FramebufferPointShadow->getTextureBuffer()));
        m_ShaderModel->SetUniform1i("pointDepthMap", 11);
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
        m_ShaderModel->SetUniform1f("far_plane", far_plane);
        m_ShaderModel->SetUniform1f("near_plane", near_plane);
		m_ModelPlanet->Draw(*m_ShaderModel);

        GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
        GLCall(glStencilMask(0xFF));

        //skybox
        if (draw_cubemap == true)
        {
            m_ShaderCubemap->Bind();
            glm::mat4 cubemapView = glm::mat4(glm::mat3(glm::lookAt(m_camera.Position, m_camera.Position + m_camera.Front, m_camera.Up)));
            m_ShaderCubemap->SetUniformMat4f("u_Proj", m_Proj);
            m_ShaderCubemap->SetUniformMat4f("u_View", cubemapView);
            GLCall(glBindVertexArray(cubemapVAO));
            m_TextureCubemap->Bind(1);
            m_ShaderCubemap->SetUniform1i("skybox", 1);
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        }

        GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FramebufferMultisample->getFBO()));
        GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer->getFBO()));
        GLCall(glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST));

        m_FramebufferMultisample->Unbind();

        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCall(glDisable(GL_DEPTH_TEST));

        m_ShaderFramebuffer->Bind();
        m_ShaderFramebuffer->SetUniform1i("screenTexture", 0);
        m_ShaderFramebuffer->SetUniform1f("near_plane", near_plane);
        m_ShaderFramebuffer->SetUniform1f("far_plane", far_plane);
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_Framebuffer->getTextureBuffer()));
        GLCall(glEnable(GL_FRAMEBUFFER_SRGB));
        m_Framebuffer->Draw();
        GLCall(glDisable(GL_FRAMEBUFFER_SRGB));
        GLCall(glBindVertexArray(0));
    }

    void TestMain::OnImGuiRender()
    {
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("Camera Pos: (%.1f, %.1f, %.1f)", m_camera.Position.x, m_camera.Position.y, m_camera.Position.z);
        ImGui::SliderFloat3("Clear Color", &clearColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Color", &lightColor.r, 0.0f, 1.0f);
        ImGui::SliderFloat3("Light Position", &lightPos.x, -5.0f, 5.0f);
        ImGui::SliderFloat("DirLight Intensity", &dirlightIntensity, 0.0f, 2.0f);
        ImGui::Checkbox("Draw a Cubemap", &draw_cubemap);
        ImGui::Checkbox("1 - Blinn Phong, 0 - Regular Phong", &blinn);
        ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f);
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