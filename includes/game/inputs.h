#pragma once

#include "../config.h"
#include "player.h"
#include "../includes/math/math_utils.h"

class Inputs
{
public:
    // constructor
    Inputs(GLFWwindow *window, int shader, Player &player);

    // user interaction
    void process_mouse(double xoffset, double yoffset);
    void process_input(float dt);

    void set_projection(float &nearClip, float &farClip);

    glm::mat4 projection;

    glm::mat4 view;

private:
    GLFWwindow *window;

    // helper function
    void update_orientation();

    // camera vectors
    glm::quat orientation = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
    glm::quat pOrientation = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
    glm::quat targetOrientation = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);

    glm::quat qPitch;
    glm::quat qYaw;

    glm::quat qCorrection = glm::quat(1.0f, glm::vec3(0.0f));
    glm::quat previousGravityCorrection = glm::quat(1.0f, glm::vec3(0.0f));

    // mouse movements
    float yaw = -90.0f;
    float pitch = 0.0f;
    float accumulatedPitch = 0.0f;
    float lastX;
    float lastY;
    float fov;

    // player-> to see player state and determine the types of motion
    Player *player;

    bool keyDown;
};