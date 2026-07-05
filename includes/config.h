#pragma once
// config folder

// this folder will contain the config class and all dependencies to link
#define GLM_ENABLE_EXPERIMENTAL

// *** Dependencies Folder Includes ***
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <stb_image.h>

// *** General Includes ***
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "math.h"

#include <vector>

// config class

#define PI 3.14159
#define G 1.0

namespace cfg {
    // win parameters
    inline float winWidth = 700.0f;
    inline float winHeight = 700.0f;

    // player parameters
    inline float speed = 40.0f;
    inline float sensitivity = 0.1f;
    inline float fov = 45.0f;

    // mathematical constants
    inline double timeSpeed = 1.0f;
}

// defining global simulation state
struct SimState {
    double timeScale = 0.0;
    double epoch = 0.0;
    double simTime = 0.0;
};

extern SimState glblState;