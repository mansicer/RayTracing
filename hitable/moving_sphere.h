//
// Created by sicer on 2021/7/7.
//

#ifndef RAYTRACING_MOVING_SPHERE_H
#define RAYTRACING_MOVING_SPHERE_H

#include "hitable.h"
#include "material/material.h"

class MovingSphere: public Hitable  {
public:
    MovingSphere() {}
    MovingSphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, Material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material_ptr(m)  {};
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        vec3 oc = r.origin() - center(r.time());
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;
        if (discriminant > 0) {
            float tmp = (-b - sqrt(b * b - a * c)) / a;
            if (tmp < t_max && tmp > t_min) {
                rec.t = tmp;
                rec.p = r.point_at_parameter(rec.t);
                get_sphere_uv((rec.p - center(r.time()))/radius, rec.u, rec.v);
                rec.normal = (rec.p - center(r.time())) / radius;
                rec.material_ptr = material_ptr;
                return true;
            }
            tmp = (-b + sqrt(b * b - a * c)) / a;
            if (tmp < t_max && tmp > t_min) {
                rec.t = tmp;
                rec.p = r.point_at_parameter(rec.t);
                get_sphere_uv((rec.p - center(r.time()))/radius, rec.u, rec.v);
                rec.normal = (rec.p - center(r.time())) / radius;
                rec.material_ptr = material_ptr;
                return true;
            }
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        AxisAlignedBoundingBox box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
        AxisAlignedBoundingBox box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
        bbox = surrounding_box(box0, box1);
        return true;
    }
    void clear() override {
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
    vec3 center(float time) const {
        return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
    }
    vec3 center0, center1;
    float time0, time1;
    float radius;
    Material *material_ptr;
};

#endif //RAYTRACING_MOVING_SPHERE_H
