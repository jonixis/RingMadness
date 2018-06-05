#ifndef Ball_hpp
#define Ball_hpp

#include "bRenderer.h"

class Ball{
public:
    vmml::Vector3f position;
    vmml::Matrix4f matrix;
    float radius;
    GLboolean hit;
    
    Ball();
    Ball(vmml::Vector3f translation);
};


#endif /* Ball_hpp */
