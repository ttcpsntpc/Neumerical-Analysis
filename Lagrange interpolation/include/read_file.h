#ifndef READ_FILE_H
#define READ_FILE_H

#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "vertex.h"
using namespace std;

class ReadFile_c
{
private:
    //bool ReadTxtFile(const char *txt_filename);
    bool ReadInfFile(const char *inf_filename);
    bool ReadRawFile(const char *raw_filename);

public:
    ReadFile_c(const char *filename);
    ~ReadFile_c();

    struct InfData
    {
        int data_resolution[3];
        char * sample_type;
        float voxel_size[3];
        char * endian;
        glm::vec3 max, min;
    } inf_data;

    struct RawData
    {
        vector<vector<vector<int>>> voxel_data; // the value is distance field
        int size;
    } raw_data;

    // struct TxtData
    // {
    //     int data_resolution;
    //     int dimension;
    //     vector<glm::vec3> weight;
    //     glm::vec3 max, min;
    // } txt_data;
};

ReadFile_c::ReadFile_c(const char *filename)
{
    char *inf_filename = strcat(strdup(filename), ".inf");
    char *raw_filename = strcat(strdup(filename), ".raw");
    ReadInfFile(inf_filename);
    ReadRawFile(raw_filename);
    //ReadTxtFile(txt_filename);
    free(inf_filename);
    free(raw_filename);
}

ReadFile_c::~ReadFile_c()
{
}

bool ReadFile_c::ReadInfFile(const char *inf_filename)
{
    ifstream ifs(inf_filename, ios::in);
    if (ifs.fail())
    {
        cout << "Failed to open inf file." << '\n';
        return false;
    }

    string line;
    //string raw_file_name;
    while(getline(ifs, line))
    {
        size_t pos = line.find('=');
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        
        if(key == "raw-file")   
        {
            //raw_file_name = value;
        }
        else if(key == "resolution")
            sscanf(value.c_str(), "%dx%dx%d", 
                &inf_data.data_resolution[0], 
                &inf_data.data_resolution[1], 
                &inf_data.data_resolution[2]);
        else if(key == "sample-type")
            inf_data.sample_type = strdup(value.c_str());
        else if(key == "voxel-size")
            sscanf(value.c_str(), "%f:%f:%f", 
                &inf_data.voxel_size[0], 
                &inf_data.voxel_size[1], 
                &inf_data.voxel_size[2]);
        else if(key == "endian")
            inf_data.endian = strdup(value.c_str());
        else if(key == "max")
            sscanf(value.c_str(), "%f:%f:%f", 
                &inf_data.max.x, 
                &inf_data.max.y, 
                &inf_data.max.z);
        else if(key == "min")
            sscanf(value.c_str(), "%f:%f:%f", 
                &inf_data.min.x, 
                &inf_data.min.y, 
                &inf_data.min.z);
    }

    //return strdup(raw_file_name.c_str());
    return true;
}

bool ReadFile_c::ReadRawFile(const char *raw_filename)
{
    if(raw_filename == nullptr)
    {
        return false;
    }
    ifstream ifs(raw_filename, ios::in | ios::binary);
    if (ifs.fail())
    {
        cout << "Failed to open raw file." << '\n';
        return false;
    }

    int x = inf_data.data_resolution[0];
    int y = inf_data.data_resolution[1];
    int z = inf_data.data_resolution[2];

    for(int k = 0; k < z; ++k)
    {
        vector<vector<int>> temp_y;
        for(int j = 0; j < y; ++j)
        {
            vector<int> temp_x;
            for(int i = 0; i < x; ++i)
            {
                unsigned char value;
                ifs.read(reinterpret_cast<char*>(&value), sizeof(unsigned char));
                temp_x.push_back(static_cast<int>(value));
            }
            temp_y.push_back(temp_x);
        }
        raw_data.voxel_data.push_back(temp_y);
    }

    return true;
}

/*
bool ReadFile_c::ReadTxtFile(const char *txt_filename)
{
    ifstream ifs(txt_filename, ios::in);
    if (ifs.fail())
    {
        cout << "Failed to open file." << '\n';
        return false;
    }

    string my_line;

    getline(ifs, my_line, ' ');
    txt_data.data_resolution = stoi(my_line);
    getline(ifs, my_line, '\n');
    txt_data.dimension = stoi(my_line);

    glm::vec3 max_temp = glm::vec3(0.0f, 0.0f, 0.0f), min_temp = glm::vec3(9999.9f, 9999.9f, 9999.9f);
    for (int i = 0; i < txt_data.data_resolution; i++)
    {
        glm::vec3 temp;
        getline(ifs, my_line, ' ');
        temp.x = stof(my_line);
        if(temp.x > max_temp.x) max_temp.x = temp.x;
        else if (temp.x < min_temp.x) min_temp.x = temp.x;
        getline(ifs, my_line, ' ');
        temp.y = stof(my_line);
        if(temp.y > max_temp.y) max_temp.y = temp.y;
        else if (temp.y < min_temp.y) min_temp.y = temp.y;
        getline(ifs, my_line, ' ');
        temp.z = stof(my_line);
        if(temp.z > max_temp.z) max_temp.z = temp.z;
        else if (temp.z < min_temp.z) min_temp.z = temp.z;
        txt_data.weight.push_back(temp);
    }
    txt_data.max = max_temp;
    txt_data.min = min_temp;

    return true;
}
*/
#endif