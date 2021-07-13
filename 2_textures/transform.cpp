#include <utils/glinit.h>
#include <shaders/shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

int main() {
    // create opengl context
    glfwSetup();
    // create window
    GLFWwindow* window = createWindow("2D Transform");
    // load opengl function pointers
    if (!glLoad()) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    // compile glsl shader
    Shader shader = Shader("../shaders/vertex/transform.vs", "../shaders/fragment/texture.fs");
   
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

    // initialize and assign vertex array, vertex buffer, and element buffer objects
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind VAO to opengl context 
    glBindVertexArray(VAO);
    // bind VBO and load vertex data 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // bind EBO and load element data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // initialize and enable vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // initialize and assign textures
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture1);
    // specify texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // flip data loaded from file
    stbi_set_flip_vertically_on_load(true);
    // initialize and assign texture information 
    int width, height, nChannels;
    unsigned char *data = stbi_load("../resources/container.jpg", &width, &height, &nChannels, 0);
    // generate texture image and mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    stbi_image_free(data);
    // bind another texture
    glBindTexture(GL_TEXTURE_2D, texture2);
    // specify texture parameters for new texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create texture and mipmap
    data = stbi_load("../resources/face.png", &width, &height, &nChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    // set shader program
    shader.use();
    // modify static shader uniforms
    shader.setInt("fTexture0", 0);
    shader.setInt("fTexture1", 1);

    // initialize dynamic shader uniforms
    float mixture = 0.2f; 
    // render loop 
    while (!glfwWindowShouldClose(window)) {
        // check for user input
        processInput(window);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            mixture += 0.001f;
            if (mixture >= 1.0f) mixture = 1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            mixture -= 0.001f;
            if (mixture <= 0.0f) mixture = 0.0f;
        }    

        // set dynamic shader uniforms
        shader.setFloat("mixture", mixture);
        // clear window 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 transMat = glm::mat4(1.0f);
        transMat = glm::translate(transMat, glm::vec3(0.5f, -0.5f, 0.0f));
        transMat = glm::rotate(transMat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMat4("transform", transMat);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

        transMat = glm::mat4(1.0f);
        transMat = glm::translate(transMat, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleValue = abs(sin(glfwGetTime()));
        transMat = glm::scale(transMat, glm::vec3(scaleValue, scaleValue, scaleValue));
        shader.setMat4("transform", transMat);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap window buffers and grab user input 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // delete assigned objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // close window, kill threads, and release resources 
    glfwTerminate();
    return 0;
}
