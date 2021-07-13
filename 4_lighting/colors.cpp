#include <utils/glinit.h>
#include <objShaders/shader.h>
#include <camera/camera.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera setup
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float prevXPos = SCR_WIDTH / 2.0f;
float prevYPos = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float delta = 0.0f;
float prevFrame = 0.0f;

// light source position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
    // create opengl context
    setupGLFWContext();
    // create window
    GLFWwindow* window = createWindow("Colors");
    // load opengl function pointers
    if(!glLoad()) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    // compile glsl shaders
    Shader objShader = Shader("../shaders/vertex/colors.vs", "../shaders/fragments/colors.fs");
    Shader lightShader = Shader("../shaders/vertex/colors.vs", "../shaders/fragments/light_source.fs");
    // vertex coordinates
    float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
    };

    // initialize buffer and array objects for cube
    unsigned int VBO, VAO; 
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // configure position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // initialize VAO for light object
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    // bind VBO to lightVAO
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // light position attribute (same vertices as the cube)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window)) {
        // framerate timing logic
        float currentFrame = glfwGetTime();
        delta = currentFrame - prevFrame;
        prevFrame = currentFrame;

        // input
        processInput(window);

        // render window color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // activate objShader 
        objShader.use();
        objShader.setVec3("objColor", glm::vec3(1.0f, 1.0f, 1.0f));
        objShader.vetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));     
         
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

