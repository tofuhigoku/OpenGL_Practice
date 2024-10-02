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

struct Vertex {
    float position[3];
    float normal[3];
    float textureCoordinate[2];
};

struct Object
{
    char * object_tag;
    size_t object_tag_len;
    
    char * mtl_name;
    size_t mtl_name_len;

    size_t NumOFFaces;

    // size_t NumOFVertex_position;
    // size_t NumOFVertex_normal;
    // size_t NumOFTexture_coordinate;

};

class ObjClass
{
    public:
        struct Object obj_buffer;
        unsigned int index;

        vector<struct Object> obj_vector;
        vector<glm::vec3> v;
        vector<glm::vec3> vn;
        vector<glm::vec2> vt;
        vector<unsigned int> faces_indices;
        line_data rawline;

        ObjClass();
        ~ObjClass();

        int importOBJ(const string &path);

    private:
        int process_rawdata(FILE* fd);
        int ExtractFaceindices(const char* dataline);

        int fill_nametag(char* nametag);
        int fill_mtlname(const char* mtlname);
};


ObjClass::ObjClass()
    {
        memset(&obj_buffer, 0, sizeof(obj_buffer));
        index = 0;
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

        }

        if(fd_src != NULL) fclose(fd_src);

        return ret;
    }

int ObjClass::process_rawdata(FILE* fd)
    {
        int ret = 0;
        FILE* local_fd = NULL;
        if(fd == NULL)
        {
            cout << "failed to open file" << endl;
            ret = 1;
        }
        local_fd = fd;

        char * line = NULL;
        size_t len = 0;
        size_t read;
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


            sscanf(line, "%s %f %f %f", rawline, &rawline.d1, &rawline.d2, &rawline.d3);
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
        for(int i =0; i < sizeof(num)/sizeof(unsigned int); i++)
        {
            faces_indices.push_back(num[i]);
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

#endif