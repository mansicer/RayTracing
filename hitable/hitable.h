//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_HITABLE_H
#define RAYTRACE_HITABLE_H

#include "utils/ray.h"
#include "utils/boundingbox.h"

class Material;
class World;

struct hit_record {
    float t;
    float u, v;
    vec3 p;
    vec3 normal;
    Material *material_ptr;
    World *world;
};

class Hitable {
public:
    virtual ~Hitable() = default;
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
    virtual bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const = 0;
    virtual void clear() = 0;
    virtual float pdf_value(const vec3 &origin, const vec3 &direction) const {
        return 0;
    }
    virtual vec3 random_direction(const vec3 &origin) const {
        return vec3(0, 0, 0);
    }
};

#endif //RAYTRACE_HITABLE_H
