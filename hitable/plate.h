//
// Created by sicer on 2021/6/28.
//

#ifndef RAYTRACING_PLATE_H
#define RAYTRACING_PLATE_H

#include "hitable.h"
#include "material/material.h"

class XYPlate : public Hitable {
public:
    vec3 center;
    float small_r, big_r;
    Material *material_ptr;
    XYPlate() = default;
    XYPlate(const vec3 &center, float small_radius, float big_radius, Material *mat_ptr) : center(center),
        small_r(small_radius), big_r(big_radius), material_ptr(mat_ptr) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        float t = (center.z() - r.origin().z()) / r.direction().z();
        if (t < t_min || t > t_max) {
            return false;
        }
        vec3 p = r.point_at_parameter(t);
        if (!in_circle(p)) {
            return false;
        }
        rec.u = 0;
        rec.v = 0;
        rec.p = p;
        rec.t = t;
        if (center.z() < r.origin().z()) {
            rec.normal = vec3(0, 0, 1);
        } else {
            rec.normal = vec3(0, 0, -1);
        }
        rec.material_ptr = material_ptr;
        return true;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = AxisAlignedBoundingBox(center - vec3(big_r, big_r, 1e-5), center + vec3(big_r, big_r, 1e-5));
        return true;
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        ray r(origin, direction);
        hit_record rec;
        if (this->hit(r, 1e-5, MAXFLOAT, rec)) {
            float area = M_PI * (pow(big_r, 2) - pow(small_r, 2));
            float squared_distance = direction.squared_length() * rec.t * rec.t;
            float cosine = fabs(dot(unit_vector(direction), rec.normal));
            return squared_distance / (cosine * area);
        } else {
            return 0;
        }
    }
    vec3 random_direction(const vec3 &origin) const override {
        float theta = drand48() * 2 * M_PI;
        float distance = drand48() * (big_r - small_r) + small_r;
        vec3 point = vec3(center.x() + distance * cos(theta), center.y() + distance * sin(theta), center.z());
        return point - origin;
    }
    void clear() override {
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }

protected:
    bool in_circle(const vec3 point) const {
        return pow(point.x() - center.x(), 2) + pow(point.y() - center.y(), 2) >= pow(small_r, 2) &&
            pow(point.x() - center.x(), 2) + pow(point.y() - center.y(), 2) <= pow(big_r, 2);
    }
};

#endif //RAYTRACING_PLATE_H
