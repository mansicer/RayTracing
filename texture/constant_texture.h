//
// Created by sicer on 2021/6/18.
//

#ifndef RAYTRACING_CONSTANT_TEXTURE_H
#define RAYTRACING_CONSTANT_TEXTURE_H

#include "texture.h"

class ConstantTexture : public Texture {
public:
    vec3 color;
    ConstantTexture() = default;
    ConstantTexture(float r, float g, float b): color(vec3(r, g, b)) {}
    ConstantTexture(vec3 c): color(c) {}
    vec3 value(float u, float v, const vec3 &p) const override {
        return color;
    }
    void clear() override {}
};

#endif //RAYTRACING_CONSTANT_TEXTURE_H
