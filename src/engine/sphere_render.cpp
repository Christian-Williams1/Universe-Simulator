#include "../includes/render/sphere_render.h"
#include "../includes/simulation/body.h"

SphereRenderer::SphereRenderer(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices, Shader &shader)
{
    initialise_container(vertices, indices);

    this->idxCount = indices.size(); // storing index count for static geometry of cube sphere
    this->shader = &shader;
}

void SphereRenderer::initialise_container(std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void SphereRenderer::draw(Body &body, glm::dvec3 worldPos, bool scaleSpace)
{
    glBindVertexArray(VAO);

    // passing colour
    glm::vec3 color = body.get_color();
    
    int vertexColorLoc = glGetUniformLocation(shader->shaderID, "col");
    glUniform3f(vertexColorLoc, color.x, color.y, color.z);

    // passing model matrix
    glm::mat4 model = glm::mat4{1.0f};
    glm::dvec3 relativePos = body.position - worldPos;
    glm::vec3 bodySize = glm::vec3{(float)body.get_size()};
    // if scale space render scale both relative pos and body size
    if (scaleSpace)
    {
        relativePos = glm::vec3{relativePos}*scaleFactor;
        bodySize = bodySize*scaleFactor;
    }
    
    model = glm::translate(model, glm::vec3(relativePos));
    model = glm::scale(model, bodySize);

    int modelLoc = glGetUniformLocation(shader->shaderID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // passing inverse transformed normal map -> keep for later
    //glm::mat3 transInvModel = glm::transpose(glm::inverse(model));

    //int transInvModelLoc = glGetUniformLocation(shader.shaderID, "transposeInverseModel");
    //glUniformMatrix3fv(transInvModelLoc, 1, GL_FALSE, glm::value_ptr(transInvModel));

    // passing worldPos
    int bodyPosLoc = glGetUniformLocation(shader->shaderID, "worldPos");

    glm::vec3 bodyPosUniform = glm::vec3(body.position);
    glUniform3fv(bodyPosLoc, 1, glm::value_ptr(bodyPosUniform));

    glDrawElements(GL_TRIANGLES, idxCount, GL_UNSIGNED_INT, 0);
}

SphereRenderer::~SphereRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}