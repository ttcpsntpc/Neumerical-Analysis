#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Object.h"
#include "Shader.h"
#include "vertex.h"
#include "texture.h"
#include "camera.h"
#include "read_file.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <math.h>
#include <vector>
#include <algorithm>
#include <thread>
#include <random>

using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);



int CreateVoxelTexture();
int CreateTransferFunctionTexture();
int UpdateTransferFunctionTexture(float color[5][3]);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
Camera_c camera(glm::vec3(100.0f, 150.0f, 600.0f));
glm::vec3 lightPos(500.0f, 500.0f, 500.0f);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

ReadFile_c rf("../../raw/vase01");

glm::ivec2 voxelIndex(101, 101);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style   // USELESS
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader_c lightShader("shader/lightShader.vs", "shader/lightShader.fs");
    Shader_c lightCube("shader/lightCube.vs", "shader/lightCube.fs");

    vertex.CreateVertices();
    Object_c square_red;
    square_red.CreateObject(vertices[0], {});
    Object_c square_blue;
    square_blue.CreateObject(vertices[1], {});
    Object_c cube;
    cube.CreateObject(vertices[2], {});
    Object_c axis;  
    axis.CreateObject(vertices[3], {});
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::NewFrame();
        ImGui::Begin("Read voxel");
        ImGui::Text("Eye position: (%.1f, %.1f, %.1f)", camera.Position.x, camera.Position.y, camera.Position.z);
        ImGui::Text("Light position: (%.1f, %.1f, %.1f)", lightPos.x, lightPos.y, lightPos.z);
        if(ImGui::Button("Transfer function"))
        {
        }
        ImGui::End();
        // ImGui::ShowDemoWindow();

        // create model matrix
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // create view matrix
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        // create projection matrix
        glm::mat4 projection = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

        // active shader for shading
        lightShader.use();

        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        lightShader.setVec3("lightPos", lightPos);
        lightShader.setVec3("viewPos", camera.Position);

        glBindVertexArray(cube.VAO_);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 10.0f, 10.0f));
        model = glm::scale(model, glm::vec3(100, 100, 100));
        lightCube.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, cube.size);


        // active shader for background
        lightCube.use();
        lightCube.setVec3("lightPos", lightPos);
        lightCube.setMat4("model", model);
        lightCube.setMat4("view", view);
        lightCube.setMat4("projection", projection);

        glBindVertexArray(cube.VAO_);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        lightCube.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, cube.size);

        glBindVertexArray(axis.VAO_);
        model = glm::mat4(1.0f);
        lightCube.setMat4("model", model);
        glDrawArrays(GL_LINES, 0, axis.size);

        // ImGui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    static bool moveObject = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        moveObject = !moveObject;

    if(moveObject == 0)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FRONT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACK, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            lightPos.z -= 100.0f * deltaTime; 
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            lightPos.z += 100.0f * deltaTime; 
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            lightPos.x -= 100.0f * deltaTime; 
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            lightPos.x += 100.0f * deltaTime; 
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            lightPos.y += 100.0f * deltaTime; 
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
            lightPos.y -= 100.0f * deltaTime; 
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.ProcessKeyboard(PITCHUP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera.ProcessKeyboard(PITCHDOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera.ProcessKeyboard(YAWLEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        camera.ProcessKeyboard(YAWRIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int CreateVoxelTexture()
{
    int x = rf.inf_data.data_resolution[0]; 
    int y = rf.inf_data.data_resolution[1];
    int z = rf.inf_data.data_resolution[2]; 

    unsigned char *texture_data = new unsigned char[x * y * z * 4];

    for(int k = 0; k < z; k++)         
        for(int j = 0; j < y; j++)
            for(int i = 0; i < x; i++)  
            {
                int idx = (k * y * x + j * x + i) * 4;

                // gradient x
                if(i == 0)
                    texture_data[idx + 0] = (rf.raw_data.voxel_data[k][j][i + 1] - rf.raw_data.voxel_data[k][j][i]) / rf.inf_data.voxel_size[0];
                else if(i == x - 1)
                    texture_data[idx + 0] = (rf.raw_data.voxel_data[k][j][i] - rf.raw_data.voxel_data[k][j][i - 1]) / rf.inf_data.voxel_size[0];
                else 
                    texture_data[idx + 0] = (rf.raw_data.voxel_data[k][j][i + 1] - rf.raw_data.voxel_data[k][j][i - 1]) / (2 * rf.inf_data.voxel_size[0]);

                // gradient y
                if(j == 0)
                    texture_data[idx + 1] = (rf.raw_data.voxel_data[k][j + 1][i] - rf.raw_data.voxel_data[k][j][i]) / rf.inf_data.voxel_size[1];
                else if(j == y - 1)
                    texture_data[idx + 1] = (rf.raw_data.voxel_data[k][j][i] - rf.raw_data.voxel_data[k][j - 1][i]) / rf.inf_data.voxel_size[1];
                else 
                    texture_data[idx + 1] = (rf.raw_data.voxel_data[k][j + 1][i] - rf.raw_data.voxel_data[k][j - 1][i]) / (2 * rf.inf_data.voxel_size[1]);

                // gradient z 
                if(k == 0)
                    texture_data[idx + 2] = (rf.raw_data.voxel_data[k + 1][j][i] - rf.raw_data.voxel_data[k][j][i]) / rf.inf_data.voxel_size[2];
                else if(k == z - 1)
                    texture_data[idx + 2] = (rf.raw_data.voxel_data[k][j][i] - rf.raw_data.voxel_data[k - 1][j][i]) / rf.inf_data.voxel_size[2];
                else 
                    texture_data[idx + 2] = (rf.raw_data.voxel_data[k + 1][j][i] - rf.raw_data.voxel_data[k - 1][j][i]) / (2 * rf.inf_data.voxel_size[2]);

                // intensity
                texture_data[idx + 3] = rf.raw_data.voxel_data[k][j][i];
            }

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_3D, texID);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 注意這裡 x 和 z 換位後也要改
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, x, y, z, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

    return texID;
}


int CreateTransferFunctionTexture()
{
    // Create a transfer function texture
    const int tf_size = 256;
    unsigned char transfer_function[tf_size][4];

    for (int i = 0; i < tf_size; ++i)
    {
        transfer_function[i][3] = 2; // Alpha
        if(i < 40) // Low intensity
        {
            transfer_function[i][0] = 0; // Red
            transfer_function[i][1] = 0; // Green
            transfer_function[i][2] = 0; // Blue
            transfer_function[i][3] = 0; // Alpha
        }
        else if(i < 80)
        {
            transfer_function[i][0] = (i - 40) / 40.0f * 255; // Red
            transfer_function[i][1] = 20; // Green
            transfer_function[i][2] = 20; // Blue
        }
        else if(i < 120)
        {
            transfer_function[i][0] = 255; // Red
            transfer_function[i][1] = (i - 80) / 40.0f * 255; // Green
            transfer_function[i][2] = 20; // Blue
        }
        else if(i < 160)
        {
            transfer_function[i][0] = 255 - (i - 120) / 40.0f * 255; // Red
            transfer_function[i][1] = 255; // Green
            transfer_function[i][2] = 20; // Blue
        }
        else if(i < 200)
        {
            transfer_function[i][0] = 20; // Red
            transfer_function[i][1] = 255; // Green
            transfer_function[i][2] = (i - 160) / 40.0f * 255; // Blue
        }
        else 
        {
            transfer_function[i][0] = 20; // Red
            transfer_function[i][1] = 255 - (i - 160) / 40.0f * 255; // Green
            transfer_function[i][2] = 255; // Blue
        }
    }

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_1D, texID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // upload the 1D texture data
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, tf_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, transfer_function);

    return texID;
}

int UpdateTransferFunctionTexture(float color[5][3])
{
    const int tf_size = 256;
    unsigned char transfer_function[tf_size][4];

    for (int i = 0; i < tf_size; ++i)
    {
        transfer_function[i][3] = 2; // Alpha
        if(i < 40) // Low intensity
        {
            transfer_function[i][0] = 0; // Red
            transfer_function[i][1] = 0; // Green
            transfer_function[i][2] = 0; // Blue
            transfer_function[i][3] = 0; // Alpha
        }
        else if(i < 80)
        {
            transfer_function[i][0] = static_cast<unsigned char>(color[0][0] * 255.0f); // Red
            transfer_function[i][1] = static_cast<unsigned char>(color[0][1] * 255.0f); // Green
            transfer_function[i][2] = static_cast<unsigned char>(color[0][2] * 255.0f); // Blue
        }
        else if(i < 120)
        {
            transfer_function[i][0] = static_cast<unsigned char>(color[1][0] * 255.0f); // Red
            transfer_function[i][1] = static_cast<unsigned char>(color[1][1] * 255.0f); // Green
            transfer_function[i][2] = static_cast<unsigned char>(color[1][2] * 255.0f); // Blue
        }
        else if(i < 160)
        {
            transfer_function[i][0] = static_cast<unsigned char>(color[2][0] * 255.0f); // Red
            transfer_function[i][1] = static_cast<unsigned char>(color[2][1] * 255.0f); // Green
            transfer_function[i][2] = static_cast<unsigned char>(color[2][2] * 255.0f); // Blue
        }
        else if(i < 200)
        {
            transfer_function[i][0] = static_cast<unsigned char>(color[3][0] * 255.0f); // Red
            transfer_function[i][1] = static_cast<unsigned char>(color[3][1] * 255.0f); // Green
            transfer_function[i][2] = static_cast<unsigned char>(color[3][2] * 255.0f); // Blue
        }
        else 
        {
            transfer_function[i][0] = static_cast<unsigned char>(color[4][0] * 255.0f); // Red
            transfer_function[i][1] = static_cast<unsigned char>(color[4][1] * 255.0f); // Green
            transfer_function[i][2] = static_cast<unsigned char>(color[4][2] * 255.0f); // Blue
        }
    }

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_1D, texID);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // upload the 1D texture data
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, tf_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, transfer_function);

    return texID;
}