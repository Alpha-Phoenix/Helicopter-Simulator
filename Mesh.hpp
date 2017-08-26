#ifndef MESH_H
#define MESH_H

#include <learnopengl/shader_m.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#define VERTEX_BUFFER_INDEX     0
#define TEXCOORD_BUFFER_INDEX   1
#define NORMAL_BUFFER_INDEX     2
#define VBO_SIZE                3

enum ObjFlag {
    VERTEX   = 1 << 0,
    NORMAL   = 1 << 1,
    TEXCOORD = 1 << 2,
    COMPLETE = VERTEX | NORMAL | TEXCOORD
};

class Mesh
{
public:
    GLuint vao;
    GLuint vbo[VBO_SIZE];
    GLuint modelFLag;
    GLuint textureID;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    glm::vec3 pivot;
public:
    Mesh(const char *modelPath, const char *texturePath = nullptr, glm::vec3 pivot = glm::vec3(0));
    virtual ~Mesh();
    void draw(const Shader &shader);
    void rotate(GLfloat radians, glm::vec3 axis, glm::vec3 pivot);
    void rotate(GLfloat radians, glm::vec3 axis);
    void translate(glm::vec3 position);
    void setPivot(glm::vec3 pivot = glm::vec3(0));
};

#endif // MESH_H
