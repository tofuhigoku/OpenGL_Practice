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

struct Texture_st {
    unsigned int id;
    string mtl_name;
    string path;
    int width, height, nrChannels;
    unsigned char *data;
    unsigned int texture; 
};


struct Object
{
    string object_tag;
    // size_t object_tag_len;
    
    string mtl_name;
    // size_t mtl_name_len;

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
        vector<struct Texture_st> Texture_vector;
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
        int DrawMesh(ShaderClass &shader, struct Object* p_object_buffer);
        int DrawOjbject(ShaderClass &shader);

    private:
        bool statusOK;
        string directory;
        void setName(const string &name);

        int process_rawdata(FILE* fd);
        int ExtractFaceindices(const char* dataline);

        int fill_nametag(const char* nametag);
        int fill_mtlname(const char* mtlname);

        int GlobTexture();
        int LoadTexture( struct Texture_st* p_Texture_buffer);
        int InitTexture();

        int setupObjectMesh(struct Object* p_object_buffer, size_t &object_buffer_index);
        
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
                statusOK = false;
            }
            else
            {
                GlobTexture();
                for(size_t i =0; i <  Texture_vector.size(); i ++)
                {
                    cout << "texture"<<i<<"  id= "<< Texture_vector[i].id<<" mtl_name= "<<Texture_vector[i].mtl_name << " path= "<<Texture_vector[i].path <<endl;
                }
                InitTexture();
            }
        }

        if(statusOK == true)
        {
            struct Object* p_object_buffer =  NULL;
            for(size_t i =0; i< obj_vector.size(); i++)
            {
                p_object_buffer = &obj_vector[i];
                if(setupObjectMesh(p_object_buffer, i) != 0)
                {
                    statusOK = false;
                    break;
                }
            }
        }
    }

ObjClass::~ObjClass()
    {
        // if(obj_buffer.mtl_name != NULL) free(obj_buffer.mtl_name);
        // if(obj_buffer.object_tag != NULL) free(obj_buffer.object_tag);

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
                    cout << "mtl_name: " <<mtl_name<<endl;
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
                    char tag2[255];
                    sscanf(line, "%s %s", rawline.tag, tag2);
                    std::string mtlname(tag2);
                    // string mtlname  = str.substr(str.find_first_of(' ')+1, str.size() - str.find_first_of(' '));
                    cout << "mtlname: " << mtlname << endl;
                    // fill_mtlname(mtlname.c_str());
                    obj_vector[index].mtl_name = mtlname;
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

int ObjClass::fill_nametag(const char* nametag)
    {
        if(nametag == NULL)
        {
            cout << "name tag not found" << endl;
            return 1;
        }

        // obj_buffer.object_tag_len = strlen(nametag);
        // if(obj_buffer.object_tag.size() == 0  || nametag[0] == '\n' || nametag[0] == '\0')
        // {
        //     cout << "invalid name tag" << endl;
        //     return 1;
        // }

        // obj_buffer.object_tag = (char*) calloc(obj_buffer.object_tag_len, sizeof(char));
        // if(obj_buffer.object_tag == NULL)
        // {
        //     cout << "obj_buffer.object_tag == NULL => calloc name tag buffer failed" << endl;
        //     return 1;
        // }
        // strcpy(obj_buffer.object_tag, nametag);
        string local_nametag(nametag);
        obj_buffer.object_tag =  local_nametag;
        return 0;
    }

int ObjClass::fill_mtlname(const char* mtlname)
    {
        if(mtlname == NULL)
        {
            cout << "mtlname not found" << endl;
            return 1;
        }

        // obj_buffer.mtl_name_len = strlen(mtlname);
        // if(obj_buffer.mtl_name_len == 0  || mtlname[0] == '\n' || mtlname[0] == '\0')
        // {
        //     cout << "invalid mtlname" << endl;
        //     return 1;
        // }

        // obj_buffer.mtl_name = NULL;
        // obj_buffer.mtl_name = (char*) calloc(obj_buffer.mtl_name_len, sizeof(char));
        // if(obj_buffer.mtl_name == NULL)
        // {
        //     cout << "obj_buffer.mtl_name == NULL => calloc mtlname buffer failed" << endl;
        //     return 1;
        // }
        // strcpy(obj_buffer.mtl_name, mtlname);
        string local_mtlname(mtlname);
        obj_buffer.mtl_name = local_mtlname;

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

    FILE* fd_mtlfile = NULL;
    string mtl_path = directory + "/" + mtl_name;
    cout << "mtl path: " << mtl_path << endl;
    fd_mtlfile = fopen(mtl_path.c_str(), "r");

    if(fd_mtlfile == NULL)
    {
        cout << "No mtl found /  Cant open mtl file" << endl;
        return 1;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    Texture_st texture_buffer = {0};
    while((read = getline(&line, &len, fd_mtlfile)) != -1)
    {
        /* code */
        if(line[0] == '#')
        {
            continue;
        }

        sscanf(line, "%s %f %f %f", rawline.tag, &rawline.d1, &rawline.d2, &rawline.d3);
        if(strcmp(rawline.tag, "newmtl") == 0)
        {
            char mtl_name[255];
            sscanf(line, "%s %s", rawline.tag, mtl_name);
            string str(mtl_name);
            texture_buffer.mtl_name = str;
            cout << "newmtl: " << str << endl;
            texture_buffer.id = Texture_vector.size();
            Texture_vector.push_back(texture_buffer);
        }
        else if(strcmp(rawline.tag, "map_Kd") == 0)
        {
            /**
             * @brief Diffuse map
             * 
             */
            char texture_name[255];
            sscanf(line, "%s %s", rawline.tag, texture_name);
            cout << "texture name " << texture_name <<endl;
            string str_texture_name(texture_name);
            string texture_path = directory + '/' + str_texture_name;

            int index = Texture_vector.size()-1;
            if(index>=0)
            {
                Texture_vector[index].path = texture_path;
            }
            else
            {
                cout << "map_kd exsits before mtl " << endl;
            }

        }
        else if(strcmp(rawline.tag, "map_Bump") == 0)
        {
            /**
             * @brief For future Normal map update
             * 
             */
        }
        else if(strcmp(rawline.tag, "map_Ks") == 0)
        {
            /**
             * @brief For future Specular map update
             * 
             */
        }
        
    }

    if (fd_mtlfile != NULL)
    {
        /* code */
        fclose(fd_mtlfile);
    }
    

    return ret;
}
int ObjClass::LoadTexture(struct Texture_st* p_Texture_buffer)
{
    int ret =  0;
    if(p_Texture_buffer == NULL)
    {
        cout << "p_Texture_buffer = NULL " << endl;
        return 1;
    }
    stbi_set_flip_vertically_on_load(true);

    string texture_path = p_Texture_buffer->path;
    unsigned char* p_data = p_Texture_buffer->data;
    int width, height, nrChannels;

    p_Texture_buffer->texture=0;
    glGenTextures(1, &p_Texture_buffer->texture);
    glBindTexture(GL_TEXTURE_2D, p_Texture_buffer->texture);
    // set the texture wrapping/filtering options (on currently bound texture)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    p_data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (p_data)
    {
        p_Texture_buffer->width = width;
        p_Texture_buffer->height = height;
        p_Texture_buffer->nrChannels = nrChannels;

        cout << "nrChannels = " << p_Texture_buffer->nrChannels << endl;
        GLenum format;
        if(nrChannels == 4)
        {
            format = GL_RGBA;
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, p_data);
            // glGenerateMipmap(GL_TEXTURE_2D);

        }
        else if( nrChannels == 3)
        {
            format = GL_RGB;
            // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, p_data);
            // glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if(nrChannels == 1)
        {
            format = GL_RED;
        }
        else
        {
            cout << "NUm of Channel not equal 3 or 4" << endl;
            ret = 2;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, p_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        ret = 1;
    }

    stbi_image_free(p_data);

    return ret;
}

int ObjClass::InitTexture()
{
    int ret = 0;
    for (size_t i =0; i < Texture_vector.size(); i++)
    {
        ret = LoadTexture(&Texture_vector[i]);
        if(ret ==0)
        {
            cout << "successfully loaded texture: "  << Texture_vector[i].path << " texture id= "<< Texture_vector[i].texture << endl;
        }

    }
    return ret;

}

int ObjClass::setupObjectMesh(struct Object* p_object_buffer, size_t &object_buffer_index)
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
        glBufferData(GL_ARRAY_BUFFER, p_object_buffer->NumOFFaces *3* sizeof(Vertex_st), &face_data[offset], GL_STATIC_DRAW);
        cout << "setupObjectMesh: " << p_object_buffer->object_tag << "NumOFFaces: " << p_object_buffer->NumOFFaces << " vertices size: " << p_object_buffer->NumOFFaces*3 << endl;
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

int ObjClass::DrawMesh(ShaderClass &shader, struct Object* p_object_buffer )
{
    // for(unsigned int i = 0; i < obj_vector.size(); i++)
    // {
    //     // obj_vector[i].Draw(shader);
    //     cout << "Draw object: " << obj_vector[i].object_tag << " usemtl: " << obj_vector[i].mtl_name << endl;
    
    // }

    // unsigned int diffuseNr = 1;
    // unsigned int specularNr = 1;

    // for(unsigned int i = 0; i < textures.size(); i++)
    // {
    int ret = 0;
    if(p_object_buffer == NULL)
    {
        cout << "p_object_buffer = NULL => DrawMesh failed" << endl;
        return 1;
    }
    // cout << "p_object_buffer != NULL" << endl;

    struct Texture_st* p_Texture_buffer = NULL;
    // cout << "Texture_vector.size()= " << Texture_vector.size() << endl;
    for (size_t i =0; i < Texture_vector.size() ; i++)
    {
        // cout << i <<" p_object_buffer->mtl_name  " << p_object_buffer->mtl_name << ends;
        // cout << i <<" Texture_vector[i].mtl_name  " << Texture_vector[i].mtl_name << endl;

        if( Texture_vector[i].mtl_name == p_object_buffer->mtl_name)
        {
            p_Texture_buffer = &Texture_vector[i];
            break;
        }
    }
    if(p_Texture_buffer == NULL)
    {
        cout << "not found matched texture : object" << p_object_buffer->mtl_name  << " : " << p_object_buffer->object_tag << endl;
        return 1;
    }
    
    cout << "found matched texture : object" << p_object_buffer->mtl_name  << " : " << p_object_buffer->object_tag << endl;

    glActiveTexture(GL_TEXTURE0); // activate texture unit first
        // retrieve texture number (the N in diffuse_textureN)
        // string number;
        // string name = textures[i].type;
        // if(name == "texture_diffuse")
        //     number = std::to_string(diffuseNr++);
        // else if(name == "texture_specular")
        //     number = std::to_string(specularNr++);

        // cout << "name: " << name << "\tsetFloat: " << ("material." + name + number).c_str() << endl;

        // shader.setFloat(("material." + name + number).c_str(), i);
        glUniform1i(glGetUniformLocation(shader.ID, "texture_diffuse"), 0);
        glBindTexture(GL_TEXTURE_2D, p_Texture_buffer->texture);
    // }
    // glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(p_object_buffer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, p_object_buffer->VBO);
    // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, p_object_buffer->NumOFFaces*3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);

}

int ObjClass::DrawOjbject(ShaderClass &shader)
{  
    int ret =0;
    struct Object* p_object_buffer = NULL;
    for (size_t i = 0; i < obj_vector.size(); i++)
    {
        p_object_buffer = &obj_vector[1];
        cout << "start draw object: " << p_object_buffer->object_tag << " num of vertices: " << p_object_buffer->NumOFFaces*3 << endl;
        DrawMesh(shader, p_object_buffer);
    }

    return ret;

}

#endif