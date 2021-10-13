//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_SPHERE_H
#define RAYTRACE_SPHERE_H

#include "hitable.h"
#include "../material/material.h"

class Sphere: public Hitable {
public:
    vec3 center;
    float radius;
    Material *material_ptr;

    Sphere(vec3 cen, float r, Material* mat_ptr): center(cen), radius(r), material_ptr(mat_ptr) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;
        if (discriminant > 0) {
            float tmp = (-b - sqrt(b * b - a * c)) / a;
            if (tmp < t_max && tmp > t_min) {
                rec.t = tmp;
                rec.p = r.point_at_parameter(rec.t);
                get_sphere_uv((rec.p - center)/radius, rec.u, rec.v);
                rec.normal = (rec.p - center) / radius;
                rec.material_ptr = material_ptr;
                return true;
            }
            tmp = (-b + sqrt(b * b - a * c)) / a;
            if (tmp < t_max && tmp > t_min) {
                rec.t = tmp;
                rec.p = r.point_at_parameter(rec.t);
                get_sphere_uv((rec.p - center)/radius, rec.u, rec.v);
                rec.normal = (rec.p - center) / radius;
                rec.material_ptr = material_ptr;
                return true;
            }
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = AxisAlignedBoundingBox(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
        return true;
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        hit_record rec;
        if (this->hit(ray(origin, direction), 1e-5, MAXFLOAT, rec)) {
            float cos_theta_max = sqrt(1 - radius * radius / (center - origin).squared_length());
            float solid_angle = 2 * M_PI * (1 - cos_theta_max);
            return 1 / solid_angle;
        } else {
            return 0;
        }
    }
    vec3 random_direction(const vec3 &origin) const override {
        vec3 direction = center - origin;
        float distance_squared = direction.squared_length();
        OrthogonalNormalBasis onb;
        onb.build_from_w(direction);
        return onb.local(random_to_sphere(radius, distance_squared));
    }
    void clear() override {
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
};

#endif //RAYTRACE_SPHERE_H
