#include "Ball.hpp"
#include "bRenderer.h"

Ball::Ball()
{
    
}

Ball::Ball(vmml::Vector3f translation)
{
    position = translation;
    matrix = vmml::create_translation(position) * vmml::create_scaling(vmml::Vector3f(2.0f,2.0f,2.f));
    radius = 2.f;
    hit = false;
}
