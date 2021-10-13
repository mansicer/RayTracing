//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_DIELECTRIC_H
#define RAYTRACE_DIELECTRIC_H

#include "material.h"

float schlick(float cosine, float refract_index) {
    float r0 = (1 - refract_index) / (1 + refract_index);
    r0 *= r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

class Dielectric: public Material {
public:
    float refract_index;
    Dielectric(float refract_index): refract_index(refract_index) {}
    bool scatter(const ray& r_in, const hit_record& hrec, scatter_record &srec) const override {
        srec.is_specular = true;
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), hrec.normal);
        float ni_over_nt;
        srec.attenuation = vec3(1, 1, 1);
        vec3 refracted;
        float reflected_prob;
        float cosine;
        if (dot(r_in.direction(), hrec.normal) > 0) {
            outward_normal = -hrec.normal;
            ni_over_nt = refract_index;
            cosine = refract_index * dot(r_in.direction(), hrec.normal) / r_in.direction().length();
        } else {
            outward_normal = hrec.normal;
            ni_over_nt = 1.0 / refract_index;
            cosine = -dot(r_in.direction(), hrec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflected_prob = schlick(cosine, refract_index);
        } else {
            reflected_prob = 1;
        }
        if (drand48() < reflected_prob) {
            srec.specular_ray = ray(hrec.p, reflected, r_in.time());
        } else {
            srec.specular_ray = ray(hrec.p, refracted, r_in.time());
        }
        return true;
    }
    void clear() override {
    }
};

#endif //RAYTRACE_DIELECTRIC_H
