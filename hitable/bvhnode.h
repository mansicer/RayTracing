//
// Created by sicer on 2021/6/17.
//

#ifndef RAYTRACING_BVHNODE_H
#define RAYTRACING_BVHNODE_H

#include "rectangle.h"
#include "material/material.h"

class BVHNode : public Hitable {
public:
    Hitable *left = nullptr;
    Hitable *right = nullptr;
    AxisAlignedBoundingBox box;
    BVHNode() = default;
    BVHNode(std::list<Hitable *> list, float t0, float t1) {
        int axis = int (3 * drand48());
        if (axis == 0) {
            list.sort([t0, t1](Hitable *a, Hitable *b) {
                AxisAlignedBoundingBox boxa, boxb;
                if (!a->bounding_box(t0, t1, boxa) || !b->bounding_box(t0, t1, boxb)) {
                    std::cerr << "No bounding box found!" << std::endl;
                    exit(-1);
                }
                return boxa.min().x() <= boxb.min().x();
            });
        } else if (axis == 1) {
            list.sort([t0, t1](Hitable *a, Hitable *b) {
                AxisAlignedBoundingBox boxa, boxb;
                if (!a->bounding_box(t0, t1, boxa) || !b->bounding_box(t0, t1, boxb)) {
                    std::cerr << "No bounding box found!" << std::endl;
                    exit(-1);
                }
                return boxa.min().y() <= boxb.min().y();
            });
        } else {
            list.sort([t0, t1](Hitable *a, Hitable *b) {
                AxisAlignedBoundingBox boxa, boxb;
                if (!a->bounding_box(t0, t1, boxa) || !b->bounding_box(t0, t1, boxb)) {
                    std::cerr << "No bounding box found!" << std::endl;
                    exit(-1);
                }
                return boxa.min().z() <= boxb.min().z();
            });
        }
        if (list.empty()) {
            std::cerr << "Try to initialize an empty BVH node!" << std::endl;
            exit(-1);
        }
        if (list.size() == 1) {
            std::cerr << "Exist BVH Node with only 1 element" << std::endl;
            exit(-1);
        }
        else if (list.size() == 2) {
            left = list.front();
            right = list.back();
        }
        else {
            auto half = std::next(list.begin(), (int)list.size()/2);
            auto left_list = std::list<Hitable *>(list.begin(), half);
            auto right_list = std::list<Hitable *>(half, list.end());
            if (left_list.size() == 1) {
                left = left_list.front();
            } else {
                left = new BVHNode(left_list, t0, t1);
            }
            right = new BVHNode(right_list, t0, t1);
        }
        AxisAlignedBoundingBox left_box, right_box;
        if (!left->bounding_box(t0, t1, left_box) || !right->bounding_box(t0, t1, right_box)) {
            std::cerr << "No bounding box found!" << std::endl;
            exit(-1);
        }
        box = surrounding_box(left_box, right_box);
    }
    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override {
        bool hit_any = false;
        if (box.hit(r, t_min, t_max)) {
            float closest = t_max;
            hit_record rec_tmp;
            if (left->hit(r, t_min, closest, rec_tmp)) {
                closest = rec_tmp.t;
                rec = rec_tmp;
                hit_any = true;
            }
            if (right->hit(r, t_min, closest, rec_tmp)) {
                closest = rec_tmp.t;
                rec = rec_tmp;
                hit_any = true;
            }
            if (hit_any) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    bool bounding_box(float t0, float t1, AxisAlignedBoundingBox &bbox) const override {
        bbox = box;
        return true;
    }
    void clear() override {
        left->clear();
        delete left;
        right ->clear();
        delete right;
        left = right = nullptr;
    }
};

#endif //RAYTRACING_BVHNODE_H
