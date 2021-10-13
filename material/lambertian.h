//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_LAMBERTIAN_H
#define RAYTRACE_LAMBERTIAN_H

#include "material.h"

class Lambertian: public Material {
public:
    Texture * albedo;
    Lambertian(Texture *a): albedo(a) {}
    bool scatter(const ray& r_in, const hit_record& hrec, scatter_record &srec) const override {
        srec.is_specular = false;
        srec.attenuation = albedo->value(hrec.u, hrec.v, hrec.p);
        srec.pdf_ptr = new CosinePDF(hrec.normal);
        return true;
    }
    float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override {
        float cosine = dot(rec.normal, unit_vector(scattered.direction()));
        if (cosine < 0) {
            cosine = 0;
        }
        return cosine / M_PI;
    }
    void clear() override {
        if (albedo) {
            albedo->clear();
            delete albedo;
            albedo = nullptr;
        }
    }
};

#endif //RAYTRACE_LAMBERTIAN_H
