//
// Created by sicer on 2021/6/21.
//

#ifndef RAYTRACING_TEXTURE_DEMO_H
#define RAYTRACING_TEXTURE_DEMO_H

#include "demo.h"

class TextureDemo : public Demo {
    void create_demo(int width, int height) override {
        float time0 = 0, time1 = 1;
        std::list<Hitable *> list1, list2;
        list1.push_back(new Sphere(vec3(0, 0, 1), 1, new Lambertian(new NoiseTexture(2))));
        int nx, ny, nn;
        unsigned char *image1 = stbi_load("place.jpg", &nx, &ny, &nn, 0);
//        list1.push_back(new Sphere(vec3(3, 3, 1), 1, new Lambertian(new ImageTexture(image1, nx, ny))));
        list1.push_back(new MovingSphere(vec3(3, 3, 1), vec3(3, 3, 1) + unit_vector(random_in_unit_disk()) ,
                                         time0, time1, 1, new Lambertian(new ImageTexture(image1, nx, ny))));
//        unsigned char *image2 = stbi_load("IMG_0534.JPG", &nx, &ny, &nn, 0);
//        list1.push_back(new Sphere(vec3(3, -3, 1), 1, new Lambertian(new ImageTexture(image1, nx, ny))));
        list1.push_back(new MovingSphere(vec3(3, -3, 1), vec3(3, -3, 1) + unit_vector(random_in_unit_disk()),
                                         time0, time1, 1, new Lambertian(new ImageTexture(image1, nx, ny))));

        list2.push_back(new Plane(0, 0, 1, 0, new Lambertian(new CheckerTexture(new ConstantTexture(0, 0, 0), new ConstantTexture(1, 1, 1)))));

        vec3 lookfrom(13, 3, 3);
        vec3 lookat(0, 0, 0);

        float dist_to_focus = 1 * (lookfrom - lookat).length();
        float aperture = 0;
        camera = new Camera(lookfrom, lookat, vec3(0, 0, 1), 30, float(width)/float(height), aperture, dist_to_focus, time0, time1);

        world = new World(list1, list2, time0, time1);

        ((World *) world)->build_world();
    }
};

#endif //RAYTRACING_TEXTURE_DEMO_H
