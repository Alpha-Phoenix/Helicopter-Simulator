#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Mesh.hpp>
#include <Utils.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* TITLE = "OpenGL Helicopter Simulator";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

using namespace std;

int main()
{
    GLFWwindow* window = initOpenGL(SCR_WIDTH, SCR_HEIGHT, TITLE);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::mat4 view, projection;

    glm::vec3 observerPosition(3.0f, 3.0f, 3.0f);

    view = glm::lookAt(observerPosition, origin, yaxis);
    projection = glm::perspective(glm::radians(60.0f), float(SCR_WIDTH)/float(SCR_HEIGHT), 0.1f, 100.0f);

    Shader notexShader("shaders/notex.vert", "shaders/notex.frag");
    Mesh helicopter_helice_sup("models/helicopter/helice_sup.obj");
    Mesh helicopter_helice_back("models/helicopter/helice_back.obj");
    Mesh helicopter_cabine("models/helicopter/cabine.obj");
    Mesh helicopter_base("models/helicopter/base.obj");
    //Mesh monkey("models/monkey.obj");

    notexShader.use();
    notexShader.setMat4("view", view);
    notexShader.setMat4("projection", projection);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(RGBA_WHITE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;
        //model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
        //model = glm::rotate(model, glm::radians(30.0f) * (float)glfwGetTime(), xaxis);
        model = glm::rotate(model, glm::radians(30.0f) * (float)glfwGetTime(), yaxis);
        //model = glm::rotate(model, glm::radians(30.0f) * (float)glfwGetTime(), zaxis);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

        notexShader.use();
        notexShader.setMat4("model", model);

        helicopter_cabine.draw(notexShader);
        helicopter_helice_back.draw(notexShader);
        helicopter_base.draw(notexShader);
        helicopter_helice_sup.draw(notexShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}
