// This file contains parameters to do with planetary bodies, natural satellites, etc.

#pragma once

#include "../config.h"
#include "../includes/simulation/orbit.h"

class Body
{
    public:
        Body(Body *parent, glm::vec3 color, KeplerianElements elements, float size, double mass);

        // getters 
        double get_size();
        glm::vec3 get_color();
        bool get_type();
        glm::dvec3 get_change_in_position();

        void set_colour(glm::vec3 colour);

        glm::vec3 next_position();

        void orbit_traverse(glm::dvec3 &position);

        std::vector<Body *> children;
        Body *parent;

        ~Body();

        Orbit *orbit;
        glm::dvec3 position;
    
    private:

        glm::dvec3 deltaPosition;
        glm::dvec3 previousPosition;

        bool lightSource;

        double size;
        double mass;
        glm::vec3 colour;
};