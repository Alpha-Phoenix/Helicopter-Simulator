#include <Utils.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 rotateV3(glm::vec3 &vector, float deg, glm::vec3 &axis)
{
    glm::mat4 rotM;
    rotM = glm::rotate(rotM, glm::radians(deg), axis);
    glm::vec4 vec(vector, 0.0f);
    glm::vec3 retVal = glm::vec3(rotM * vec);
    return retVal;
}


GLFWwindow *initOpenGL(const int width, const int height, const char *title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);
    return window;
}
