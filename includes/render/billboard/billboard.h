#pragma once

#include "./config.h"
#include "./render/shader.h"
#include "../dependencies/stb_image.h"

class Billboard 
{
    public:
        Billboard(Shader &shader);
        void draw(glm::dvec3 worldPos, glm::mat4 view, glm::mat4 projection);

    private:
        Shader *shader;
        unsigned int VAO, VBO, EBO, tex;
        int width, height, nrChannels;
};