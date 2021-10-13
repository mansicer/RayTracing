//
// Created by sicer on 2021/6/19.
//

#ifndef RAYTRACING_DIFFUSE_LIGHT_H
#define RAYTRACING_DIFFUSE_LIGHT_H

#include "material.h"
#include "texture/texture.h"

class DiffuseLight : public Material {
public:
    Texture *emit;
    DiffuseLight(Texture *t) : emit (t) {}
    bool scatter(const ray& r_in, const hit_record& hrec, scatter_record &srec) const override {
        return false;
    }
    vec3 emitted(const ray &r_in, const hit_record &rec) const override {
        if (dot(rec.normal, r_in.direction()) < 0) {
            return emit->value(rec.u, rec.v, rec.p);
        } else {
            return vec3(0, 0, 0);
        }
    }
    void clear() override {
        emit->clear();
        delete emit;
        emit = nullptr;
    }
};

#endif //RAYTRACING_DIFFUSE_LIGHT_H
