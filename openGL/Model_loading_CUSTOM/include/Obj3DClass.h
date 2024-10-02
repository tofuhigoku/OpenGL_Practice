#ifndef Obj3DClass_H
#define Obj3DClass_H

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
#include <vector>
#include "ImageClass.h"
#include "ShaderClass.h"
// #include "Mesh.h"
// #include "ModelClass.h"
#include "stb_image.h"

using namespace std;

struct line_data
{
    char tag[100];
    float d1,d2,d3;
};

struct Vertex_st {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinate;
};

struct Texture {
    unsigned int id;
    string mtl_name;
    string path;
};


struct Object
{
    char * object_tag;
    size_t object_tag_len;
    
    char * mtl_name;
    size_t mtl_name_len;

    size_t NumOFFaces;

    unsigned int VAO, VBO, EBO;
    // size_t NumOFVertex_position;
    // size_t NumOFVertex_normal;
    // size_t NumOFTexture_coordinate;

};

class ObjClass
{
    public:
        string model_name;
        string mtl_name;

        struct Object obj_buffer;
        unsigned int index;

        vector<struct Object> obj_vector;
        vector<struct Texture> Texture_vector;
        // vector<glm::vec3> v;
        // vector<glm::vec3> vn;
        // vector<glm::vec2> vt;
        vector<glm::uvec3> faces_indices;
        vector<Vertex_st> face_data;
        line_data rawline;

        ObjClass(const string &path);
        ~ObjClass();

        int importOBJ(const string &path);
        string getName();
        string getDir();

    private:
        bool statusOK;
        string directory;
        void setName(const string &name);

        int process_rawdata(FILE* fd);
        int ExtractFaceindices(const char* dataline);

        int fill_nametag(char* nametag);
        int fill_mtlname(const char* mtlname);

        int GlobTexture();

        int setupObjectMesh(struct Object* p_object_buffer, unsigned int &object_buffer_index);
        int Draw(ShaderClass &shader);
};


ObjClass::ObjClass(const string &path)
    {
        memset(&obj_buffer, 0, sizeof(obj_buffer));
        index = 0;
        if(importOBJ(path) == 0)
        {
            statusOK = true;
        }
        else
        {
            statusOK = false;
        }

        if(statusOK == true)
        {
            size_t pos = path.find_last_of('/');
            setName(path.substr(pos+1,path.size() -1-pos ));
            directory = path.substr(0, pos);

            if(mtl_name.size() <= strlen(".mtl"))
            {
                cout << "mtl file not found!" << endl;
            }
        }
    }

ObjClass::~ObjClass()
    {
        if(obj_buffer.mtl_name != NULL) free(obj_buffer.mtl_name);
        if(obj_buffer.object_tag != NULL) free(obj_buffer.object_tag);

        memset(&obj_buffer, 0, sizeof(obj_buffer));
        index =0;
    }

int ObjClass::importOBJ(const string &path)
    {
        int ret = 0;
        FILE* fd_src = NULL;

        string format_file  = path.substr(path.find_last_of('.'), 4);
        if(strcmp(format_file.c_str(), ".obj"))
        {
            cout << "Invalid format file (not .obj file)" << endl;
            ret = 1;
        }
        if(ret == 0 )
        {
            fd_src = fopen(path.c_str(), "r");
            if(fd_src == NULL)
            {
                cout << "failed to open file" << endl;
                ret = 1;
            }
        }
        if (ret == 0)
        {
            /* code */
            ret = process_rawdata(fd_src);
        }

        if(fd_src != NULL) fclose(fd_src);

        return ret;
    }

int ObjClass::process_rawdata(FILE* fd)
    {
        int ret = 0;
        vector<glm::vec3> v;
        vector<glm::vec3> vn;
        vector<glm::vec2> vt;
        FILE* local_fd = NULL;
        if(fd == NULL)
        {
            cout << "failed to open file" << endl;
            ret = 1;
        }
        local_fd = fd;

        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        // size_t write;
        

        while((read = getline(&line, &len, local_fd)) != -1)
        {
            /**
             * @brief while loop for processing raw data from .obj file.
             * 
             */
            if(line[0] == '#')
            {
                continue;
            }


            sscanf(line, "%s %f %f %f", rawline.tag, &rawline.d1, &rawline.d2, &rawline.d3);
            if(strcmp(rawline.tag, "mtllib") ==0 )
            {
                char tag2[100];
                sscanf(line, "%s %s", rawline.tag, tag2);
                if(strlen(tag2) > 0)
                {
                    string str(tag2);
                    mtl_name = str;
                }
                continue;
            }

            switch (rawline.tag[0])
            {
            case 'v':   /**< line format is match with vertex type */
            
                if(rawline.tag[1] == 't')           /**< line format is match with vertice texture coordinate */
                {
                    vt.push_back(glm::vec2(rawline.d1, rawline.d2));
                }
                else if(rawline.tag[1] == 'n')      /**< line format is match with vertice normal vector */
                {
                    vn.push_back(glm::vec3(rawline.d1, rawline.d2, rawline.d3));
                }
            else                                    /**< line format is match with vertice position vector */
                {
                    v.push_back(glm::vec3(rawline.d1, rawline.d2, rawline.d3));
                }
                break;
            case 'o':   /**< line format is match with object */

                fill_nametag(line);

                obj_vector.push_back(obj_buffer);
                index = obj_vector.size() - 1;

                obj_buffer.NumOFFaces=0;

                break;
            case 'g':   /**< line format is match with group */

                char tag2[100];
                sscanf(line, "%s %s", rawline.tag, tag2);
                if(strcmp(tag2, "default") != 0)
                {
                    fill_nametag(line);
                    
                    obj_vector.push_back(obj_buffer);
                    index = obj_vector.size() - 1;

                    obj_buffer.NumOFFaces=0;
                }
                break;
            case 'f':   /**< line format is match with faces indices */
                ExtractFaceindices(line);
                obj_vector[index].NumOFFaces +=1;

                break;

            default:
                /* cout something here! */
                if(strcmp(rawline.tag, "usemtl") == 0 )
                {

                    std::string str(line);
                    string mtlname  = str.substr(str.find_first_of(' ')+1, str.size() - str.find_first_of(' '));

                    fill_mtlname(mtlname.c_str());
                }
                else
                {
                    /* cout something here! */
                }
                break;
            }
        }

        for(size_t ss = 0; ss < faces_indices.size(); ss ++)
        {
            struct Vertex_st tmp = {v[faces_indices[ss][0]-1], vn[faces_indices[ss][2]-1], vt[faces_indices[ss][1]-1] };
            // printf("face idx 1: v1 %d vn1 %d vt1 %d \n",faces_indices[ss][0]-1 , faces_indices[ss][1]-1 ,faces_indices[ss][2]-1);
            // printf("face data: v= %f, %f, %f\tvn= %f, %f, %f\tvt= %f, %f\n",tmp.position.x, tmp.position.y, tmp.position.z, tmp.normal.x,tmp.normal.y, tmp.normal.z, tmp.textureCoordinate.x, tmp.textureCoordinate.y);
            face_data.push_back(tmp);
        }

        return ret;
    }

int ObjClass::ExtractFaceindices(const char* dataline)
    {
        int c = 0;
        if(dataline == NULL)
        {
            return 1;
        }
        char* local_buffer = NULL;
        local_buffer =(char*) calloc(strlen(dataline), sizeof(char));

        if(local_buffer == NULL)
        {
            return 2;
        }

        while(dataline[c] != '\n' && dataline[c] != '\0')
        {
            local_buffer[c] = dataline[c];
            if (dataline[c] == '/')
            {
                local_buffer[c] = ' ';
            }
            c++;
        }
        char tag[10];
        unsigned int num[9];
        sscanf(local_buffer, "%s %d %d %d %d %d %d %d %d %d", tag, &num[0], &num[1], &num[2], &num[3], &num[4], &num[5],&num[6], &num[7], &num[8]);
        for(size_t i =0; i < 3; i++)
        {
            faces_indices.push_back(glm::vec3(num[i*3], num[i*3+1], num[i*3+2]));
        }
        return 0;
    }

int ObjClass::fill_nametag(char* nametag)
    {
        if(nametag == NULL)
        {
            cout << "name tag not found" << endl;
            return 1;
        }

        obj_buffer.object_tag_len = strlen(nametag);
        if(obj_buffer.object_tag_len == 0  || nametag[0] == '\n' || nametag[0] == '\0')
        {
            cout << "invalid name tag" << endl;
            return 1;
        }

        obj_buffer.object_tag = NULL;
        obj_buffer.object_tag = (char*) calloc(obj_buffer.object_tag_len, sizeof(char));
        if(obj_buffer.object_tag == NULL)
        {
            cout << "obj_buffer.object_tag == NULL => calloc name tag buffer failed" << endl;
            return 1;
        }
        strcpy(obj_buffer.object_tag, nametag);

        return 0;
    }

int ObjClass::fill_mtlname(const char* mtlname)
    {
        if(mtlname == NULL)
        {
            cout << "mtlname not found" << endl;
            return 1;
        }

        obj_buffer.mtl_name_len = strlen(mtlname);
        if(obj_buffer.mtl_name_len == 0  || mtlname[0] == '\n' || mtlname[0] == '\0')
        {
            cout << "invalid mtlname" << endl;
            return 1;
        }

        obj_buffer.mtl_name = NULL;
        obj_buffer.mtl_name = (char*) calloc(obj_buffer.mtl_name_len, sizeof(char));
        if(obj_buffer.mtl_name == NULL)
        {
            cout << "obj_buffer.mtl_name == NULL => calloc mtlname buffer failed" << endl;
            return 1;
        }
        strcpy(obj_buffer.mtl_name, mtlname);

        return 0;
    }

void ObjClass::setName(const string &name)
{
    model_name = name;
}

string ObjClass::getName()
{
    // cout << sizeof(Vertex_st) << endl;
    return model_name;
}

string ObjClass::getDir()
{
    return directory;
}

int ObjClass::GlobTexture()
{
    int ret = 0;



    return ret;
}

int ObjClass::setupObjectMesh(struct Object* p_object_buffer, unsigned int &object_buffer_index)
{

    int ret = 0;
    if(p_object_buffer == NULL)
    {
        cout << "p_object_buffer = NULL => setupObjectMesh failed" << endl;
        return 1;
    }
    unsigned int offset = 0;
    for (size_t u = 0; u < object_buffer_index; u++)
    {
        offset += obj_vector[u].NumOFFaces;
    }
    unsigned int* p_VAO  = &p_object_buffer->VAO;
    unsigned int* p_VBO = &p_object_buffer->VBO;
    unsigned int* p_EBO = &p_object_buffer->EBO;

    glGenVertexArrays(1, p_VAO);
    glGenBuffers(1, p_VBO);
    glGenBuffers(1, p_EBO);

    glBindVertexArray(*p_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *p_VBO);
        glBufferData(GL_ARRAY_BUFFER, p_object_buffer->NumOFFaces * sizeof(Vertex_st), &face_data[offset], GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *p_EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void*)offsetof(Vertex_st, normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_st), (void*)offsetof(Vertex_st, textureCoordinate));
        
    glBindVertexArray(0);

    return ret;
}

int ObjClass::Draw(ShaderClass &shader)
{
    for(unsigned int i = 0; i < obj_vector.size(); i++)
    {
        // obj_vector[i].Draw(shader);
        cout << "Draw object: " << obj_vector[i].object_tag << " usemtl: " << obj_vector[i].mtl_name << endl;
    }
}

#endif