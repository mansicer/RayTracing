//
// Created by sicer on 2021/4/18.
//

#ifndef RAYTRACE_PLAYGROUND_H
#define RAYTRACE_PLAYGROUND_H

#include "demo.h"

class PlaygroundDemo: public Demo {
    void create_demo(int width, int height) override {
        std::list<Hitable *> list;
        for (int i = -3; i <= 13; i++) {
            for (int j = -3; j <= 13; j++) {
                float radius = 0.2 * drand48() + 0.1;
                float x = i + 0.9 * drand48();
                float y = j + 0.9 * drand48();
                vec3 center(x + 0.9 * drand48(), y, radius);

                if (abs(center.x() - center.y()) > 0.8) {
                    float choose_mat = drand48();
                    Material *mat;

                    if (choose_mat < 0.8) {
                        mat = new Lambertian(new ConstantTexture(drand48() * drand48(), drand48() * drand48(), drand48() * drand48()));
                    } else if (choose_mat < 0.95) {
                        mat = new Metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48());
                    } else {
                        mat = new Dielectric(1.2 + 0.6 * drand48());
                    }

                    float choose_shape = drand48();
                    if (choose_shape < 0.8) {
                        list.push_back(new Sphere(center, radius, mat));
                    } else {
                        list.push_back(new Cylinder(center.x(), center.y(), 0, 0.2 + 0.4 * drand48(), radius, mat));
                    }
                }
            }
        }
        list.push_back(new Sphere(vec3(2, 2, 1.3), 1.3, new Dielectric(1.5)));
        list.push_back(new Sphere(vec3(5, 5, 1.3), 1.3, new Lambertian(new ConstantTexture(0.4, 0.2, 0.1))));
        list.push_back(new Sphere(vec3(8, 8, 1.3), 1.3, new Metal(vec3(0.7, 0.6, 0.5), 0.1)));

//        list.push_back(new Sphere(vec3(1, 1, 5), 0.2, new DiffuseLight(new ConstantTexture(4, 4, 4))));

        vec3 lookfrom(18, 12, 1.5);
        vec3 lookat(0, 3, 0);

        float dist_to_focus = 1 * (lookfrom - lookat).length();
        float aperture = 0;
        camera = new Camera(lookfrom, lookat, vec3(0, 0, 1), 30, float(width)/float(height), aperture, dist_to_focus, 0, 1);

        world = new World(list, camera->start_time(), camera->end_time());
        ((World *)world)->add_unbounding_object(new Plane(0, 0, 1, 0, new Lambertian(new CheckerTexture(new ConstantTexture(0.2, 0.3, 0.1), new ConstantTexture(0.9, 0.9, 0.9)))));
        ((World *)world)->build_world();
    }
};

#endif //RAYTRACE_PLAYGROUND_H
