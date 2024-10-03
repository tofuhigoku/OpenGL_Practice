#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <time.h>
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
float v2[] = {
8.853842, 104.335716, 3.088814, 0.015846, 0.928716,
8.917645, 102.693520, 2.872913, 0.017191, 0.918737,
9.109365, 104.372154, 2.637386, 0.087131, 0.935688,
9.109365, 104.372154, 2.637386, 0.087131, 0.935688,
8.917645, 102.693520, 2.872913, 0.017191, 0.918737,
9.189501, 102.782356, 2.408039, 0.038272, 0.920157,
};

float vertices[] = {
        // positions            // texture coords
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,
        
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f,      1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,       1.0f, 0.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f,
        0.5f, -0.5f, 0.5f,      1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,       1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,      0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f
};
unsigned int  faces[] = {
    // pos indices          // texture indice
    // 0,  1,  2, 
    // 0,  2,  4, 
    0,  1,  2, 
    0,  2,  3, 

    4,  5,  6, 
    4,  6,  7, 

    6, 1, 2,
    6, 1, 5,

    0, 1, 5,
    0, 4, 5,

    0, 3, 7,
    0, 4, 7,

    2, 3, 7,
    2, 6, 7,

};

float square_test[] = {
    // positions                // texture coords
        -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,     1.0f, 0.0f,
        0.5f, 0.5f, 0.5f,      1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,     0.0f, 1.0f,

         // positions                // texture coords
        -0.5f, -0.5f, -0.5f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,      0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,     1.0f, 0.0f,

};
float square_test2[] = {
    // positions           
        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f, 0.5f, -0.5f,   
        -0.5f, 0.5f, -0.5f, 

// texture coords
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
};
float texture_coords[]= {
     // texture coords
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
        
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
};
float positions[] = {
        // positions           
        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f, 0.5f, -0.5f,   
        -0.5f, 0.5f, -0.5f, 
        
        -0.5f, -0.5f, 0.5f,  
        0.5f, -0.5f, 0.5f,     
        0.5f, 0.5f, 0.5f,     
        -0.5f, 0.5f, 0.5f,     

        -0.5f, 0.5f, 0.5f,     
        -0.5f, 0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, 0.5f,  

        0.5f, 0.5f, 0.5f,   
        0.5f, 0.5f, -0.5f,    
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, 0.5f,   

        -0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,   
        0.5f, -0.5f, 0.5f,   
        -0.5f, -0.5f, 0.5f,   

        -0.5f, 0.5f, -0.5f,  
        0.5f, 0.5f, -0.5f,   
        0.5f, 0.5f, 0.5f,   
        -0.5f, 0.5f, 0.5f,   
};

float cubefaces[] = {
        // positions           // texture coords
        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, // top left

        -0.5f, 0.5f, 0.5f,     0.0f, 1.0f // top left
        -0.5f, -0.5f, 0.5f,    0.0f, 0.0f, // bottom left
        0.5f, -0.5f, 0.5f,     1.0f, 0.0f, // bottom right
        0.5f, 0.5f, 0.5f,      1.0f, 1.0f, // top right


        0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom right
        0.5f, -0.5f, 0.5f,    0.0f, 0.0f, // bottom left
        0.5f, 0.5f, 0.5f,     0.0f, 1.0f, // top left

        // -0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        // -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom right
        // -0.5f, -0.5f, 0.5f,    0.0f, 0.0f, // bottom left
        // -0.5f, 0.5f, 0.5f,     0.0f, 1.0f, // top left

        // 0.5f, 0.5f, -0.5f,      1.0f, 1.0f, // top right
        // 0.5f, 0.5f, 0.5f,     1.0f, 0.0f, // bottom right
        // -0.5f,0.5f, 0.5f,     0.0f, 0.0f, // bottom left
        // -0.5f,0.5f, -0.5f,     0.0f, 1.0f, // top left

        // 0.5f, -0.5f, -0.5f,      1.0f, 1.0f, // top right
        // 0.5f, -0.5f, 0.5f,     1.0f, 0.0f, // bottom right
        // -0.5f,-0.5f, 0.5f,    0.0f, 0.0f, // bottom left
        // -0.5f,-0.5f, -0.5f,     0.0f, 1.0f // top left


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
    glEnable(GL_DEPTH_TEST);

    ShaderClass OwOShader("./include/vertex.vs", "./include/fragment.fs");

    unsigned int VBO , VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(square_test), square_test, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        /* testure init*/
        ImageClass  OwOTexture("OwO2.png", true);
        ImageClass  BvTexture("logo2.png", true);

        OwOShader.use();
        OwOShader.setInt("texture0", 0);
        OwOShader.setInt("texture1", 1);

    // float radian = 0;
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::rotate(trans, glm::radians(radian), glm::vec3(0.0, 0.0, 1.0));
    // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f),glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        // note that we’re translating the scene in the reverse direction
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(OwOShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(OwOShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(OwOShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // glEnable(GL_DEPTH_TEST);
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f, 0.0f, 0.0f),
            glm::vec3( 2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f, 2.0f, -2.5f),
            glm::vec3( 1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    float color[4] = {1.0f, 0.5f, 0.0f, 1.0f};
    while ( !glfwWindowShouldClose(window))
    {
        glClearColor(0,0,0.5, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(Fill_or_Line == 0)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    
        // model = glm::rotate(model, glm::radians(-1.0f),glm::vec3(1.0f, 0.5f, 0.0f));
        // modelLoc = glGetUniformLocation(OwOShader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, OwOTexture.texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, BvTexture.texture);
        OwOShader.use();

        glBindVertexArray(VAO);
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        for (unsigned int i =0; i<10; i++)
        {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, cubePositions[i]);
                    model = glm::rotate(model, glm::radians((float)glfwGetTime()*i),glm::vec3(1.0f, 0.5f, 0.0f));
                    // cout << "cubePositions[i] = " << i << endl;
                    OwOShader.setMat4("model", model);
            modelLoc = glGetUniformLocation(OwOShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            // glDrawArrays(GL_TRIANGLES, 0, 7313);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        }

        // glDrawArrays(GL_POLYGON, 0, 12);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0 );

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