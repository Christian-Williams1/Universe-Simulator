#pragma once
#include "../config.h"

double newtons_method(double M,  double e);

glm::vec2 convert_to_spherical(glm::dvec3 &centre, glm::dvec3 &point);

glm::vec3 rotate_vector(glm::vec3 vector, glm::vec2 rotationVector);

glm::quat generate_quat(glm::vec3 u, glm::vec3 v, glm::quat Q = glm::quat(1, 0, 0, 0));