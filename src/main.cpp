#include <iostream>
#include "../includes/config.h"
#include "../includes/render/shader.h"
#include "../includes/game/inputs.h"
#include "../includes/render/cube_sphere.h"
#include "../includes/render/sphere_render.h"
#include "../includes/render/cube_map.h"
#include "../includes/simulation/body.h"
#include "../includes/simulation/orbit.h"
#include "../includes/game/player.h"
#include "../includes/render/bloom.h"
#include "../includes/render/point_sphere.h"

// debug
#include "../debug/debug_camera.h"
#include "../debug/camera_mesh.h"

// void processInput(GLFWwindow *window, int shader);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// keeping track of time betweeen each frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// defining mouse movement vars
Inputs *glblInputs = nullptr;
bool firstMouse = true;
float lastX = cfg::winWidth / 2.0f;
float lastY = cfg::winHeight / 2.0f;

SimState glblState = {0.0, 1.0, 0.0};

// debugging states
bool debugMode = false;
DebugCamera *glblDebug = nullptr;

int main()
{
    // Setting up window
    if (!glfwInit())
    {
        std::cout << "Unable to open window" << std::endl;

        return -1;
    }

    // setting to version 430
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // initialising window and monitor
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    GLFWwindow *window;
    const GLFWvidmode *mode = glfwGetVideoMode(primary);

    if (primary == NULL)
    {
        std::cout << "Unable to fetch primary monitor" << std::endl;

        return -1;
    }

    if (mode == NULL)
    {
        std::cout << "Unable to initialise GLFWvidmode" << std::endl;

        return -1;
    }

    // setting win hints
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    // glfwWindowHint(GLFW_SAMPLES, 32);
    cfg::winWidth = static_cast<float>(mode->width);
    cfg::winHeight = static_cast<float>(mode->height);

    window = glfwCreateWindow(cfg::winWidth, cfg::winHeight, "program", NULL, NULL);
    glfwSetWindowPos(window, 0, 0);

    if (window == NULL)
    {
        std::cout << "Unable to load window" << std::endl;

        return -1;
    }

    glfwMakeContextCurrent(window);

    // callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, cfg::winWidth, cfg::winHeight);

    // defining simulation state default parameters
    glblState.timeScale = 1.0f;

    // compiling shaders
    Shader planetShader("../shaders/vertex.vert", "../shaders/planet.frag");
    Shader sunShader("../shaders/vertex.vert", "../shaders/sun.frag");

    Shader ppBloom("../shaders/bloom.vert", "../shaders/bloom.frag"); // swapped bloom and blur add to video
    Shader ppBlur("../shaders/bloom.vert", "../shaders/blur.frag");

    // debug camera
    Shader cameraShader("../shaders/camera.vert", "../shaders/camera.frag");

    // temporary starfield
    Shader starfieldShader("../shaders/starfield.vert", "../shaders/starfield.frag");
    StarField *starfield = new StarField(starfieldShader);

    // idk
    Shader shaders[4] = {sunShader, planetShader, starfieldShader, cameraShader};//sunShader,

    // enabling some random shi idk anymore kms
    glPointSize(1.5f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_MULTISAMPLE);

    // generating sphere
    CubeSphere cubeSphere(200);

    // creating planets
    float scaleFact = 1;
    Body *sun = new Body(nullptr, glm::vec3{1.0f, 0.9f, 0.3f}, {0, 0, 0, 0, 0}, scaleFact*100.0f, 10000.0f);
    Body *planet = new Body(sun, glm::vec3{1.0f, 0.5f, 0.25f}, {0.0, scaleFact*1080.0, 0.0, 0.0, 0.0}, scaleFact*50.0f, 500.0f);
    Body *planetT = new Body(sun, glm::vec3{1.0f, 0.05f, 0.09f}, {0.0, scaleFact*4000.0, 0.01, 0.01, 1.50}, scaleFact*70.0f, 700.0f);
    Body *planetTh = new Body(sun, glm::vec3{0.4f, 0.9f, 0.25f}, {0.0, scaleFact*8000.0, 0.09, 0.01, 0.90}, scaleFact*80.0f, 800.0f);
    Body *moon = new Body(planet, glm::vec3{0.4f, 0.5f, 0.25f}, {0.05, scaleFact*300.0, 0.01, 0.1, 0.0}, scaleFact*10.9f, 200.0f);
    Body *moonT = new Body(planet, glm::vec3{0.0f, 0.5f, 0.25f}, {0.05, scaleFact*450.0, 0.02, 0.2, 0.0}, scaleFact*10.9f, 200.0f);
    Body *moonTh = new Body(planetT, glm::vec3{0.5f, 0.5f, 0.5f}, {0.05, scaleFact*450.0, 0.03, 0.0, 0.0}, scaleFact*5.9f, 120.6f);
    Body *moonF = new Body(planet, glm::vec3{0.9f, 0.5f, 0.25f}, {0.05, scaleFact*600.0, 0.04, 0.1, 0.0}, scaleFact*1.9f, 50.2f);
    // Body *moonFi = new Body(planet, glm::vec3{0.8f, 0.8f, 0.8f}, {0.05, 10.0, 0.05, 0.1, 0.0}, 0.9f);

    sun->children.push_back(planet);
    sun->children.push_back(planetT);
    sun->children.push_back(planetTh);
    //planet->children.push_back(moon);
    planet->children.push_back(moonT);
    planetT->children.push_back(moonTh);
    // planetT->children.push_back(moonF);
    // planetT->children.push_back(moonFi);

    std::vector<Body *> bodies = {planet, planetT, planetTh, moonT, moonTh}; //moon, moonF, moonFi};
    SphereRenderer *sphereRender = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices, planetShader);

    SphereRenderer *daSun = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices, sunShader);

    // creating inputs class
    Player *player = new Player(*sun);
    Inputs inputs(window, planetShader.shaderID, *player);
    glblInputs = &inputs;

    // creating debug camera
    DebugCamera *debug = new DebugCamera(window);
    glblDebug = debug;
    CameraMesh *cameraMesh = new CameraMesh(cameraShader);

    // creating bloom effect class
    Bloom *bloom = new Bloom(ppBloom, ppBlur);

    glUseProgram(planetShader.shaderID);

    // enable face culling
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    while (!glfwWindowShouldClose(window))
    {
        // getting time between frames
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // setting global simulation time
        glblState.simTime += glblState.timeScale * deltaTime;

        // processing any inputs on the stack to update view matrix
        inputs.process_input(deltaTime);

        // updating orientation

        // check collision (basic)
        player->basic_collision_detection();
        if (debugMode)
        {
            debug->update_camera(deltaTime);
        }

        // clearing default buffer
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering to FBO
        glBindFramebuffer(GL_FRAMEBUFFER, bloom->FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // send updated inputs into respective shader programs
        if (!debugMode)
        {
            for (auto &shader : shaders)
            {
                glUseProgram(shader.shaderID); // remove

                int projLoc = glGetUniformLocation(shader.shaderID, "projection");
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(inputs.projection));

                int viewLoc = glGetUniformLocation(shader.shaderID, "view");
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(inputs.view));
            }
        }

        if (debugMode)
        {
            for (auto &shader : shaders)
            {
                glUseProgram(shader.shaderID); // remove

                int projLoc = glGetUniformLocation(shader.shaderID, "projection");
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(inputs.projection));

                int viewLoc = glGetUniformLocation(shader.shaderID, "view");
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(debug->view));
            }
        }

        // drawing temporary starfield
        glDisable(GL_DEPTH_TEST);
        starfield->draw();
        glEnable(GL_DEPTH_TEST);

        // update planet position
        glm::dvec3 sunPos = glm::dvec3{0.0f};
        sun->orbit_traverse(sunPos);

        // checking the players SOI and within critical radius
        player->check_critical_radius();
        player->check_soi();

        // rendering planets/sun
        bool scaleSpace = true;
        bool localSpace = player->get_local_space();

        // player follows planets when in SOI
        player->worldPos += player->get_body()->get_change_in_position();

        // player orbits planet when in critical radius
        // if (localSpace)
        // {
        //     glm::mat4 rot = glm::mat4(1.0f);
        //     rot = glm::translate(rot, glm::vec3(player->get_body()->position));
        //     rot = glm::rotate(rot, glm::radians(15.0f*deltaTime* (float)glblState.timeScale), glm::vec3(0.0f, 1.0f, 0.0f));
        //     rot = glm::translate(rot, -glm::vec3(player->get_body()->position));
        //     player->worldPos = glm::vec3(rot*glm::vec4(player->worldPos, 1.0f));
        // }

        // rendering light container (sun)
        glUseProgram(sunShader.shaderID);

        if (!(player->get_body() == sun && localSpace))
        {
            glUseProgram(sunShader.shaderID);
            daSun->draw(*sun, player->worldPos, scaleSpace);
        }

        // rendering containers (planets)
        glUseProgram(planetShader.shaderID);

        // drawing scale space
        for (int i = 0; i < bodies.size(); i++)
        {
            if (player->get_body() == bodies[i] && localSpace)
            {
                continue;
            }
            else
            {
                sphereRender->draw(*bodies[i], player->worldPos, scaleSpace);
            }
        }

        glClear(GL_DEPTH_BUFFER_BIT);

        if (localSpace)
        {
            if (player->get_body() == sun)
            {
                glUseProgram(sunShader.shaderID);

            }
            sphereRender->draw(*player->get_body(), player->worldPos, !scaleSpace);
        }

        if (debugMode)
        {
            cameraMesh->draw(inputs.projection, inputs.view, debug->projection, debug->view);
        }

        // final rendering to quad
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ppBlur.shaderID);
        bloom->draw_quad();

        glfwSwapBuffers(window);
        glfwPollEvents(); // remove events from stack
    }

    glBindVertexArray(0);
    glfwDestroyWindow(window);
    return 0;
}

// controlling rotational movement of the screen via mouse
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
    {
        glblInputs->process_mouse(xoffset, yoffset);
    }
    

    if (debugMode)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glblDebug->process_mouse(xoffset, yoffset);
        } 
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!glblDebug) return;

    glblDebug->process_scroll(yoffset);
}