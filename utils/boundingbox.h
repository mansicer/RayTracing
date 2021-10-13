//
// Created by sicer on 2021/6/17.
//

#ifndef RAYTRACING_BOUNDINGBOX_H
#define RAYTRACING_BOUNDINGBOX_H

#include "ray.h"

class AxisAlignedBoundingBox {
public:
    AxisAlignedBoundingBox() = default;
    AxisAlignedBoundingBox(const vec3 &a, const vec3 &b): _min(a), _max(b) {}

    vec3 min() const { return _min; }
    vec3 max() const { return _max; }

    bool hit(const ray &r, float tmin, float tmax) const {
        for (int a = 0; a < 3; a++) {
            float invD = 1.0 / r.direction()[a];
            float t0 = (min()[a] - r.origin()[a]) * invD;
            float t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0) {
                std::swap(t0, t1);
            }
            tmin = fmax(tmin, t0);
            tmax = fmin(tmax, t1);
            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }

    vec3 _min;
    vec3 _max;
};

AxisAlignedBoundingBox surrounding_box(AxisAlignedBoundingBox &a, AxisAlignedBoundingBox &b) {
    vec3 small = vec3(fmin(a.min().x(), b.min().x()), fmin(a.min().y(), b.min().y()), fmin(a.min().z(), b.min().z()));
    vec3 big = vec3(fmax(a.max().x(), b.max().x()), fmax(a.max().y(), b.max().y()), fmax(a.max().z(), b.max().z()));
    return AxisAlignedBoundingBox(small, big);
}

#endif //RAYTRACING_BOUNDINGBOX_H
