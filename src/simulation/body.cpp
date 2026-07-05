#include "../includes/simulation/body.h"

Body::Body(Body *parent, glm::vec3 color, KeplerianElements elements, float size, double mass)
{
    this->parent = parent;
    this->mass = mass;//1.0f;
    if (parent != nullptr)
    {
        this->parent = parent;
        this->mass = mass;//1.0f;
        this->orbit = new Orbit(parent->mass, mass, elements);
        this->lightSource = true;
    }
    else
    {
        this->orbit = new Orbit(0.0, mass, elements);
        this->position = glm::vec3{0.0f};
    }

    this->size = size;
    this->colour = color;
}

// getters
double Body::get_size()
{
    return this->size;
}

glm::vec3 Body::get_color()
{
    return this->colour;
}

bool Body::get_type()
{
    return this->lightSource;
}

glm::dvec3 Body::get_change_in_position()
{
    return position - previousPosition ;
}

// setters
void Body::set_colour(glm::vec3 colour)
{
    this->colour = colour;
}

void Body::orbit_traverse(glm::dvec3 &position)
{
    this->previousPosition = this->position;
    this->position = orbit->calculate_position() + position;

    for (auto &child : children)
    {
        child->orbit_traverse(this->position);
    }
}



glm::vec3 Body::next_position()
{
    position = orbit->calculate_position();

    return position + parent->position;
}

Body::~Body()
{
    delete this->orbit;
    this->orbit = nullptr;
}