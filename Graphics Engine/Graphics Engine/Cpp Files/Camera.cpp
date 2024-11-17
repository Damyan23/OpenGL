#include <C:\Users\dpeyc\OneDrive\Desktop\Graphics Engine\Graphics Engine\Header Files\Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 up)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(2.5f), mouseSensitivity(0.02f), fov(45.0f), window(window), yaw(-90.0f), pitch(0.0f)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();

    glfwGetWindowSize(window, &width, &height);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio, float nearPlane, float farPlane) {
    return glm::perspective(glm::radians(GetFOV()), aspectRatio, nearPlane, farPlane);
}

void Camera::ProcessKeyboardInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += movementSpeed * front * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= movementSpeed * front * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= movementSpeed * right * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += movementSpeed * right * deltaTime;
}

void Camera::ProcessMouseMovement(float xpos, float ypos, bool constrainPitch) {
    // Initialize last position on first use
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate offsets with inverted y for correct orientation
    float xOffset = (xpos - lastX) * mouseSensitivity;
    float yOffset = (lastY - ypos) * mouseSensitivity;

    // Update the last known positions
    lastX = xpos;
    lastY = ypos;

    // Update yaw and pitch based on offsets
    yaw += xOffset;
    pitch += yOffset;

    // Constrain the pitch to avoid "flipping"
    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    // Update camera vectors to reflect new angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
    fov -= yOffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

glm::vec3 Camera::GetFront() const {
    return front;
}

float Camera::GetFOV() const {
    return fov;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::LookAt(glm::fvec3 lookPosition)
{
    glm::lookAt(lookPosition, lookPosition + front, up);
}
