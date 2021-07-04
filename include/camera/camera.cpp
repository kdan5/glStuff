#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(FOV) {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(FOV) {
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
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
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

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

void Camera::processMouseScroll(float yOffset) {
    Zoom -= (float)yOffset;
    if (Zoom < 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom > FOV) {
        Zoom = FOV;
    }
}

void Camera::updateCameraVectors() {
    glm::vec3 front = glm::vec3(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)), 
                                sin(glm::radians(Pitch)), 
                                cos(glm::radians(Yaw)) * sin(glm::radians(Pitch)));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front)); 
}

