#pragma once

#include <stddef.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// OpenGL
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// glimac
#include "glimac/common.hpp"
#include "p6/p6.h"

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
    }

    // Build final vertex array
    vertices.resize(vertexPosIndices.size(), glimac::ShapeVertex());

    for (size_t i = 0; i < vertices.size(); i++)
    {
        vertices[i].position  = vertexPositions[vertexPosIndices[i] - 1];
        vertices[i].normal    = vertexNormals[vertexNormalIndices[i] - 1];
        vertices[i].texCoords = vertexTexCoords[vertexTexCoordIndices[i] - 1];
    }
    std::cout << "OBJ file loaded" << std::endl;
    return vertices;
}
