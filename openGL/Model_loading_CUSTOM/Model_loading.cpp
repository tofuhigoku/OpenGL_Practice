#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include "ImageClass.h"
#include "ShaderClass.h"
#include "Mesh.h"
#include "ModelClass.h"
#include "Obj3DClass.h"

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float yaw = 0.0f, pitch = 0.0f;


bool firstMouse = true;
bool mouse_left_pressed = false;
float lastX =400.0f, lastY = 400.0f;

float Zoom = 45.0f;


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

    // float currentFrame = glfwGetTime();
    // deltaTime = currentFrame - lastFrame;
    // lastFrame = currentFrame;
    // const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     cameraPos += cameraSpeed * cameraFront;
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     cameraPos -= cameraSpeed * cameraFront;
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_CursorPositioncallback(GLFWwindow* window, double xpos, double ypos)
{
    if(mouse_left_pressed)
    {
        if (firstMouse)
        {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        }
        else
        {
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;
            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;
            yaw += xoffset;
            pitch += yoffset;
            if(pitch > 89.0f)
            pitch = 89.0f;
            if(pitch < -89.0f)
            pitch = -89.0f;
            glm::vec3 direction;
            direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            direction.y = sin(glm::radians(pitch));
            direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(direction);
        }
    }
}

void mouse_CursorClickcallback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        // cout << "time: " << (int) glfwGetTime() << endl;
        firstMouse = true;
        mouse_left_pressed = true;


    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {

        // cout << "time: " << (int) glfwGetTime() << endl;
        firstMouse = true;
        mouse_left_pressed = false;


    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


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
    glfwSetCursorPosCallback(window, mouse_CursorPositioncallback);
    glfwSetMouseButtonCallback(window, mouse_CursorClickcallback);
    glfwSetScrollCallback(window, scroll_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(glewInit() != GLEW_OK){ glfwTerminate(); return 0;}
    cout << "glewInit: GLEW_OK"<< endl;

    glViewport(0, 0, 800, 800);
    glEnable(GL_DEPTH_TEST);
    // stbi_set_flip_vertically_on_load(true);

    // build and compile shaders
    // -------------------------
    ShaderClass OwOShader("./include/vertex.vs", "./include/fragment.fs");

    // load models
    // -----------
    // Model OwOModel("./backpack/backpack.obj");
    // Model OwOModel("./sketch/final_v01.obj");

    string obj_path("./sketch/final_v01.obj");

    ObjClass ABCD( obj_path );
    cout << ABCD.getName() << endl;
    cout << ABCD.getDir() << endl;
    // cout << "point Num " << ABCD.v.size() << endl;
    // cout << "Normal Num " << ABCD.vn.size() << endl;
    // cout << "Texture coord Num " << ABCD.vt.size() << endl;
    cout << "face Num " << ABCD.faces_indices.size() << endl;

    // for(size_t i =0; i< ABCD.obj_vector.size(); i++)
    // {
    //     cout << "objecttag: "<< ABCD.obj_vector[i].object_tag << "mtlname: " << ABCD.obj_vector[i].mtl_name << endl;
    //     cout << endl;
    // }

                                    // cout <<'\n'<< endl;
                                    // FILE* fdtest = fopen("./testdata.txt", "w+");
                                    // if(fdtest == NULL)
                                    // {
                                    //     cout << "failed to open file" << endl;
                                    // }
                                    // else{
                                    //     char lines[255];
                                    //     sprintf(lines, "size: %ld\n", 3*ABCD.obj_vector[0].NumOFFaces);
                                    //     fputs(lines, fdtest);
                                    //     for(size_t h = 0; h < 3*ABCD.obj_vector[0].NumOFFaces; h++)
                                    //     {
                                    //         sprintf(lines, "%f, %f, %f, %f, %f,\n", ABCD.face_data[h].position.x,  ABCD.face_data[h].position.y,  ABCD.face_data[h].position.z,  ABCD.face_data[h].textureCoordinate.x, ABCD.face_data[h].textureCoordinate.y);
                                    //         fputs(lines, fdtest);

                                    //     } 
                                    // }
                                    // cout << "end;" <<endl;

                // cout << "v idx= " << ABCD.faces_indices[0].x-1 << "vn idx= " << ABCD.faces_indices[0].y-1 << "vt idx= " << ABCD.faces_indices[0].z-1 << endl;


                unsigned int VBO , VAO, EBO;
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);

                    glBindVertexArray(VAO);
                        glBindBuffer(GL_ARRAY_BUFFER, VBO);
                        glBufferData(GL_ARRAY_BUFFER, ABCD.face_data.size() * sizeof(Vertex_st), &ABCD.face_data[0], GL_STATIC_DRAW);
                        cout << "ABCD.obj_vector[0].NumOFFaces= " <<ABCD.obj_vector[2].NumOFFaces << endl;
                        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void*)0);
                        glEnableVertexAttribArray(0);

                        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void*)offsetof(Vertex_st, normal));
                        glEnableVertexAttribArray(1);

                        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void*)offsetof(Vertex_st, textureCoordinate));
                        glEnableVertexAttribArray(2);
                        // cout << sizeof(Vertex_st) << endl;

                        glBindBuffer(GL_ARRAY_BUFFER, 0); 
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                        glBindVertexArray(0);

    
    /* testure init*/
        ImageClass  HEADTexture("./sketch/head.png", true);
        ImageClass  BODYTexture("./sketch/body.png", true);
        ImageClass  allTexture("./sketch/all.png", true);



        OwOShader.setInt("texture_diffuse", 0);
        // OwOShader.setInt("texture_diffuse", 1);
        // OwOShader.setInt("texture_diffuse", 2);

        



    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f),glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
    model = glm::translate(model, glm::vec3(-5, -80, -3));

    glm::mat4 view = glm::mat4(1.0f);
    // note that we’re translating the scene in the reverse direction
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(OwOShader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(OwOShader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(OwOShader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));



    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    viewLoc = glGetUniformLocation(OwOShader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 

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

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        const float cameraSpeed = 25.0f * deltaTime; // adjust accordingly
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
        // model = glm::rotate(model, glm::radians(-1.0f),glm::vec3(1.0f, 0.5f, 0.0f));
        // modelLoc = glGetUniformLocation(OwOShader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        viewLoc = glGetUniformLocation(OwOShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 

        projection = glm::perspective(glm::radians(Zoom), 800.0f / 800.0f, 0.1f, 100.0f);
        int projectionLoc = glGetUniformLocation(OwOShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, OwOTexture.texture);
        OwOShader.use();

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // for (unsigned int i =0; i<10; i++)
        // {
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.10f, 0.10f, 0.10f));
        // model = glm::rotate(model, glm::radians((float)glfwGetTime()*(i+10)),glm::vec3(1.0f, 0.5f, 0.0f));
        OwOShader.setMat4("model", model);

        
        
        // glActiveTexture(GL_TEXTURE0);
        // // glBindTexture(GL_TEXTURE_2D, HEADTexture.texture);
        // glBindTexture(GL_TEXTURE_2D, ABCD.Texture_vector[0].texture);

        // glDrawArrays(GL_TRIANGLES, 0, ABCD.obj_vector[0].NumOFFaces *3 );
        // // glDrawArrays(GL_TRIANGLES, ABCD.obj_vector[0].NumOFFaces *3,  ABCD.obj_vector[1].NumOFFaces *3);
        // glDrawArrays(GL_TRIANGLES, ABCD.obj_vector[0].NumOFFaces *3 + ABCD.obj_vector[1].NumOFFaces *3,  ABCD.obj_vector[2].NumOFFaces *3);
        // // glDrawArrays(GL_TRIANGLES, ABCD.obj_vector[0].NumOFFaces *3 + ABCD.obj_vector[1].NumOFFaces *3 + ABCD.obj_vector[2].NumOFFaces *3,  ABCD.obj_vector[3].NumOFFaces *3);

        // glActiveTexture(GL_TEXTURE0);
        // // glBindTexture(GL_TEXTURE_2D, BODYTexture.texture);
        // glBindTexture(GL_TEXTURE_2D, ABCD.Texture_vector[1].texture);
        // glDrawArrays(GL_TRIANGLES, ABCD.obj_vector[0].NumOFFaces *3 + ABCD.obj_vector[1].NumOFFaces *3 + ABCD.obj_vector[2].NumOFFaces *3 + ABCD.obj_vector[3].NumOFFaces *3,  ABCD.obj_vector[4].NumOFFaces *3);

        // glActiveTexture(GL_TEXTURE0);
        // // glBindTexture(GL_TEXTURE_2D, allTexture.texture);
        // glBindTexture(GL_TEXTURE_2D, ABCD.Texture_vector[2].texture);
        // glDrawArrays(GL_TRIANGLES, ABCD.obj_vector[0].NumOFFaces *3 + ABCD.obj_vector[1].NumOFFaces *3 + ABCD.obj_vector[2].NumOFFaces *3 + ABCD.obj_vector[3].NumOFFaces *3 + ABCD.obj_vector[4].NumOFFaces *3 + ABCD.obj_vector[5].NumOFFaces *3 + ABCD.obj_vector[6].NumOFFaces *3,  ABCD.obj_vector[7].NumOFFaces *3);

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        ABCD.DrawMesh(OwOShader, &ABCD.obj_vector[0]);
        ABCD.DrawMesh(OwOShader, &ABCD.obj_vector[2]);

        ABCD.DrawMesh(OwOShader, &ABCD.obj_vector[4]);
        ABCD.DrawMesh(OwOShader, &ABCD.obj_vector[7]);

        // OwOModel.Draw(OwOShader);
        // ABCD.DrawOjbject(OwOShader);

        // }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();

    return ret;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
    Zoom = 1.0f;
    if (Zoom > 90.0f)
    Zoom = 90.0f;
}