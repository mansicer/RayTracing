//
// Created by sicer on 2021/4/18.
//

#ifndef RAYTRACE_PLANE_H
#define RAYTRACE_PLANE_H

#include "hitable.h"

class Plane: public Hitable {
public:
    float a, b, c, d;
    Material *material_ptr;
    // create a plane by the equation a * x + b * y + c * z = d
    Plane(float a, float b, float c, float d, Material *material_ptr): a(a), b(b), c(c), d(d), material_ptr(material_ptr) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        float numerator = - a * r.origin().x() - b * r.origin().y() - c * r.origin().z() + d;
        float denominator = a * r.direction().x() + b * r.direction().y() + c * r.direction().z();
        if (abs(denominator) < 1e-6) {
            return false;
        }
        float t = numerator / denominator;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.point_at_parameter(rec.t);
            if (dot(vec3(a, b, c), r.direction()) > 0) {
                rec.normal = unit_vector(-vec3(a, b, c));
            } else {
                rec.normal = unit_vector(vec3(a, b, c));
            }
            rec.material_ptr = material_ptr;
            return true;
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        return false;
    }
    void clear() override {
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
};

#endif //RAYTRACE_PLANE_H
