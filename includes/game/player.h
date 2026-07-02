#pragma once

#include "../config.h"
#include "./simulation/body.h"

typedef enum {
    FREEMODE,
    GRAVITY
} Mode;

class Player
{
    public:
        Player(Body &body);
        void check_soi();
        void check_critical_radius();
        bool get_local_space();
        Body *get_body();
        void basic_collision_detection();

        glm::dvec3 worldPos;
        float criticalRadiusScalar = 1.05;
    private:
        bool localSpace = false;
        Body *body;
        Mode mode;
};