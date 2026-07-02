// This file contains parameters to do with planetary bodies, natural satellites, etc.

#pragma once

#include "../config.h"
#include "../includes/simulation/orbit.h"

class Body
{
    public:
        Body(Body *parent, glm::vec3 color, KeplerianElements elements, float size, double mass);

        double get_size();
        glm::vec3 get_color();
        bool get_type();
        void set_colour(glm::vec3 colour);

        glm::vec3 next_position();

        void orbit_traverse(glm::dvec3 &position);

        std::vector<Body *> children;
        Body *parent;

        ~Body();

        Orbit *orbit;
        glm::dvec3 position;
    
    private:
        //std::vector<Body *> children;

        bool lightSource;
        double size;
        double mass;
        glm::vec3 colour;
};