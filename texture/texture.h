//
// Created by sicer on 2021/6/18.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "utils/vec3.h"

class Texture {
public:
    virtual ~Texture() = default;
    virtual vec3 value(float u, float v, const vec3 &p) const = 0;
    virtual void clear() = 0;
};

#endif //RAYTRACING_TEXTURE_H
