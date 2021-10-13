//
// Created by sicer on 2021/6/21.
//

#ifndef RAYTRACING_NOISE_TEXTURE_H
#define RAYTRACING_NOISE_TEXTURE_H

#include "texture.h"
#include "utils/noise.h"

class NoiseTexture : public Texture {
public:
    Perlin noise;
    float scale;
    NoiseTexture(float scale) : scale(scale) {};
    vec3 value(float u, float v, const vec3 &p) const override {
        return vec3(1,1,1) * 0.5 * (1 + sin(scale * p.x() + 5 * noise.turb(scale * p))) ;
    }
    void clear() override {}
};

#endif //RAYTRACING_NOISE_TEXTURE_H
