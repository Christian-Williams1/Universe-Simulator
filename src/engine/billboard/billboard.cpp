#include "../includes/render/billboard/billboard.h"

Billboard::Billboard(Shader &shader)
{
    this->shader = &shader;

        // generating 2D Quad
    const std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f    // top right
    };

    const std::vector<unsigned int> indices = 
    {
        0, 1, 2,
        1, 2, 3
    };

    // generating 2D quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // fetching texture
    unsigned char *data = stbi_load("../media/star_glow.png", &width, &height, &nrChannels, 4);

    std::cout 
        << "width: " << width 
        << " height: " << height 
        << " channels: " << nrChannels 
        << std::endl;

    // generating a texture
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load ./media/star_glow.png" << std::endl;
    }
    stbi_image_free(data);
}

void Billboard::draw(glm::dvec3 worldPos, glm::mat4 view, glm::mat4 projection)
{
    glUseProgram(shader->shaderID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    // calculating billboard position
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::scale(model, glm::vec3(10000));
    model = glm::translate(model, glm::vec3(0, 0, 10.0f));//-static_cast<glm::vec3>(worldPos));
    glm::mat4 MV = projection*view*model;
    shader->set_mat4("mv", MV);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    shader->set_int("tex", 0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}