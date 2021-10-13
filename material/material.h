//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_MATERIAL_H
#define RAYTRACE_MATERIAL_H

#include "hitable/hitable.h"
#include "utils/sample.h"
#include "utils/pdf.h"
#include "texture/texture.h"

struct scatter_record {
    ray specular_ray;
    bool is_specular;
    vec3 attenuation;
    PDF *pdf_ptr;
};

class Material {
public:
    virtual ~Material() = default;
    virtual bool scatter(const ray& r_in, const hit_record& hrec, scatter_record &srec) const = 0;
    virtual float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const {
        return false;
    }
    virtual vec3 emitted(const ray &r_in, const hit_record &rec) const {
        return vec3(0, 0, 0);
    }
    virtual void clear() {}
};

#endif //RAYTRACE_MATERIAL_H
