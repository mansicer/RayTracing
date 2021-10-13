//
// Created by sicer on 2021/6/22.
//

#ifndef RAYTRACING_SAMPLE_H
#define RAYTRACING_SAMPLE_H

#include "utils.h"

class OrthogonalNormalBasis {
public:
    vec3 axis[3];
    OrthogonalNormalBasis() = default;
    inline vec3 operator[](int i) const {
        return axis[i];
    }
    vec3 u() const {
        return axis[0];
    }
    vec3 v() const {
        return axis[1];
    }
    vec3 w() const {
        return axis[2];
    }
    vec3 local(float a, float b, float c) const {
        return a * u() + b * v() + c * w();
    }
    vec3 local(const vec3 &p) const {
        return p.x() * u() + p.y() * v() + p.z() * w();
    }
    void build_from_w(const vec3 &n) {
//        axis[0] = unit_vector(n);
//        vec3 a;
//        if (fabs(u().y()) > 0.9) {
//            a = vec3(0, 0, 1);
//        } else {
//            a = vec3(0, 1, 0);
//        }
//        axis[1] = unit_vector(cross(u(), a));
//        axis[2] = cross(u(), v());
        axis[2] = unit_vector(n);
        vec3 a;
        if (fabs(w().x()) > 0.9) {
            a = vec3(0, 1, 0);
        } else {
            a = vec3(1, 0, 0);
        }
        axis[1] = cross(w(), a);
        axis[0] = cross(w(), v());
//        axis[0] = cross(w(), a);
//        axis[1] = cross(w(), u());
    }
};

#endif //RAYTRACING_SAMPLE_H
