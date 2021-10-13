//
// Created by sicer on 2021/3/11.
//

#ifndef RAYTRACE_RAY_H
#define RAYTRACE_RAY_H

#include "utils.h"

class ray {
protected:
    vec3 A;
    vec3 B;
    float _time;
public:
    ray() {}
    ray(const vec3& a, const vec3& b, float ti = 0.0) {
        A = a; B = b; _time = ti;
    }
    vec3 origin() const {
        return A;
    }
    vec3 direction() const {
        return B;
    }
    float time() const {
        return _time;
    }
    vec3 point_at_parameter(float t) const {
        return A + t * B;
    }
};

#endif //RAYTRACE_RAY_H
