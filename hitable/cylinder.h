//
// Created by sicer on 2021/4/17.
//

#ifndef RAYTRACE_CYLINDER_H
#define RAYTRACE_CYLINDER_H

#include "hitable.h"

class Cylinder: public Hitable {
protected:

public:
    float radius;
    float z_min, z_max;
    float x, y;
    Material *material_ptr;
    Cylinder(float x, float y, float z_min, float z_max, float radius, Material* material_ptr) :
        x(x), y(y), z_min(z_min), z_max(z_max), radius(radius), material_ptr(material_ptr) {
        if (z_min > z_max) {
            std::swap(this->z_min, this->z_max);
        }
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        vec3 oc = vec3(r.origin().x() - x, r.origin().y() - y, r.origin().z());
        float a = pow(r.direction().x(), 2) + pow(r.direction().y(), 2);
        float b = 2 * (r.direction().x() * oc.x() + r.direction().y() * oc.y());
        float c = pow(oc.x(), 2) + pow(oc.y(), 2) - pow(radius, 2);
        float discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            float t1 = (-b - sqrt(discriminant)) / (2 * a);
            float t2 = (-b + sqrt(discriminant)) / (2 * a);

            float t3, t4;
            bool increasing = true;
            if (r.direction().z() > 0) {
                t3 = (z_min - r.origin().z()) / r.direction().z();
                t4 = (z_max - r.origin().z()) / r.direction().z();
                increasing = true;
            } else {
                t3 = (z_max - r.origin().z()) / r.direction().z();
                t4 = (z_min - r.origin().z()) / r.direction().z();
                increasing = false;
            }

            if (t1 < t3) {
                if (t2 < t3) {
                    // t1 < t2 < t3 < t4
                    if (t1 < t_max && t1 > t_min && in_z_range(r, t1)) {
                        set_record_side(r, t1, rec);
                        return true;
                    } else if (t2 < t_max && t2 > t_min && in_z_range(r, t2)) {
                        set_record_side(r, t2, rec);
                        return true;
                    } else if (t3 < t_max && t3 > t_min && in_circle_range(r, t3)) {
                        set_record_bottom(r, t3, rec, !increasing);
                        return true;
                    } else if (t4 < t_max && t4 > t_min && in_circle_range(r, t4)) {
                        set_record_bottom(r, t4, rec, increasing);
                        return true;
                    }
                } else {
                    if (t2 < t4) {
                        // t1 < t3 < t2 < t4
                        if (t1 < t_max && t1 > t_min && in_z_range(r, t1)) {
                            set_record_side(r, t1, rec);
                            return true;
                        } else if (t3 < t_max && t3 > t_min && in_circle_range(r, t3)) {
                            set_record_bottom(r, t3, rec, !increasing);
                            return true;
                        } else if (t2 < t_max && t2 > t_min && in_z_range(r, t2)) {
                            set_record_side(r, t2, rec);
                            return true;
                        } else if (t4 < t_max && t4 > t_min && in_circle_range(r, t4)) {
                            set_record_bottom(r, t4, rec, increasing);
                            return true;
                        }
                    } else {
                        // t1 < t3 < t4 < t2
                        if (t1 < t_max && t1 > t_min && in_z_range(r, t1)) {
                            set_record_side(r, t1, rec);
                            return true;
                        } else if (t3 < t_max && t3 > t_min && in_circle_range(r, t3)) {
                            set_record_bottom(r, t3, rec, !increasing);
                            return true;
                        } else if (t4 < t_max && t4 > t_min && in_circle_range(r, t4)) {
                            set_record_bottom(r, t4, rec, increasing);
                            return true;
                        } else if (t2 < t_max && t2 > t_min && in_z_range(r, t2)) {
                            set_record_side(r, t2, rec);
                            return true;
                        }
                    }
                }
            } else {
                if (t1 < t4) {
                    if (t2 < t4) {
                        // t3 < t1 < t2 < t4
                        if (t3 < t_max && t3 > t_min && in_circle_range(r, t3)) {
                            set_record_bottom(r, t3, rec, !increasing);
                            return true;
                        } else if (t1 < t_max && t1 > t_min && in_z_range(r, t1)) {
                            set_record_side(r, t1, rec);
                            return true;
                        } else if (t2 < t_max && t2 > t_min && in_z_range(r, t2)) {
                            set_record_side(r, t2, rec);
                            return true;
                        } else if (t4 < t_max && t4 > t_min && in_circle_range(r, t4)) {
                            set_record_bottom(r, t4, rec, increasing);
                            return true;
                        }
                    } else {
                        // t3 < t1 < t4 < t2
                        if (t3 < t_max && t3 > t_min && in_circle_range(r, t3)) {
                            set_record_bottom(r, t3, rec, !increasing);
                            return true;
                        } else if (t1 < t_max && t1 > t_min && in_z_range(r, t1)) {
                            set_record_side(r, t1, rec);
                            return true;
                        } else if (t4 < t_max && t4 > t_min && in_circle_range(r, t4)) {
                            set_record_bottom(r, t4, rec, increasing);
                            return true;
                        } else if (t2 < t_max && t2 > t_min && in_z_range(r, t2)) {
                            set_record_side(r, t2, rec);
                            return true;
                        }
                    }
                } else {
                    // t3 < t4 < t1 < t2
                    if (t3 < t_max && t3 > t_min && in_circle_range(r, t3)) {
                        set_record_bottom(r, t3, rec, !increasing);
                        return true;
                    } else if (t4 < t_max && t4 > t_min && in_circle_range(r, t4)) {
                        set_record_bottom(r, t4, rec, increasing);
                        return true;
                    } else if (t1 < t_max && t1 > t_min && in_z_range(r, t1)) {
                        set_record_side(r, t1, rec);
                        return true;
                    } else if (t2 < t_max && t2 > t_min && in_z_range(r, t2)) {
                        set_record_side(r, t2, rec);
                        return true;
                    }
                }
            }
        }
        return false;
    }
    void set_record_side(const ray &r, float t, hit_record &rec) const {
        rec.t = t;
        rec.p = r.point_at_parameter(rec.t);
        rec.normal = vec3(rec.p.x() - x, rec.p.y() - y, 0);
        rec.material_ptr = material_ptr;
    }
    void set_record_bottom(const ray &r, float t, hit_record &rec, bool pos) const {
        rec.t = t;
        rec.p = r.point_at_parameter(rec.t);
        rec.normal = pos ? vec3(0, 0, 1) : vec3(0, 0, -1);
        rec.material_ptr = material_ptr;
    }
    bool in_circle_range(const ray &r, float t) const {
        float rx = r.point_at_parameter(t).x(), ry = r.point_at_parameter(t).y();
        return pow(rx - x, 2) + pow(ry - y, 2) <= pow(radius, 2);
    }
    bool in_z_range(const ray &r, float t) const {
        return r.point_at_parameter(t).z() <= z_max && r.point_at_parameter(t).z() >= z_min;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = AxisAlignedBoundingBox(vec3(x, y, z_min) - vec3(radius, radius, 0), vec3(x, y, z_max) + vec3(radius, radius, 0));
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

#endif //RAYTRACE_CYLINDER_H
