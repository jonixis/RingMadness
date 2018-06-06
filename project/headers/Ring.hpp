#ifndef Ring_hpp
#define Ring_hpp

#include "bRenderer.h"

class Ring{
public:
    vmml::Vector3f position;
    vmml::Matrix4f matrix;
    float radius;
    GLboolean hit;
    
    Ring();
    Ring(vmml::Vector3f translation);
};


#endif /* Ring_hpp */
