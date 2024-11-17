#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    // Constructor with vectors
    Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 up);

    // Camera Attributes
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Getters for view and projection matrices
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float aspectRatio, float nearPlane, float farPlane);

    // Camera movement functions
    void ProcessKeyboardInput(float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

    // Getters for camera attributes
    glm::vec3 position;
    glm::vec3 GetFront() const;
    float GetFOV() const;

    //Other functions
    void LookAt(glm::fvec3 lookPosition);
private:
    GLFWwindow* window;

    // Euler Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float fov;

    int width;
    int height;

    float lastX;
    float lastY;

    bool firstMouse = true;

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

#endif // CAMERA_H
