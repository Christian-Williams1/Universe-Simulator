#include "../includes/math/math_utils.h"

double newtons_method(double M,  double e)
{
    double soln[2] = {0, 0};

    // calculating first approximation
    soln[0] = M + e*sin(M) * (1.0 + e*cos(M));
    
    for (int i = 0; i < 10; i++)
    {
        soln[1] = soln[0] - (soln[0]-M-e*sin(soln[0]))/(1-e*cos(soln[0]));

        if (abs(soln[1] - soln[0]) < 1e-6)
        {
            break;
        }

        soln[0] = soln[1];
    }

    return soln[1];
}

glm::vec2 convert_to_spherical(glm::dvec3 &centre, glm::dvec3 &point)
{
    double distance = glm::distance(centre, point);
    glm::dvec3 relativePos = centre - point;

    // converting relative position to angles in spherical coordinates
    // angles.y = phi
    // angles.x = theta
    glm::vec2 angles;
    angles.y = glm::acos((float)(relativePos.z/distance));
    angles.x = glm::atan((float)(relativePos.y, relativePos.x));

    return angles;
}

glm::vec3 rotate_vector(glm::vec3 vector, glm::vec2 rotationVector)
{
    glm::vec3 up = glm::vec3{0.0f, 0.0f, 1.0f};
    glm::vec3 front = glm::vec3{1.0f, 0.0f, 0.0f};

    // creating rotation matrix
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, rotationVector.y, up);
    rotationMatrix = glm::rotate(rotationMatrix, rotationVector.x, front);

    // rotating vector
    glm::vec3 finalVector = glm::vec3(rotationMatrix*glm::vec4(vector, 1.0f));

    return finalVector;
}

// finds the quaternion representing the rotation from u->v.
// default Q gives the shortest rotation.
// the return will give the quat closest to Q that rotates u -> v
glm::quat generate_quat(glm::vec3 u, glm::vec3 v, glm::quat Q)
{
    glm::quat rotation;

    // normalizing vectors
    u = glm::normalize(u);
    v = glm::normalize(v);

    //special cases??
    float d = glm::dot(u, v);
    if (d > 0.999999f)
    {
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    if (d < -0.999999f)
    {
        return glm::quat(0, glm::normalize(glm::cross(glm::vec3{0.0f, 0.0f, 1.0f}, u)));
    }

    glm::vec3 crossProduct = glm::cross(u,v);
    rotation.x = crossProduct.x;
    rotation.y = crossProduct.y;
    rotation.z = crossProduct.z;
    rotation.w = glm::sqrt(1+glm::dot(u, v));

    return glm::normalize(rotation);
}