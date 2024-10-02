#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>


using namespace std;

char  desfile_name[] ="final_v01_without_s.txt";
char  srcfile_name[] = "final_v01.obj";

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
    public:
        char * object_tag;
        size_t object_tag_len;
        
        char * mtl_name;
        size_t mtl_name_len;

        size_t NumOFFaces;

        size_t NumOFVertex_position;
        size_t NumOFVertex_normal;
        size_t NumOFTexture_coordinate;

        // struct Vertex* p_Vertex_Array;
        // Object(const char* name)
        // {
        //     object_tag =  NULL;

        //     object_tag_len = strlen(name);
        //     if(object_tag_len == 0)
        //     {
        //         object_tag = new char(object_tag_len*sizeof(char));
        //         if(object_tag != NULL)
        //         {
        //             strcpy(object_tag, name);
        //         }
        //     }
        // }
        // ~Object()
        // {
        //     if(object_tag != NULL)
        //     {
        //         delete(object_tag);
        //     }
        // }
};

class OBJ2
{
    public:
        struct Object obj_buffer;
        unsigned int index;
        vector<struct Object> obj_vector;

    OBJ2()
    {
        memset(&obj_buffer, 0, sizeof(obj_buffer));
        index = 0;
    }
    ~OBJ2()
    {
        if(obj_buffer.mtl_name != NULL) free(obj_buffer.mtl_name);
        if(obj_buffer.object_tag != NULL) free(obj_buffer.object_tag);

        memset(&obj_buffer, 0, sizeof(obj_buffer));
        index =0;
    }
    int fill_nametag(char* nametag)
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
    int fill_mtlname(char* mtlname)
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
};

class OBJfile
{
    public:
        vector<string> file_content;
        vector<string> Object_tag;
        size_t FileLen;
        size_t ObjectNum;
        struct Object* p_Object;
    OBJfile(const string &filename)
    {
        ObjectNum = 0;
        FileLen = 0;
        p_Object = NULL;
    }
    int p_ObjetcAlloc()
    {
        if(ObjectNum ==0)
        {
            cout << "ObjectNum=0 => NoObject found" << endl;
            return 1;
        }
        p_Object = (Object*) calloc(ObjectNum, sizeof(Object));
        if(p_Object == NULL)
        {
            cout << "p_Object == NULL => p_ObjetcAlloc failed" << endl;
            return 1;
        }
        return 0;
    }
    int fill_Objectbuffer()
    {
        if(p_Object ==  NULL)
        {
            cout << "p_Object == NULL => Objectbuffer was not allocated" << endl;
            return 1;
        }
        
        for (int i = 0; i< Object_tag.size(); i++)
        {
            p_Object[i].object_tag_len = Object_tag[i].size();
            p_Object[i].object_tag = (char*) calloc(p_Object[i].object_tag_len, sizeof(char));
            if(p_Object[i].object_tag == NULL)
            {
                cout << "failed to allocate object tag: "<< Object_tag[i] << endl;
                return 1;
            }
            strcpy(p_Object[i].object_tag, Object_tag[i].c_str());
        }
        int c = -1;
        for (int i =0; i < FileLen; i++ )
        {
            char k = file_content[i][0];
            char b = file_content[i][1];
            switch (k)
            {
            case 'g':
                c+=1;
                break;

            case 'o':
                c+=1;
                break;

            case 'v':
                    if(b == 'n')
                    {
                        p_Object[c].NumOFVertex_normal+=1;
                    }
                    else if (b == 't')
                    {
                        p_Object[c].NumOFTexture_coordinate+=1;
                    }
                    else
                    {
                        p_Object[c].NumOFVertex_position+=1;
                    }
                break;

            case 'f':
                    p_Object[c].NumOFFaces+=1;
                break;

            case 's':
            default:
                break;
            }
            
        }
        return 0;
    }

    int write(FILE * fd, int* writeLen)
    {
        int ret =0;
        int c = 0;
        if(fd == NULL)
        {
            cout << "invalid fd" << endl;
            return -1;
        }
        if(file_content.size()==0)
        {
            cout << "file_content empty" << endl;
            return -1;
        }
        for (int i = 0; i< file_content.size(); i++)
        {
            ret = fputs(file_content[i].c_str(), fd);
            if(ret <= 0)
            {
                break;
            }
            c+=1;
        }
        if(writeLen != NULL)
            *writeLen = c;
        
        return ret;
    }
};

vector<float> v;
vector<float> vn;
vector<float> vt;

vector<unsigned int> faces_indices;
int main(int argc, char** argv)
{
    int ret = 0;
    FILE * fd_src =  NULL;
    FILE * fd_des =  NULL;
    char *s;

    fd_src = fopen(srcfile_name, "r");
    fd_des = fopen(desfile_name, "w+");
    if(fd_des == NULL || fd_src == NULL)
    {
        printf("failed to open file: %s", (fd_des==NULL)?srcfile_name:desfile_name);
        return 1;
    }
    char * line = NULL;
    size_t len = 0;
    size_t read;
    size_t write;

    OBJ2    object_test;

    OBJfile Objectfile(srcfile_name);
    struct  line_data line_data_;
    while ((read = getline(&line, &len, fd_src)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s", line);
        if(line[0] == '#')
        {
            continue;
        }
        Objectfile.file_content.push_back(line);
        Objectfile.FileLen +=1;

        int c_ = 0;
        sscanf(line, "%s %f %f %f", line_data_.tag, &line_data_.d1, &line_data_.d2, &line_data_.d3);
        switch (line_data_.tag[0])
        {
        case 'v':   /**< line format is match with vertex type */
        
            if(line_data_.tag[1] == 't')    /**< line format is match with vertice texture coordinate */
            {
                vt.push_back(line_data_.d1);
                vt.push_back(line_data_.d2);
            }
            else if(line_data_.tag[1] == 'n')   /**< line format is match with vertice normal vector */
            {
                vn.push_back(line_data_.d1);
                vn.push_back(line_data_.d2);
                vn.push_back(line_data_.d3);
            }
            else                                /**< line format is match with vertice position vector */
            {
                v.push_back(line_data_.d1);
                v.push_back(line_data_.d2);
                v.push_back(line_data_.d3);
            }
            break;
        case 'o':   /**< line format is match with object */
            Objectfile.ObjectNum+=1;
            Objectfile.Object_tag.push_back(line);

            object_test.fill_nametag(line);

            object_test.obj_vector.push_back(object_test.obj_buffer);
            object_test.index = object_test.obj_vector.size() - 1;

            object_test.obj_buffer.NumOFFaces=0;
            break;
        case 'g':   /**< line format is match with group */
            Objectfile.ObjectNum+=1;
            Objectfile.Object_tag.push_back(line);

            char tag2[100];
            sscanf(line, "%s %s", line_data_.tag, tag2);
            if(strcmp(tag2, "default") != 0)
            {


                object_test.fill_nametag(line);
                



                object_test.obj_vector.push_back(object_test.obj_buffer);
                object_test.index = object_test.obj_vector.size() - 1;

                object_test.obj_buffer.NumOFFaces=0;
            }
            break;
        case 'f':   /**< line format is match with faces indices */
            c_ = 0;
            while(line[c_] != '\n' && line[c_] != '\0')
            {
                if (line[c_] == '/')
                {
                    line[c_] = ' ';
                }
                c_++;
            }
            char tag[10];
            unsigned int num[9];
            sscanf(line, "%s %d %d %d %d %d %d %d %d %d", tag, &num[0], &num[1], &num[2], &num[3], &num[4], &num[5],&num[6], &num[7], &num[8]);
            for(int i =0; i < sizeof(num)/sizeof(unsigned int); i++)
            {
                faces_indices.push_back(num[i]);
            }

            object_test.obj_vector[object_test.index].NumOFFaces +=1;
            // object_test.obj_buffer.NumOFFaces +=1 ;

            break;

        default:
            /* cout something here! */
            break;
        }
        // int len2  = fputs(line, fd_des);
        // if(len2 < 0)
        // {
        //     printf("write failed:\n");
        //     break;
        // }
        // if(write = )
        // fputs(line, &fd_des);
    }

    if (line)
            free(line);

    ret = Objectfile.p_ObjetcAlloc();
    if(ret ==0 )
    {
        ret = Objectfile.fill_Objectbuffer();
        if(ret == 0)
        {
            for (int k=0; k<Objectfile.ObjectNum; k++)
            {
                cout << Objectfile.p_Object[k].object_tag << '\t' << Objectfile.p_Object[k].NumOFVertex_position << '\t' << Objectfile.p_Object[k].NumOFVertex_normal << '\t' << Objectfile.p_Object[k].NumOFTexture_coordinate << '\t' << Objectfile.p_Object[k].NumOFFaces << endl;
            }
        }
    }
    // int writen_len = 0;
    // Objectfile.write(fd_des, &writen_len);
    
    if(fd_des != NULL )
    {
        fclose(fd_des);
    }
    if(fd_src != NULL)
    {
        fclose(fd_src);
    }


    for(int i =0; i< object_test.obj_vector.size(); i++)
    {
        cout << "\tnumoffaces: " << object_test.obj_vector[i].NumOFFaces <<"  objtag" << object_test.obj_vector[i].object_tag << endl;
    }
    // cout << "f " << ends;
    // for (size_t i = 0; i < 9; i++)
    // {
    //     /* code */
    //     cout << " " << faces_indices[i + (object_test.obj_vector[0].NumOFFaces + object_test.obj_vector[1].NumOFFaces -1)*9] << ends; 
    // }

    // printf("\n");
    cout << "point Num " << v.size() << endl;
    cout << "Normal Num " << vn.size() << endl;
    cout << "Texture coord Num " << vt.size() << endl;
    cout << "face Num " << faces_indices.size() << endl;
    

    return 0;
}