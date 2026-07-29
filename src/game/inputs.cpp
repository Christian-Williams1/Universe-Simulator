#include "../includes/game/inputs.h"

Inputs::Inputs(GLFWwindow *window, int shader, Player &player)
{
    this->window = window;

    this->player = &player;

    // initialising camera/mouse
    this->lastX = cfg::winWidth / 2.0f;
    this->lastY = cfg::winHeight / 2.0f;

    // setting perspective
    this->projection = glm::mat4(1.0f);
    this->projection = glm::perspective(glm::radians(cfg::fov), cfg::winWidth / cfg::winHeight, 0.1f, 10000.0f);

    this->keyDown = false;
}

void Inputs::set_projection(float &nearClip, float &farClip)
{
    this->projection = glm::perspective(glm::radians(cfg::fov), cfg::winWidth / cfg::winHeight, nearClip, farClip);
}

void Inputs::process_input(float dt)
{

    // static int followCol = -1;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    update_orientation();

    glm::quat worldOrientation = glm::normalize(qCorrection * orientation);

    glm::vec3 forward = glm::normalize(worldOrientation * glm::vec3(0.0f, 0.0f, -1.0f));
    glm::vec3 right = glm::normalize(worldOrientation * glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 up = glm::normalize(player->worldPos - player->get_body()->position);
    right = glm::normalize(glm::cross(forward, up));
    // forward = glm::normalize(forward - glm::dot(forward, up) * up);

    const float cameraSpeed = cfg::speed * dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player->worldPos += cameraSpeed * forward; //  cameraSpeed * glm::vec3{0.0f, 0.0f, 1.0f};
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player->worldPos -= cameraSpeed * forward; //  cameraSpeed * glm::vec3{0.0f, 0.0f, 1.0f};
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player->worldPos -= right * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player->worldPos += right * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        player->worldPos += cameraSpeed * up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        player->worldPos -= cameraSpeed * up;
    }

    // time scaling
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !keyDown)
    {
        keyDown = true;
        glblState.timeScale *= 2;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !keyDown)
    {
        keyDown = true;
        if (glblState.timeScale > 1)
            glblState.timeScale /= 2;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !keyDown)
    {
        keyDown = true;
        cfg::speed *= 2;
    }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !keyDown)
    {
        keyDown = true;
        if (cfg::speed > 1)
            cfg::speed /= 2;
    }
    else if (
        glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_Z) != GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS &&
        keyDown)
    {
        keyDown = false;
    }

    this->view = glm::mat4_cast(glm::inverse(worldOrientation));
}

// controlling rotational movement of the screen via mouse
void Inputs::process_mouse(double xoffset, double yoffset)
{
    xoffset *= cfg::sensitivity;
    yoffset *= cfg::sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    // accumulatedPitch += yoffset;
    // if (accumulatedPitch > 89.0f)
    // {
    //     accumulatedPitch = 89.0f;
    //     pitch = 0.0f;
    // }
    // if (accumulatedPitch < -89.0f)
    // {
    //     accumulatedPitch = -89.0f;
    //     pitch = 0.0f;
    // }
}

void Inputs::update_orientation()
{
    glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 front = orientation * glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 normal;

    if (player->get_local_space())
    {
        normal = -glm::normalize(player->worldPos - player->get_body()->position);
        qCorrection = glm::rotation(up, normal);// generate_quat(glm::vec3(0.0f, 1.0f, 0.0f), normal);
    }
    else
    {
        qCorrection = glm::quat(1.0f, glm::vec3(0.0f));
    }

    //qCorrection = glm::normalize(glm::slerp(previousGravityCorrection, qCorrection, 0.1f));
    //previousGravityCorrection = qCorrection;

    qYaw = glm::angleAxis(glm::radians(yaw), up);
    qPitch = glm::angleAxis(glm::radians(pitch), glm::normalize(right));

    // orthonormalize right

    orientation = qYaw*qPitch;
    orientation = glm::normalize(orientation);

    // reset movement
    // yaw = 0;
    // pitch = 0;
}