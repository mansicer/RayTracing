//
// Created by sicer on 2021/6/18.
//

#ifndef RAYTRACING_CHECKER_TEXTURE_H
#define RAYTRACING_CHECKER_TEXTURE_H

#include "texture.h"

class CheckerTexture : public Texture {
public:
    Texture * odd;
    Texture * even;
    CheckerTexture() = default;
    CheckerTexture(Texture *texture1, Texture *texture2): odd(texture1), even(texture2) {}
    vec3 value(float u, float v, const vec3 &p) const override {
        float sines = 1;
        sines *= sin(10 * p.x()) != 0 ? sin(10 * p.x()) : 1;
        sines *= sin(10 * p.y()) != 0 ? sin(10 * p.y()) : 1;
        sines *= sin(10 * p.z()) != 0 ? sin(10 * p.z()) : 1;
        if (sines < 0) {
            return odd->value(u, v, p);
        } else {
            return even->value(u, v, p);
        }
    }
    void clear() override {
        if (odd) {
            odd->clear();
            delete odd;
            odd = nullptr;
        }
        if (even) {
            even->clear();
            delete even;
            even = nullptr;
        }
    }
};

#endif //RAYTRACING_CHECKER_TEXTURE_H
