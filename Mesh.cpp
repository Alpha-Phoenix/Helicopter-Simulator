#include <Mesh.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <Utils.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <learnopengl/stb_image.h>

Mesh::Mesh(const char *modelPath, const char *texturePath, glm::vec3 pivot)
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
    this->pivot = pivot;
    this->modelFLag = modelFlag;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(VBO_SIZE, this->vbo);

    if (this->modelFLag & VERTEX) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[VERTEX_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(VERTEX_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(VERTEX_BUFFER_INDEX);
    }

    if (this->modelFLag & TEXCOORD) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[TEXCOORD_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->texcoords.size() * sizeof(glm::vec2), &this->texcoords[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(TEXCOORD_BUFFER_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(TEXCOORD_BUFFER_INDEX);

        // load and create a texture
        // -------------------------
        glGenTextures(1, &this->textureID);
        glBindTexture(GL_TEXTURE_2D, this->textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    if (this->modelFLag & NORMAL) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[NORMAL_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_DYNAMIC_DRAW);
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
    if(this->modelFLag & TEXCOORD) {
        glBindTexture(GL_TEXTURE_2D, this->textureID);
    }
    shader.use();
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() * 3);
}

void Mesh::rotate(GLfloat radians, glm::vec3 axis, glm::vec3 pivot)
{
    glm::mat4 rotationMatrix, rotPivot;
    rotationMatrix = glm::translate(rotationMatrix, pivot);
    rotationMatrix = glm::rotate(rotationMatrix, radians, axis);
    rotationMatrix = glm::translate(rotationMatrix, -pivot);

    for (size_t i = 0; i < this->vertices.size(); i++) {
        // Updating Vertices
        glm::vec3 vertex = this->vertices[i];
        glm::vec4 vertex4(vertex, 1.0f);
        vertex4 = rotationMatrix * vertex4;
        this->vertices[i] = glm::vec3(vertex4);

        // Updating Normals
        glm::vec3 normal = this->normals[i];
        glm::vec4 normal4(normal, 0.0f);
        normal4 = rotationMatrix * normal4;
        this->normals[i] = glm::vec3(normal4);
    }

    if(pivot != this->pivot) {
        rotPivot = glm::translate(rotPivot, pivot);
        rotPivot = glm::rotate(rotPivot, radians, axis);
        rotPivot = glm::translate(rotPivot, -pivot);
        this->pivot = glm::vec3(rotPivot * glm::vec4(this->pivot, 1.0f));
    }

    glBindVertexArray(this->vao);

    if (this->modelFLag & VERTEX) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[VERTEX_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(VERTEX_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(VERTEX_BUFFER_INDEX);
    }

    if (this->modelFLag & NORMAL) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[NORMAL_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(NORMAL_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(NORMAL_BUFFER_INDEX);
    }
}

void Mesh::rotate(GLfloat radians, glm::vec3 axis)
{
    this->rotate(radians, axis, this->pivot);
}

void Mesh::translate(glm::vec3 position)
{
    glm::mat4 translationMatrix;
    translationMatrix = glm::translate(translationMatrix, position);
    for (size_t i = 0; i < this->vertices.size(); i++) {
        // Updating Vertices
        glm::vec3 vertex = this->vertices[i];
        glm::vec4 vertex4(vertex, 1.0f);
        vertex4 = translationMatrix * vertex4;
        this->vertices[i] = glm::vec3(vertex4);
    }
    this->pivot += position;

    glBindVertexArray(this->vao);

    if (this->modelFLag & VERTEX) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo[VERTEX_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(VERTEX_BUFFER_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0));
        glEnableVertexAttribArray(VERTEX_BUFFER_INDEX);
    }
}

void Mesh::setPivot(glm::vec3 pivot)
{
    this->pivot = pivot;
}
