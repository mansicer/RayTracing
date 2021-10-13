//
// Created by sicer on 2021/6/21.
//

#ifndef RAYTRACING_TRANSFORMATION_H
#define RAYTRACING_TRANSFORMATION_H

#include "hitable.h"

class Translate: public Hitable {
public:
    Hitable *object;
    vec3 offset;
    Translate(Hitable *p, const vec3 &distance): object(p), offset(distance) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        ray move_ray(r.origin() - offset, r.direction(), r.time());
        if (object->hit(move_ray, t_min, t_max, rec)) {
            rec.p += offset;
            return true;
        } else {
            return false;
        }
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        if (object->bounding_box(t0, t1, bbox)) {
            bbox = AxisAlignedBoundingBox(bbox.min() + offset, bbox.max() + offset);
            return true;
        }
        else {
            return false;
        }
    }
    void clear() override {
        object->clear();
        if (object) {
            delete object;
            object = nullptr;
        }
    }
};

class ZRotation: public Hitable {
public:
    float sin_theta, cos_theta;
    Hitable *object;
    bool hasbox;
    AxisAlignedBoundingBox bbox;
    ZRotation(Hitable *ptr, float degree) : object(ptr) {
        float radians = (M_PI / 180) * degree;
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        hasbox = object->bounding_box(0, 1, bbox);
        vec3 min(MAXFLOAT, MAXFLOAT, MAXFLOAT);
        vec3 max(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                    float y = i * bbox.max().y() + (1 - j) * bbox.min().y();
                    float z = i * bbox.max().z() + (1 - z) * bbox.min().z();
                    float newx = cos_theta * x + sin_theta * y;
                    float newy = -sin_theta * x + cos_theta * y;
                    vec3 newpoint(newx, newy, z);
                    min = minimum(min, newpoint);
                    max = maximum(max, newpoint);
                }
            }
        }
        bbox = AxisAlignedBoundingBox(min, max);
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        vec3 origin = transformation(r.origin());
        vec3 direction = transformation(r.direction());
        ray rotated_r(origin, direction);
        if (object->hit(rotated_r, t_min, t_max, rec)) {
            rec.p = inv_transformation(rec.p);
            rec.normal = inv_transformation(rec.normal);
            return true;
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        if (hasbox) {
            bbox = this->bbox;
        }
        return hasbox;
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        return object->pdf_value(transformation(origin), transformation(direction));
    }
    vec3 random_direction(const vec3 &origin) const override {
        return object->random_direction(transformation(origin));
    }
    void clear() override {
        if (object) {
            object->clear();
            delete object;
            object = nullptr;
        }
    }

protected:
    vec3 transformation(const vec3 &p) const {
        vec3 result = p;
        result[0] = cos_theta * p.x() - sin_theta * p.y();
        result[1] = sin_theta * p.x() + cos_theta * p.y();
        return result;
    }
    vec3 inv_transformation(const vec3 &p) const {
        vec3 result = p;
        result[0] = cos_theta * p.x() + sin_theta * p.y();
        result[1] = -sin_theta * p.x() + cos_theta * p.y();
        return result;
    }
};

#endif //RAYTRACING_TRANSFORMATION_H
