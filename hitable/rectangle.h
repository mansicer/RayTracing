//
// Created by sicer on 2021/6/21.
//

#ifndef RAYTRACING_RECTANGLE_H
#define RAYTRACING_RECTANGLE_H

#include "material/material.h"

class XYRectangle: public Hitable {
public:
    float x0, x1, y0, y1, z;
    Material *material_ptr;
    XYRectangle() = default;
    XYRectangle(float xmin, float xmax, float ymin, float ymax, float z, Material *material): x0(xmin), x1(xmax),
        y0(ymin), y1(ymax), z(z), material_ptr(material) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        float t = (z - r.origin().z()) / r.direction().z();
        if (t < t_min || t > t_max) {
            return false;
        }
        float x = r.point_at_parameter(t).x();
        float y = r.point_at_parameter(t).y();
        if (x >= x0 && x <= x1 && y >= y0 && y <= y1) {
            rec.u = (x - x0) / (x1 - x0);
            rec.v = (y - y0) / (y1 - y0);
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            if (r.origin().z() > z) {
                rec.normal = vec3(0, 0, 1);
            } else {
                rec.normal = vec3(0, 0, -1);
            }
            rec.material_ptr = material_ptr;
            return true;
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = AxisAlignedBoundingBox(vec3(x0, y0, z - 0.00001), vec3(x1, y1, z + 0.00001));
        return true;
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        hit_record rec;
        if (this->hit(ray(origin, direction), 1e-5, MAXFLOAT, rec)) {
            float area = (y1 - y0) * (x1 - x0);
            float squared_distance = direction.squared_length() * rec.t * rec.t;
            float cosine = fabs(dot(unit_vector(direction), rec.normal));
            return squared_distance / (cosine * area);
        } else {
            return 0;
        }
    }
    vec3 random_direction(const vec3 &origin) const override {
        return vec3(x0 + drand48() * (x1 - x0), y0 + drand48() * (y1 - y0), z) - origin;
    }
    void clear() override {
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
};

class XZRectangle : public Hitable {
public:
    float x0, x1, z0, z1, y;
    Material * material_ptr;
    XZRectangle() = default;
    XZRectangle(float xmin, float xmax, float zmin, float zmax, float y, Material *material):
        x0(xmin), x1(xmax), z0(zmin), z1(zmax), y(y), material_ptr(material) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        float t = (y - r.origin().y()) / r.direction().y();
        if (t < t_min || t > t_max) {
            return false;
        }
        float x = r.point_at_parameter(t).x();
        float z = r.point_at_parameter(t).z();
        if (x >= x0 && x <= x1 && z >= z0 && z <= z1) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.u = (x1 - x) / (x1 - x0);
            rec.v = (z1 - z) / (z1 - z0);
            if (r.origin().y() > y) {
                rec.normal = vec3(0, 1, 0);
            } else {
                rec.normal = vec3(0, -1, 0);
            }
            rec.material_ptr = material_ptr;
            return true;
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = AxisAlignedBoundingBox(vec3(x0, y - 0.00001, z0), vec3(x1, y + 0.00001, z1));
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

class YZRectangle: public Hitable {
public:
    float x, y0, y1, z0, z1;
    Material *material_ptr;
    YZRectangle() = default;
    YZRectangle(float ymin, float ymax, float zmin, float zmax, float x, Material *material) :
        y0(ymin), y1(ymax), z0(zmin), z1(zmax), x(x), material_ptr(material) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        float t = (x - r.origin().x()) / r.direction().x();
        if (t < t_min || t > t_max) {
            return false;
        }
        float y = r.point_at_parameter(t).y();
        float z = r.point_at_parameter(t).z();
        if (y >= y0 && y <= y1 && z >= z0 && z <= z1) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.u = (y - y0) / (y1 - y0);
            rec.v = (z1 - z) / (z1 - z0);
            if (r.origin().x() > x) {
                rec.normal = vec3(1, 0, 0);
            } else {
                rec.normal = vec3(-1, 0, 0);
            }
            rec.material_ptr = material_ptr;
            return true;
        }
        return false;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = AxisAlignedBoundingBox(vec3(x - 0.00001, y0, z0), vec3(x + 0.00001, y1, z1));
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

class HitBox : public Hitable {
public:
    AxisAlignedBoundingBox box;
    vec3 pmin, pmax;
    Material *material_ptr;
    HitableList *list;
    HitBox() = default;
    HitBox(const vec3 &p0, const vec3 &p1, Material *material) : pmin(p0), pmax(p1), material_ptr(material) {
        std::list<Hitable *> l;
        l.push_back(new XYRectangle(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmin.z(), material));
        l.push_back(new XYRectangle(pmin.x(), pmax.x(), pmin.y(), pmax.y(), pmax.z(), material));
        l.push_back(new XZRectangle(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmin.y(), material));
        l.push_back(new XZRectangle(pmin.x(), pmax.x(), pmin.z(), pmax.z(), pmax.y(), material));
        l.push_back(new YZRectangle(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmin.x(), material));
        l.push_back(new YZRectangle(pmin.y(), pmax.y(), pmin.z(), pmax.z(), pmax.x(), material));
        list = new HitableList(l);
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        return list->hit(r, t_min, t_max, rec);
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        return list->bounding_box(t0, t1, bbox);
    }
    void clear() override {
        list->clear();
        if (material_ptr) {
            material_ptr->clear();
            delete material_ptr;
            material_ptr = nullptr;
        }
    }
};


#endif //RAYTRACING_RECTANGLE_H
