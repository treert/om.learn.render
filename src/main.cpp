#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "XSoftRender.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


std::shared_ptr<XSoftRender> soft_render;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void PrintDebugInfo() {

    auto prt_v3 = [](const xdata::vec3&v3) {
        printf("(%.1f,%.1f,%.1f)\n", v3.x, v3.y, v3.z);
    };

    std::cout << "camera pos: ";
    prt_v3(soft_render->camera.pos);
    std::cout << "camera dir: ";
    prt_v3(soft_render->camera.GetForwardDir());
    std::cout << "camera rot: ";
    prt_v3(soft_render->camera.eulers);
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
    Shader ourShader("soft_render.vs", "soft_render.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
    };
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    soft_render.reset(new XSoftRender());
    soft_render->light = new XLight();
    XGameObject *scene = XGameObject::Create();
    // scene->mesh.reset(XMesh::CreateSphere(5));
    // scene->pos = xdata::vec3(0, 0, 3);
    soft_render->light->SetParent(scene);
    soft_render->light->mesh.reset(XMesh::CreateSphere(0));
    soft_render->light->scale = xdata::vec3(0.3f);
    {
        auto *sphere = XGameObject::Create();
        sphere->mesh.reset(XMesh::CreateSphere(5));
        sphere->SetParent(scene);
        sphere->pos = xdata::vec3(0, 0, 3);
    }

    soft_render->camera.pos = xdata::vec3(3,0, -1);
    soft_render->camera.eulers = xdata::vec3(2, -49, 0);
    float fps = 0;

    float start_time = glfwGetTime();
    lastFrame = start_time;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        if (deltaTime < 0.0001f) continue;

        lastFrame = currentFrame;

        fps = fps * 0.99f + 0.01f*(1 / deltaTime);
        if (start_time + 3 < currentFrame) {
            start_time = currentFrame;
            printf("fps: %.1f tris: %d pixels: %d \n", fps, soft_render->stat_triangle_num, soft_render->stat_pixel_num);
            PrintDebugInfo();
        }

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // activate shader
        ourShader.use();

        soft_render->Render(scene);
        ourShader.setInt("ourTexture", 0);

        // render boxes
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 2*3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    XGameObject::Destroy(scene);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float xoffset = 0;
    float yoffset = 0;
    float sensitive = 10.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        xoffset -= sensitive;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        xoffset += sensitive;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        yoffset -= sensitive;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        yoffset += sensitive;

    // soft_render->light.eulers += xdata::vec3(xoffset, yoffset, 0);
    soft_render->camera.ProcessMouseMovement(xoffset, yoffset);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        soft_render->camera.ProcessKeyboard(XCamera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        soft_render->camera.ProcessKeyboard(XCamera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        soft_render->camera.ProcessKeyboard(XCamera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        soft_render->camera.ProcessKeyboard(XCamera::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

    // soft_render->camera.ProcessMouseMovement(xoffset, yoffset);
    soft_render->light->eulers += xdata::vec3(xoffset, yoffset, 0)*0.3f;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    soft_render->camera.ProcessMouseScroll(yoffset);
}