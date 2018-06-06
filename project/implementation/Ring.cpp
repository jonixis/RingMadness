#include "Ring.hpp"
#include "bRenderer.h"

Ring::Ring()
{
    
}

Ring::Ring(vmml::Vector3f translation)
{
    position = translation;
    matrix = vmml::create_translation(position) * vmml::create_scaling(vmml::Vector3f(3.0f,3.0f,3.f));
    radius = 2.f;
    hit = false;
}
