#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::lookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
    glm::vec3 zaxis = glm::normalize(position - target);
    glm::vec3 xaxis = glm::normalize(glm::cross(worldUp, zaxis));    
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    // set the translation matrix (OpenGL and GLM use column-major ordering)
    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    
    // set the rotation matrix
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x;
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x;
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.y;
    rotation[0][2] = zaxis.x;
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;
}

void Camera::processKeyboard(Camera_Movement direction, float delta) {
    float velocity = MovementSpeed * delta;
    if (direction == FORWARD) {
        Position += Front * velocity;
    }
    if (direction == LEFT) {
        Position -= Right * velocity;
    }
    if (direction == BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == RIGHT) {
        Position += Right * velocity;
    }

    Position.y = 0.0f;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom > 45.0f) {
        Zoom = 45.0f;
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
