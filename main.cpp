#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Mesh.hpp>
#include <Utils.hpp>
#include <learnopengl/camera.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* TITLE = "OpenGL Helicopter Simulator";

// camera
Camera camera(glm::vec3(0.0f, 4.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(3.0f, 3.0f, 3.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Helicopter
glm::vec3 helicopterForward = glm::vec3(-zaxis);
glm::vec3 helicopterUp = glm::vec3(yaxis);
glm::vec3 helice_tras_pivot(0.11770f, 2.15358, 4.82899);
glm::vec3 helice_sup_pivot(0.0f, 2.95383f, 0.00268f);

float helicopterRotationForce = 0.0f;
const float helicopterMaxRotationForce = 30.0f;

void processPhysics(GLFWwindow *window);

using namespace std;

int main()
{
    GLFWwindow* window = initOpenGL(SCR_WIDTH, SCR_HEIGHT, TITLE);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader lightingShader("shaders/lighting.vert", "shaders/lighting.frag");
    Mesh cabine("models/helicopter/cabine.obj", "textures/base_texture.jpg");
    Mesh helice_tras("models/helicopter/helice_back.obj", "textures/base_texture.jpg");
    Mesh helice_sup("models/helicopter/helice_sup.obj", "textures/base_texture.jpg");
    Mesh base("models/helicopter/base.obj", "textures/base_texture.jpg");
    Mesh floor("models/floor.obj", "textures/floor_texture.jpg");

    lightingShader.use();
    lightingShader.setVec3("lampColor", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setVec3("lampPos", lightPos);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        processPhysics(window);

        glClearColor(RGBA_DARK_GREY);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);
        lightingShader.setVec3("viewPos", camera.Position);

        cabine.draw(lightingShader);

        helice_tras.draw(lightingShader);
        helice_tras.rotate(glm::radians(45.0f) * deltaTime * helicopterRotationForce, xaxis, helice_tras_pivot);

        helice_sup.rotate(glm::radians(45.0f) * deltaTime * helicopterRotationForce, yaxis, helice_sup_pivot);
        helice_sup.draw(lightingShader);

        base.draw(lightingShader);
        floor.draw(lightingShader);


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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void processPhysics(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        helicopterRotationForce += 0.1f;
        if(helicopterRotationForce >= helicopterMaxRotationForce)
            helicopterRotationForce = helicopterMaxRotationForce;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        helicopterRotationForce -= 0.1f;
        if(helicopterRotationForce <= 0)
            helicopterRotationForce = 0;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)window;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void)window;
    (void)xoffset;
    camera.ProcessMouseScroll(yoffset);
}
