//
// Created by sicer on 2021/7/5.
//

#ifndef RAYTRACING_POLYGON_H
#define RAYTRACING_POLYGON_H

#include "hitable.h"
#include "material/material.h"

class Triangle: public Hitable {
public:
    vec3 p0, p1, p2;
    vec3 normal;
    float d;
    Material *material_ptr;
    Triangle(const vec3 &p0, const vec3 &p1, const vec3 &p2, Material *mat_ptr) : p0(p0), p1(p1), p2(p2), material_ptr(mat_ptr) {
        normal = unit_vector(cross(p1 - p0, p2 - p0));
        d = dot(normal, p0);
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        if (dot(normal, r.direction()) < 1e-6) {
            return false;
        }
        float t = (d - dot(normal, r.origin())) / dot(normal, r.direction());
        if (t < t_min || t > t_max ) {
            return false;
        }
        vec3 p = r.point_at_parameter(t);
        vec3 R = p - p0;
        vec3 q1 = p1 - p0;
        vec3 q2 = p2 - p0;
        float gen = 1.0 / (q1.squared_length() * q2.squared_length() - pow(dot(q1, q2), 2));
        float w1 = (q2.squared_length() * dot(R, q1) - dot(q1, q2) * dot(R, q2)) * gen;
        float w2 = (-dot(q1, q2) * dot(R, q1) + q1.squared_length() * dot(R, q2)) * gen;
        if (w1 < 0 || w2 < 0 || w1 + w2 > 1) {
            return false;
        }
        rec.t = t;
        rec.p = p;
        if (dot(normal, r.direction()) > 0) {
            rec.normal = -normal;
        } else {
            rec.normal = normal;
        }
        rec.material_ptr = material_ptr;
        return true;
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        hit_record rec;
        if (this->hit(ray(origin, direction), 1e-5, MAXFLOAT, rec)) {
            float a = (p1 - p0).length(), b = (p2 - p1).length(), c = (p2 - p1).length();
            float p = (a + b + c) / 2;
            float area = sqrt(p * (p - a) * (p - b) * (p - c));
            float squared_distance = direction.squared_length() * rec.t * rec.t;
            float cosine = fabs(dot(unit_vector(direction), rec.normal));
            return squared_distance / (cosine * area);
        } else {
            return 0;
        }
    }
    vec3 random_direction(const vec3 &origin) const override {
        float w0 = drand48(), w1 = drand48(), w2 = drand48();
        float z = w0 + w1 + w2;
        w0 = w0 / z;
        w1 = w1 / z;
        w2 = w2 / z;
        vec3 p = w0 * p0 + w1 * p1 + w2 * p2;
        return p - origin;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        auto min = vec3(MAXFLOAT, MAXFLOAT, MAXFLOAT), max = vec3(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
        min = minimum(minimum(minimum(min, p0), p1), p2);
        max = maximum(maximum(maximum(max, p0), p1), p2);
        if (min.x() == max.x()) min[0] -= 1e-5;
        if (min.y() == max.y()) min[1] -= 1e-5;
        if (min.z() == max.z()) min[2] -= 1e-5;
        bbox = AxisAlignedBoundingBox(min, max);
        return true;
    }
    void clear() override {
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
};

class Polygon: public Hitable {
public:
    std::vector<vec3> points;
    Material *material_ptr;
    HitableList *plist;
    Polygon(std::vector<vec3> points, Material *material) : points(points), material_ptr(material) {
        std::list<Hitable *> hlist;
        auto start = points.front();
        for (int i = 1; i < points.size() - 1; i++) {
            hlist.push_back(new Triangle(start, points[i], points[i+1], material));
        }
        plist = new HitableList(hlist);
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        return plist->hit(r, t_min, t_max, rec);
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        return plist->bounding_box(t0, t1, bbox);
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        return plist->pdf_value(origin, direction);
    }
    vec3 random_direction(const vec3 &origin) const override {
        return plist->random_direction(origin);
    }
    void clear() override {
        if (plist) {
            plist->clear();
            delete plist;
            plist = nullptr;
        }
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
};

#endif //RAYTRACING_POLYGON_H
