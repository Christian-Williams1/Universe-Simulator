#include "../includes/render/render_utils.h"

// void local_space_render_pass(Shader &shader, Player &player, Body &sun)
// {

// }

// void scale_space_render_pass(Shader &container, Shader &light, SphereRenderer sphereRenderer, Player &player, Body &sun)
// {
//     // update projection matrix
//     glm::mat4 projection = glm::mat4(1.0f);
//     projection = glm::perspective(glm::radians(cfg::fov), cfg::winWidth/cfg::winHeight, 10.0f, 1000.0f);

//     container.set_mat4("projection", projection);
//     light.set_mat4("projection", projection);

//     // render bodies
//     render_bodies()
// }


// void render_bodies(Shader &shader, Shader *light, Body *localSpaceBody, Body &body)
// {
//     // if sun we need a special case
//     if (light != nullptr)
//     {
//         glUseProgram(light->shaderID);
//     }
//     // render body
//     if (localSpaceBody == &body)
//     {

//     }
// }