//
// Created by sicer on 2021/4/18.
//

#ifndef RAYTRACE_STAGE_H
#define RAYTRACE_STAGE_H

#include "demo.h"

class StageDemo : public Demo {
    void create_demo(int width, int height) override {
        std::list<Hitable *> list;
        float cylinder_radius = 5;
        float cylinder_height = 0.3;
        list.push_back(new Cylinder(cylinder_radius, cylinder_radius, 0, cylinder_height, cylinder_radius, new Lambertian(new ConstantTexture(0.4, 0.2, 0.3))));

        int n_object = 10;
        float gap = 2 * cylinder_radius / n_object;
        for (int i = 0; i < n_object; i++) {
            for (int j = 0; j < n_object; j++) {
                float x = i * gap + gap * drand48();
                float y = j * gap + gap * drand48();
                float radius = 0.5 * (0.25 * gap + 0.25 * gap * drand48());
                        if (pow(x - cylinder_radius, 2) + pow(y - cylinder_radius, 2) < pow(cylinder_radius, 2) - 5) {
                            float choose_mat = drand48();
                            Material *mat;
                            if (choose_mat < 0.6) {
                                mat = new Lambertian(new ConstantTexture(drand48(), drand48(), drand48()));
                    } else if (choose_mat < 0.9) {
                        mat = new Lambertian(new CheckerTexture(new ConstantTexture(drand48() * drand48(), drand48() * drand48(), drand48() * drand48()),
                                                                new ConstantTexture(drand48(), drand48(), drand48())));
                    } else {
                        mat = new Metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())),
                                        0.3 * drand48());
                    }

                    float choose_shape = drand48();
                    Hitable *shape;
                    if (choose_shape < 0.8) {
                        shape = new Sphere(vec3(x, y, cylinder_height + radius), radius, mat);
                    } else {
                        shape = new Cylinder(x, y, cylinder_height, cylinder_height + 0.75 * gap * drand48(), radius,
                                             mat);
                    }
                    list.push_back(shape);
                }
            }
        }

        std::list<Hitable *> planes;
        planes.push_back(new Plane(1, 0, 0, 0, new Metal(vec3(0.7, 0.2, 0.8), 0.01)));
        planes.push_back(new Plane(0, 1, 0, 0, new Metal(vec3(0.1, 0.6, 0.3), 0.22)));
        planes.push_back(new Plane(0, 0, 1, 0, new Lambertian(new ConstantTexture(0.5, 0.5, 0.5))));

        vec3 lookfrom(14, 14, 4);
        vec3 lookat(3, 3, 0);

        float dist_to_focus = 1 * (lookfrom - lookat).length();
        float aperture = 0.02;
        camera = new Camera(lookfrom, lookat, vec3(0, 0, 1), 30, float(width)/float(height), aperture, dist_to_focus, 0, 1);

        world = new World(list, planes, camera->start_time(), camera->end_time());
        ((World *)(world))->build_world();
    }
};

#endif //RAYTRACE_STAGE_H
