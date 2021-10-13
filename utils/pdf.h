//
// Created by sicer on 2021/6/22.
//

#ifndef RAYTRACING_PDF_H
#define RAYTRACING_PDF_H

#include "utils.h"
#include "sample.h"
#include "hitable/hitable.h"

class PDF {
public:
    virtual ~PDF() = default;
    virtual float value(const vec3& direction) const = 0;
    virtual vec3 sample() const = 0;
};

class CosinePDF : public PDF {
public:
    OrthogonalNormalBasis onb;
    CosinePDF(const vec3 &w) {
        onb.build_from_w(w);
    }
    float value(const vec3 &direction) const override {
        float cosine = dot(unit_vector(direction), onb.w());
        if (cosine < 0) {
            return 0;
        } else {
            return cosine / M_PI;
        }
    }
    vec3 sample() const override {
        return onb.local(random_cosine_direction());
    }
};

class HitablePDF: public PDF {
public:
    Hitable *object;
    vec3 origin;
    HitablePDF(Hitable *obj, const vec3 &origin): object(obj), origin(origin) {}
    float value(const vec3 &direction) const override {
        return object->pdf_value(origin, direction);
    }
    vec3 sample() const override {
        return object->random_direction(origin);
    }
};

class MixedPDF: public PDF {
public:
    PDF &p1, &p2;
    MixedPDF(PDF &pdf1, PDF &pdf2) : p1(pdf1), p2(pdf2) {}
    float value(const vec3 &direction) const override {
        return 0.5 * p1.value(direction) + 0.5 * p2.value(direction);
    }
    vec3 sample() const override {
        if (drand48() < 0.5) {
            return p1.sample();
        } else {
            return p2.sample();
        }
    }
};

#endif //RAYTRACING_PDF_H
