#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // remove?
#include <glm/gtc/type_ptr.hpp> // remove?

#include <Shader.h>
#include <Camera.h>
#include <2D_Shapes/Cross.h>
#include <2D_Shapes/Circle.h>

#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderCrosses(Shader& shader, float angle, glm::vec3& pivotPoint);
void renderRevolvingCircles(Shader& shader, float angle, glm::vec3& pivotPoint);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float SCALE = 0.15f;
Cross cross = Cross(SCALE);
Circle centerCircle = Circle(0.025f);
Circle centerCircleBorder = Circle(0.033f);
Circle revolvingCircle = Circle(0.04f);
Circle revolvingCircleBorder = Circle(0.048f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("C:\\Users\\banni\\source\\repos\\learn-opengl\\learn-opengl\\resources\\shaders\\basic_shader\\shader.vs",
        "C:\\Users\\banni\\source\\repos\\learn-opengl\\learn-opengl\\resources\\shaders\\basic_shader\\shader.fs");

    ourShader.use();

    // orthographic projection stuff
    float left = -1.0f;
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;
    float nearPlane = -5.0f;
    float farPlane = 5.0f;
    glm::mat4 orthoProjection = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    ourShader.setMat4("projection", orthoProjection);

    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);

    cross.initialize();
    centerCircle.initialize();
    centerCircleBorder.initialize();
    revolvingCircle.initialize();
    revolvingCircleBorder.initialize();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();
        ourShader.setVec4("color", 0.75f, 0.75f, 0.75f, 1.0f);
        glm::vec3 pivotPoint = glm::vec3(0.0f, 0.0f, 0.0f);
        float angle = -(glm::pi<float>() / 5.0f) * glfwGetTime(); // configured for 1 revolution per 10 secs

        // render all crosses
        renderCrosses(ourShader, angle, pivotPoint);

        // render center circle
        ourShader.setVec4("color", 0.0f, 0.0f, 0.0f, 1.0f);
        centerCircleBorder.render(ourShader);
        ourShader.setVec4("color", 1.0f, 0.0f, 0.0f, 1.0f);
        centerCircle.render(ourShader);

        // render revolving circles
        renderRevolvingCircles(ourShader, angle, pivotPoint);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void renderCrosses(Shader& shader, float angle, glm::vec3& pivotPoint)
{
    float padding = SCALE / 3;
    float xMax, yMax;
    xMax = yMax = std::sqrt(2);
    float xMin, yMin;
    xMin = yMin = -std::sqrt(2);

    // center
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    cross.render(shader, position, angle, pivotPoint);
    // top right quadrent
    for (float y = 0; y <= yMax; y += cross.getWidth() + padding)
    {
        for (float x = 0; x <= xMax; x += cross.getWidth() + padding)
        {
            glm::vec3 position = glm::vec3(x, y, 0.0f);
            cross.render(shader, position, angle, pivotPoint);
        }
    }
    // bottom right quadrent
    for (float y = -(cross.getWidth() + padding); y >= yMin; y -= cross.getWidth() + padding)
    {
        for (float x = 0; x <= xMax; x += cross.getWidth() + padding)
        {
            glm::vec3 position = glm::vec3(x, y, 0.0f);
            cross.render(shader, position, angle, pivotPoint);
        }
    }
    // bottom left quadrent
    for (float y = -(cross.getWidth() + padding); y >= yMin; y -= cross.getWidth() + padding)
    {
        for (float x = -(cross.getWidth() + padding); x >= xMin; x -= cross.getWidth() + padding)
        {
            glm::vec3 position = glm::vec3(x, y, 0.0f);
            cross.render(shader, position, angle, pivotPoint);
        }
    }
    // top left quadrent
    for (float y = 0; y <= yMax; y += cross.getWidth() + padding)
    {
        for (float x = -(cross.getWidth() + padding); x >= xMin; x -= cross.getWidth() + padding)
        {
            glm::vec3 position = glm::vec3(x, y, 0.0f);
            cross.render(shader, position, angle, pivotPoint);
        }
    }
}

void renderRevolvingCircles(Shader& shader, float angle, glm::vec3& pivotPoint)
{
    int numberOfCircles = 3;
    float distanceFromCenter = 0.5f;
    for (int i = 0; i < numberOfCircles; i++)
    {
        float theta = i * (2 * M_PI / numberOfCircles);
        glm::vec3 position = glm::vec3(distanceFromCenter * cos(theta), distanceFromCenter * sin(theta), 0.0f);

        shader.setVec4("color", 0.0f, 0.0f, 0.0f, 1.0f);
        revolvingCircleBorder.render(shader, position, angle, pivotPoint);

        shader.setVec4("color", 0.07f, 0.55f, 0.88f, 1.0f);
        revolvingCircle.render(shader, position, angle, pivotPoint);
    }
}
