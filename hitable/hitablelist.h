//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_HITABLELIST_H
#define RAYTRACE_HITABLELIST_H

#include "hitable.h"

class HitableList: public Hitable {
public:
    std::list<Hitable *> list;
    HitableList() = default;
    HitableList(std::list<Hitable *> list): list(list) {}
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        hit_record tmp;
        bool hit_any = false;
        double closest = t_max;
        for (auto item : list) {
            if (item->hit(r, t_min, closest, tmp)) {
                hit_any = true;
                closest = tmp.t;
                rec = tmp;
            }
        }
        return hit_any;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        if (list.empty()) {
            return false;
        }
        AxisAlignedBoundingBox tmp_box;
        bool first_true = list.front()->bounding_box(t0, t1, tmp_box);
        if (!first_true) {
            return false;
        } else {
            bbox = tmp_box;
        }
        for (auto ptr = ++list.begin(); ptr != list.end(); ptr++) {
            if ((*ptr)->bounding_box(t0, t1, tmp_box)) {
                bbox = surrounding_box(bbox, tmp_box);
            } else {
                return false;
            }
        }
        return true;
    }
    float pdf_value(const vec3 &origin, const vec3 &direction) const override {
        float weight = 1.0 / list.size();
        float sum = 0;
        for (auto ptr : list) {
            sum += weight * ptr->pdf_value(origin, direction);
        }
        return sum;
    }
    vec3 random_direction(const vec3 &origin) const override {
        int index = int(drand48() * list.size());
        return (*std::next(list.begin(), index))->random_direction(origin);
    }
    void clear() override {
        for (auto p : list) {
            p->clear();
            delete p;
        }
    }
};

#endif //RAYTRACE_HITABLELIST_H
