#pragma once

#include <stddef.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// OpenGL
// #include "glfw/src/internal.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// glimac
#include "glimac/common.hpp"
#include "p6/p6.h"

// static std::vector<glimac::ShapeVertex> LoadOBJ(const char* file_name)
// {
//     // Vertex portions
//     std::vector<glm::fvec3> vertex_positions;
//     std::vector<glm::fvec2> vertex_texcoords;
//     std::vector<glm::fvec3> vertex_normals;

//     // Face vectors
//     std::vector<GLint> vertex_position_indicies;
//     std::vector<GLint> vertex_texcoord_indicies;
//     std::vector<GLint> vertex_normal_indicies;

//     // Vertex array
//     std::vector<glimac::ShapeVertex> vertices;

//     std::stringstream ss;
//     std::ifstream     in_file(file_name);
//     std::string       line   = "";
//     std::string       prefix = "";
//     glm::vec3         temp_vec3;
//     glm::vec2         temp_vec2;
//     GLint             temp_glint = 0;

//     // File open error check
//     if (!in_file.is_open())
//     {
//         throw "ERROR::OBJLOADER::Could not open file.";
//     }

//     // Read one line at a time
//     while (std::getline(in_file, line))
//     {
//         // Get the prefix of the line
//         ss.clear();
//         ss.str(line);
//         ss >> prefix;

//         if (prefix == "#")
//         {
//         }
//         else if (prefix == "o")
//         {
//         }
//         else if (prefix == "s")
//         {
//         }
//         else if (prefix == "use_mtl")
//         {
//         }
//         else if (prefix == "v") // Vertex position
//         {
//             ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
//             vertex_positions.push_back(temp_vec3);
//         }
//         else if (prefix == "vt")
//         {
//             ss >> temp_vec2.x >> temp_vec2.y;
//             vertex_texcoords.push_back(temp_vec2);
//         }
//         else if (prefix == "vn")
//         {
//             ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
//             vertex_normals.push_back(temp_vec3);
//         }
//         else if (prefix == "f")
//         {
//             int counter = 0;
//             while (ss >> temp_glint)
//             {
//                 // Pushing indices into correct arrays
//                 if (counter == 0)
//                     vertex_position_indicies.push_back(temp_glint);
//                 else if (counter == 1)
//                     vertex_texcoord_indicies.push_back(temp_glint);
//                 else if (counter == 2)
//                     vertex_normal_indicies.push_back(temp_glint);

//                 // Handling characters
//                 if (ss.peek() == '/')
//                 {
//                     ++counter;
//                     ss.ignore(1, '/');
//                 }
//                 else if (ss.peek() == ' ')
//                 {
//                     ++counter;
//                     ss.ignore(1, ' ');
//                 }

//                 // Reset the counter
//                 if (counter > 2)
//                     counter = 0;
//             }
//         }
//         else
//         {
//         }
//     }

//     // Build final vertex array (mesh)
//     vertices.resize(vertex_position_indicies.size(), glimac::ShapeVertex());

//     // Load in all indices
//     for (size_t i = 0; i < vertices.size(); ++i)
//     {
//         vertices[i].position  = vertex_positions[vertex_position_indicies[i] - 1];
//         vertices[i].texCoords = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
//         vertices[i].normal    = vertex_normals[vertex_normal_indicies[i] - 1];
//         // vertices[i].color    = glm::vec3(1.f, 1.f, 1.f);
//     }

//     // DEBUG
//     std::cout << "Nr of vertices: " << vertices.size() << "\n";

//     // Loaded success
//     std::cout << "OBJ file loaded!"
//               << "\n";
//     return vertices;
// }

static std::vector<glimac::ShapeVertex> LoadOBJ(const char* fileName)
{
    // Vertex coord
    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec3> vertexNormals;
    std::vector<glm::vec2> vertexTexCoords;

    // Face
    std::vector<GLuint> vertexPosIndices;
    std::vector<GLuint> vertexNormalIndices;
    std::vector<GLuint> vertexTexCoordIndices;

    // Vertex array
    std::vector<glimac::ShapeVertex> vertices;

    std::stringstream ss;
    std::ifstream     inFile(fileName);
    std::string       line   = "";
    std::string       prefix = "";
    glm::vec3         tempVec3;
    glm::vec2         tempVec2;
    GLint             tempGLint = 0;

    // Check error
    if (!inFile.is_open())
    {
        throw "ERROR::OBJLOADER::Could not open file.";
    }

    // Read one line
    while (std::getline(inFile, line))
    {
        // Get prefix of the line (v, vt, vn...)
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "#")
        {
        }
        else if (prefix == "o")
        {
        }
        else if (prefix == "s")
        {
        }
        else if (prefix == "use_mtl")
        {
        }
        // Vertex position
        else if (prefix == "v")
        {
            ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
            vertexPositions.push_back(tempVec3);
        }
        // Vertex TexCoord
        else if (prefix == "vt")
        {
            ss >> tempVec2.x >> tempVec2.y;
            vertexTexCoords.push_back(tempVec2);
        }
        // Vertex Normal
        else if (prefix == "vn")
        {
            ss >> tempVec3.x >> tempVec3.y >> tempVec3.z;
            vertexNormals.push_back(tempVec3);
        }
        else if (prefix == "f")
        {
            int counter = 0;
            while (ss >> tempGLint)
            {
                // Pushing indices into arrays
                if (counter == 0)
                {
                    vertexPosIndices.push_back(tempGLint);
                }
                else if (counter == 1)
                {
                    vertexTexCoordIndices.push_back(tempGLint);
                }
                else if (counter == 2)
                {
                    vertexNormalIndices.push_back(tempGLint);
                }
                // Ignoring character
                if (ss.peek() == '/')
                {
                    ++counter;
                    ss.ignore(1, '/');
                }
                else if (ss.peek() == ' ')
                {
                    ++counter;
                    ss.ignore(1, ' ');
                }
                // Reset counter
                if (counter > 2)
                {
                    counter = 0;
                }
            }
        }
        else
        {
        }
        // Build final vertex array
        vertices.resize(vertexPosIndices.size(), glimac::ShapeVertex());
        // std::cout << vertices.size() << std::endl;

        // std::cout << "vertexPositions size: " << vertexPositions.size() << std::endl;
        // std::cout << "vertexPosIndices size: " << vertexPosIndices.size() << std::endl;

        // std::cout << "vertexNormals size: " << vertexNormals.size() << std::endl;
        //  std::cout << "vertexNormalIndices size: " << vertexNormalIndices.size() << std::endl;
        // std::cout << "vertexTexCoords size: " << vertexTexCoords.size() << std::endl;
        //  std::cout << "vertexTexCoordIndices size: " << vertexTexCoordIndices.size() << std::endl;

        for (size_t i = 0; i < vertices.size(); i++)
        {
            vertices[i].position  = vertexPositions[vertexPosIndices[i] - 1];
            vertices[i].normal    = vertexNormals[vertexNormalIndices[i] - 1];
            vertices[i].texCoords = vertexTexCoords[vertexTexCoordIndices[i] - 1];
        }
        // DEBUG
        // std::cout << line << "\n";
        // std::cout << "Nb of vertices" << vertices.size() << std::endl;
    }
    std::cout << "OBJ file loaded" << std::endl;
    return vertices;
}
