//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_METAL_H
#define RAYTRACE_METAL_H

#include "material.h"
#include "utils/utils.h"
#include "utils/pdf.h"

class Metal: public Material {
public:
    vec3 albedo;
    float fuzz;
    Metal(const vec3& albedo, float fuzz): albedo(albedo), fuzz(fuzz) {
        if (fuzz < 1) {
            this->fuzz = fuzz;
        } else {
            this->fuzz = 1;
        }
    }
    bool scatter(const ray& r_in, const hit_record& hrec, scatter_record &srec) const override {
//        HitablePDF pdf = HitablePDF(hrec.world->get_lights(), hrec.p);
//        vec3 i = unit_vector(pdf.sample());
//        vec3 n = hrec.normal;
//        vec3 o = -unit_vector(r_in.direction());
//        vec3 h = unit_vector(i + o);

//        float roughness = 0.5;
//        float D = roughness * roughness / (M_PI * pow(1 + (roughness * roughness - 1) * pow(dot(n, h), 2), 2));
//        float refract_idx = 1.5;
//        float f0 = pow((1 - refract_idx) / (1 + refract_idx), 2);
//        vec3 F = vec3(f0, f0, f0) + (vec3(1, 1, 1) - vec3(f0, f0, f0)) * pow(1 - fmax(0, dot(i, h)), 5);
//        float k = roughness * sqrt(2.0 / M_PI);
//        float schlick_i = dot(n, i) / (dot(n, i) * (1 - k) + k);
//        float schlick_o = dot(n, o) / (dot(n, o) * (1 - k) + k);
//        float G = schlick_i * schlick_o;
//        float den = 4 * dot(i, n) * dot(o, n);
//        float D = (roughness + 2) * pow(cos(theta), roughness + 1) * sin(theta) / (4 * dot(o, h));
//        float G = fmin(1, fmin(2 * dot(n, h) * dot(n, i) / dot(i, h), 2 * dot(n, h) * dot(n, o) / dot(o, h)));
//        float F = f0 + (1 - f0) * pow(1 - dot(i, h), 5);
//        float den = 4 * dot(i, n) * dot(o, n);
//        srec.attenuation = albedo * F * G * D / den;
//        srec.is_specular = true;
//        srec.specular_ray = ray(hrec.p, i);
//        srec.pdf_ptr = nullptr;
//        return true;

        vec3 reflected = reflect(unit_vector(r_in.direction()), hrec.normal);
        OrthogonalNormalBasis onb;
        onb.build_from_w(hrec.normal);
        auto out = unit_vector(reflected + fuzz * onb.local(random_in_unit_disk()));
        srec.specular_ray = ray(hrec.p, unit_vector(out), r_in.time());
        srec.attenuation = albedo;
        srec.is_specular = true;
        srec.pdf_ptr = nullptr;
        return true;
    }
    float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const override {
        float cosine = dot(rec.normal, unit_vector(scattered.direction()));
        if (cosine < 0) {
            cosine = 0;
        }
        return cosine / M_PI;
    }
};

#endif //RAYTRACE_METAL_H
