#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include "ShaderClass.h"

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
// positions // colors
0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
};


int main(int argc, char** argv)
{
    int ret =0;
    
    // ShaderClass OwOShader("/home/ms/openGL/Shader/vertex.vs", "/home/ms/openGL/Shader/fragment.fs");

    if(glfwInit() == GLFW_FALSE){return 0;}
    cout << "glfwInit return GLFW_TRUE"<< endl;

    GLFWwindow * window = glfwCreateWindow(800, 600, "OwO", NULL, NULL);
    if(window == NULL){glfwTerminate(); return 0; }
    cout << "glfwCreateWindow: OK"<< endl;

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    if(glewInit() != GLEW_OK){ glfwTerminate(); return 0;}
    cout << "glewInit: GLEW_OK"<< endl;

    glViewport(0, 0, 800, 600);

    ShaderClass OwOShader("/home/ms/openGL/Shader/vertex.vs", "/home/ms/openGL/Shader/fragment.fs");

    unsigned int VBO , VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glUseProgram(shaderProgram);
    // glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

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

        // glUseProgram(shaderProgram);
        OwOShader.use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glDeleteProgram(shaderProgram);
    glfwTerminate();

    return ret;
}