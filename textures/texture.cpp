#include <utils/glinit.h>
#include <shaders/shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

int main() {
    // create opengl context
    glfwSetup();
    // create window
    GLFWwindow* window = createWindow("2D Crate Texture Test");
    // load opengl function pointers
    if (!glLoad()) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    // compile glsl shader
    Shader shader = Shader("../shaders/vertex/texture.vs", "../shaders/fragment/texture.fs");
   
    float vertices[] = {
        // position           // colors           // coords 
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   -1.0f, -1.0f,
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   -1.0f,  2.0f,
         0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,    2.0f,  2.0f, 
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,    2.0f, -1.0f
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        0, 3, 2
    };
    // initialize and assign vertex array, vertex buffer, and element buffer
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nChannels;
    unsigned char *data = stbi_load("../resources/container.jpg", &width, &height, &nChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../resources/face.png", &width, &height, &nChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    shader.use();
    shader.setInt("fTexture0", 0);
    shader.setInt("fTexture1", 1);

    float mixture = 0.2f; 

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            mixture += 0.001f;
            if (mixture >= 1.0f) mixture = 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            mixture -= 0.001f;
            if (mixture <= 0.0f) mixture = 0.0f;
        }    
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.setFloat("mixture", mixture);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return 0;
}
