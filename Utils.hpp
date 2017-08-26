#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <vector>
#include <iterator>

#define RGBA_WHITE 1.0f, 1.0f, 1.0f, 1.0f
#define RGBA_DARK_GREY 0.2f, 0.2f, 0.2f, 1.0f

const glm::vec3 origin(0.0f, 0.0f, 0.0f);
const glm::vec3 xaxis(1.0f, 0.0f, 0.0f);
const glm::vec3 yaxis(0.0f, 1.0f, 0.0f);
const glm::vec3 zaxis(0.0f, 0.0f, 1.0f);

glm::vec3 rotateV3(glm::vec3 &vector, float deg, glm::vec3 &axis);

template<typename T>
void split(const std::string &s, char delim, T &result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
}


GLFWwindow *initOpenGL(const int width, const int height, const char *title);

#endif // UTILS_HPP
