#include <utils/glinit.h>
#include <shaders/shader.h>
#include <camera/camera.h>
#define GLFW_INCLUDE_NONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const float PI = 3.14159;

// window settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// camera setup
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float prevXPos = SCREEN_WIDTH / 2.0f;
float prevYPos = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float delta = 0.0f;
float prevFrame = 0.0f;

// light source position
glm::vec3 lightPos(0.0f, 3.0f, -2.0f);

int main() {
    // create opengl context
    setupGLFWContext();
    // create window
    GLFWwindow* window = createWindow("Basic Light Show");

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load opengl function pointers
    if(!glLoad()) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // compile glsl shaders
    Shader objShaders[] = {
        Shader("../shaders/vertex/normals.vs", "../shaders/fragment/material.fs"),
        Shader("../shaders/vertex/normals.vs", "../shaders/fragment/phong.fs"),
        Shader("../shaders/vertex/normals_model.vs", "../shaders/fragment/phong_model.fs"),
        Shader("../shaders/vertex/normals_model.vs", "../shaders/fragment/specular.fs"),
        Shader("../shaders/vertex/normals_model.vs", "../shaders/fragment/diffuse.fs"),
        Shader("../shaders/vertex/colors.vs", "../shaders/fragment/ambient.fs")
    };

    Shader lightSourceShader = Shader("../shaders/vertex/colors.vs", "../shaders/fragment/light_source.fs");

    // vertex coordinates

    float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glm::vec3 cubePos[] = {
        glm::vec3(-2.0f,  1.0f,  0.0f),
        glm::vec3( 0.0f,  1.0f,  0.0f),
        glm::vec3( 2.0f,  1.0f,  0.0f),
        glm::vec3( 2.0f, -1.0f,  0.0f),
        glm::vec3( 0.0f, -1.0f,  0.0f),
        glm::vec3(-2.0f, -1.0f,  0.0f) 
    };

    // initialize buffer and array objects for cube
    unsigned int VBO, VAO; 
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // configure position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // initialize VAO for light source object
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    // bind VBO to lightVAO
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // light position attribute (same vertices as the cube)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color of rendered object and light source
    glm::vec3 objColor = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    while(!glfwWindowShouldClose(window)) {
        // framerate timing logic
        float currentFrame = glfwGetTime();
        delta = currentFrame - prevFrame;
        prevFrame = currentFrame;

        // input
        processInput(window);

        // render window 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // setup ambient shader
        objShaders[5].use();
        objShaders[5].setVec3("objColor", objColor);
        objShaders[5].setVec3("lightColor", lightColor);

        // setup diffuse shader
        objShaders[4].use();
        objShaders[4].setVec3("objColor", objColor);
        objShaders[4].setVec3("lightColor", lightColor);
        objShaders[4].setVec3("lightPos", lightPos);
        
        // setup specular shader
        objShaders[3].use();
        objShaders[3].setVec3("objColor", objColor);
        objShaders[3].setVec3("lightColor", lightColor);
        objShaders[3].setVec3("lightPos", lightPos);
        objShaders[3].setVec3("viewPos", camera.Position);
        
        // setup phong shaders
        objShaders[2].use();
        objShaders[2].setVec3("objColor", objColor); 
        objShaders[2].setVec3("lightColor", lightColor);
        objShaders[2].setVec3("lightPos", lightPos);
        objShaders[2].setVec3("viewPos", camera.Position);

        // setup view shader (same as model phong shader)
        objShaders[1].use();
        objShaders[1].setVec3("objColor", objColor); 
        objShaders[1].setVec3("lightColor", lightColor);
        objShaders[1].setVec3("lightPos", lightPos);
        objShaders[1].setVec3("viewPos", camera.Position);

        // material shader
        objShaders[0].use();
        objShaders[0].setFloat("material.shine", 32.0f);
        glm::vec3 materialAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
        glm::vec3 materialDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
        objShaders[0].setVec3("material.ambient", materialAmbient);
        objShaders[0].setVec3("material.diffuse", materialDiffuse);
        objShaders[0].setVec3("material.specular", materialSpecular);
        objShaders[0].setVec3("light.position", lightPos);
        objShaders[0].setVec3("viewPos", camera.Position);
        glm::vec3 lightSource = glm::vec3(1.0f);
        glm::vec3 diffuseSource = lightSource * glm::vec3(0.5f);
        glm::vec3 ambientSource = diffuseSource * glm::vec3(0.2f);
        glm::vec3 specularSource = glm::vec3(1.0f, 1.0f, 1.0f);
        objShaders[0].setVec3("light.ambient", ambientSource);
        objShaders[0].setVec3("light.diffuse", diffuseSource);
        objShaders[0].setVec3("light.specular", specularSource);

        // projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);         
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // bind vertex array for cube object
        glBindVertexArray(VAO);

        // world transformations
        for (unsigned int i = 0; i < 6; i++) {
            Shader shader = objShaders[i];
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);
            model = glm::translate(glm::mat4(1.0f), cubePos[i]);
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        } 

        // bind vertex array for light source object
        glBindVertexArray(lightVAO);

        // set lighting shader
        lightSourceShader.use();
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        float t = glfwGetTime() / 2.0f;
        lightPos.x = 3.0f * cos(3.0f * t);
        lightPos.y = 0.0f;
        lightPos.z = 3.0f * sin(3.0f * t);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightSourceShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

/**
 * Process all user input.
 */
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.processKeyboard(UP, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera.processKeyboard(DOWN, delta);
    }
}

/**
 * GLFW window size callback.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


/**
 * GLFW mouse/trackpad callback.
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        prevXPos = xpos;
        prevYPos = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - prevXPos;
    float yoffset = prevYPos - ypos;

    prevXPos = xpos;
    prevYPos = ypos;

    camera.processMouseMovement(xoffset, yoffset, true);
}

/**
 * GLFW mouse scroll callback.
 */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(yoffset);
}

