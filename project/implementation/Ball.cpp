#include "Ball.hpp"
#include "bRenderer.h"

Ball::Ball()
{
    
}

Ball::Ball(vmml::Vector3f translation)
{
    position = translation;
    matrix = vmml::create_translation(position);
    radius = 2.f;
    hit = false;
}
