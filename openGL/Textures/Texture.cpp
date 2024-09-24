#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "ImageClass.h"
#include "ShaderClass.h"

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;


uint8_t Fill_or_Line = 0xff;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
// if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(key == GLFW_KEY_TAB && action ==GLFW_PRESS) 
        Fill_or_Line = ~Fill_or_Line;
}

float vertices[] = {
        // positions        // colors           // texture coords
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f // top left
};


int main(int argc, char** argv)
{
    (void)(argc);
    (void)(argv);
    int ret =0;
    
    if(glfwInit() == GLFW_FALSE){return 0;}
    cout << "glfwInit return GLFW_TRUE"<< endl;

    GLFWwindow * window = glfwCreateWindow(800,800, "OwO", NULL, NULL);
    if(window == NULL){glfwTerminate(); return 0; }
    cout << "glfwCreateWindow: OK"<< endl;

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if(glewInit() != GLEW_OK){ glfwTerminate(); return 0;}
    cout << "glewInit: GLEW_OK"<< endl;

    glViewport(0, 0, 800, 800);

    ShaderClass OwOShader("./include/vertex.vs", "./include/fragment.fs");

    unsigned int VBO , VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        /* testure init*/
        ImageClass  OwOTexture("OwO2.png", true);
        ImageClass  BvTexture("logo2.png", true);

        OwOShader.use();
        OwOShader.setInt("texture0", 0);
        OwOShader.setInt("texture1", 1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float color[4] = {1.0f, 0.5f, 0.0f, 1.0f};
    while ( !glfwWindowShouldClose(window))
    {
        glClearColor(0,0,0.5, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        if(Fill_or_Line == 0)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    /* Chapter6: shader change uniform color */
        // float color[4] = {1.0f, 0.5f, 0.0f, 1.0f};
        // float timeValue = glfwGetTime();
        // color[0] = sin(timeValue) / 2.0f + 0.0f;
        // color[1] = cos(timeValue) / 2.0f + 0.5f;
        // color[2] = -sin(timeValue) / 2.0f + 0.2f;

        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        // glUniform4fv(vertexColorLocation, 1, color);
    
    
    /**/

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, OwOTexture.texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, BvTexture.texture);
        OwOShader.use();

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glDrawArrays(GL_POLYGON, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return ret;
}