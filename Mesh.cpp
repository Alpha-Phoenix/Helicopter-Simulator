#include <Mesh.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <Utils.hpp>
#include <glad/glad.h>

Mesh::Mesh(const char *modelPath)
{
    std::ifstream modelFile(modelPath);
    std::string line, lineHeader;

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempCoords;

    GLuint modelFlag = 0;

    glm::vec3 tempVertex;
    glm::vec3 tempNormal;
    glm::vec2 tempCoord;

    while(std::getline(modelFile, line)) {
        lineHeader = line.substr(0, line.find_first_of(" "));
        line = line.substr(line.find_first_of(" ") + 1);
        std::istringstream iss(line);

        if (lineHeader == "v") {
            iss >> tempVertex.x >> tempVertex.y >> tempVertex.z;
            tempVertices.push_back(tempVertex);
            modelFlag |= VERTEX;

        } else if (lineHeader == "vt") {
            iss >> tempCoord.x >> tempCoord.y;
            tempCoords.push_back(tempCoord);
            modelFlag |= TEXCOORD;

        } else if (lineHeader == "vn") {
            iss >> tempNormal.x >> tempNormal.y >> tempNormal.z;
            tempNormals.push_back(tempNormal);
            modelFlag |= NORMAL;

        } else if (lineHeader == "f") {
            std::vector<std::string> triple;
            split<std::vector<std::string>>(line, ' ', triple);

            for (std::string s : triple) {
                int vertexIndex = -1;
                int normalIndex = -1;
                int texCoordIndex = -1;
                char slashChar = '\0';
                iss.str(s);
                iss.seekg(std::ios_base::beg);

                if (modelFlag & VERTEX) {
                    iss >> vertexIndex >> slashChar;
                    vertexIndex--;
                    glm::vec3 v = tempVertices[vertexIndex];
                    this->vertices.push_back(v);
                    this->indices.push_back(vertexIndex);
                }

                if (modelFlag & TEXCOORD) {
                    iss >> texCoordIndex >> slashChar;
                    texCoordIndex--;
                    glm::vec2 c = tempCoords[texCoordIndex];
                    this->texcoords.push_back(c);
                } else {
                    iss >> slashChar;
                }

                if (modelFlag & NORMAL) {
                    iss >> normalIndex;
                    normalIndex--;
                    glm::vec3 n = tempNormals[normalIndex];
                    this->normals.push_back(n);
                }
            }
        }
    }

    modelFile.close();

    this->modelFLag = modelFlag;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(VBO_SIZE, this->vbo);

    if (this->modelFLag & VERTEX) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[VERTEX_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(VERTEX_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(VERTEX_BUFFER_INDEX);
    }

    if (this->modelFLag & TEXCOORD) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[TEXCOORD_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->texcoords.size() * sizeof(glm::vec2), &this->texcoords[0], GL_STATIC_DRAW);
        glVertexAttribPointer(TEXCOORD_BUFFER_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(TEXCOORD_BUFFER_INDEX);
    }

    if (this->modelFLag & NORMAL) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[NORMAL_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(NORMAL_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(NORMAL_BUFFER_INDEX);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(VBO_SIZE, this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

void Mesh::draw(const Shader &shader)
{
    glBindVertexArray(this->vao);
    shader.use();
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() * 3);
}
