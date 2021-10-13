//
// Created by sicer on 2021/6/18.
//

#ifndef RAYTRACING_WORLD_H
#define RAYTRACING_WORLD_H

#include "bvhnode.h"

class World : public Hitable {
public:
    std::list<Hitable *> bounding_list;
    std::list<Hitable *> unbounding_list;
    HitableList *light;
    Hitable *bounding_node;
    float time0, time1;
    std::list<Hitable *> lights_list;

    World() = default;
    World(std::list<Hitable *> list, float t0, float t1) : bounding_list(list), time0(t0), time1(t1) {}
    World(std::list<Hitable *> bounding, std::list<Hitable *> unbounding, float t0, float t1) : bounding_list(bounding), unbounding_list(unbounding), time0(t0), time1(t1) {}
    void add_bounding_object(Hitable *object) {
        bounding_list.push_back(object);
    }
    void add_unbounding_object(Hitable *object) {
        unbounding_list.push_back(object);
    }
    void add_light(Hitable *object) {
        lights_list.push_back(object);
    }
    bool has_light() const {
        return !lights_list.empty();
    }
    HitableList *get_lights() const {
        return light;
    }
    float &t0() {
        return time0;
    }
    float &t1() {
        return time1;
    }
    void build_world() {
//        bounding_node = new HitableList(bounding_list);
        bounding_node = new BVHNode(bounding_list, time0, time1);
        light = new HitableList(lights_list);
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        hit_record tmp;
        bool hit_any = bounding_node->hit(r, t_min, t_max, tmp);
        if (hit_any) {
            rec = tmp;
        }
        float closest = hit_any ? tmp.t : t_max;
        for (auto item : unbounding_list) {
            if (item->hit(r, t_min, closest, tmp)) {
                hit_any = true;
                closest = tmp.t;
                rec = tmp;
            }
        }
        return hit_any;
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        if (bounding_node == nullptr) {
            std::cerr << "Call world->build_world() first!" << std::endl;
            exit(-1);
        }
        if (unbounding_list.empty()) {
            return bounding_node->bounding_box(t0, t1, bbox);
        } else {
            return false;
        }
    }
    void clear() override {
        bounding_node->clear();
        delete bounding_node;
        bounding_node = nullptr;
    }
};

#endif //RAYTRACING_WORLD_H
