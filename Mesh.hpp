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
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
public:
    Mesh(const char *modelPath);
    virtual ~Mesh();
    void draw(const Shader &shader);
    void rotate(GLfloat radians, glm::vec3 axis);
    void translate(glm::vec3 position);
    void scale(GLfloat scaleFactor);
};

#endif // MESH_H
