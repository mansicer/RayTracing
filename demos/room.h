//
// Created by sicer on 2021/6/21.
//

#ifndef RAYTRACING_ROOM_H
#define RAYTRACING_ROOM_H

#include "demo.h"

class RoomDemo: public Demo {
    void create_demo(int width, int height) override {
        std::list<Hitable *> list1, list2;

        auto red = new Lambertian(new ConstantTexture(0.65, 0.05, 0.05));
        auto green = new Lambertian(new ConstantTexture(0.12, 0.45, 0.15));
        auto white = new Lambertian(new ConstantTexture(0.73, 0.73, 0.73));
        auto blue = new Lambertian(new ConstantTexture(0.25, 0.15, 0.69));
        auto light = new DiffuseLight(new ConstantTexture(15, 15 , 15));
        auto aluminum = new Metal(vec3(0.8, 0.85, 0.88), 0.1);
        auto noise = new Lambertian(new NoiseTexture(0.1));

        list1.push_back(new ZRotation(new HitBox(vec3(265, 395, 0), vec3(430, 560, 165), white), -10));
//        list1.push_back(new Sphere(vec3(160, 240, 100), 100, aluminum));
        list1.push_back(new Translate(new ZRotation(new HitBox(vec3(-50, -70, 0), vec3(50, 70, 200), aluminum), 35), vec3(150, 200, 0)));

        auto light_obj = new XYPlate(vec3(278, 380, 554), 0, 75 , light);
//        auto light_obj = new XYRectangle(200, 300, 300, 460, 554, light);
        auto light_obj2 = new Polygon({vec3(200, 1, 250), vec3(250, 1, 215),
                                       vec3(300, 1, 215), vec3(350, 1, 250),
                                       vec3(300, 1, 285), vec3(250, 1, 285)}, light);
//        auto light_obj2 = new Polygon({vec3(1, 400, 250), vec3(1, 425, 275),
//                                       vec3(1, 400, 300), vec3(1, 375, 275)}, light);
        list1.push_back(light_obj);
        list1.push_back(light_obj2);

        list1.push_back(new XZRectangle(0, 555, 0, 555, 0, green));
        list1.push_back(new XZRectangle(0, 555, 0, 555, 800, red));
        list1.push_back(new YZRectangle(0, 800, 0, 555, 0, blue));
        list1.push_back(new XYRectangle(0, 555, 0, 800, 0, white));
        list1.push_back(new XYRectangle(0, 555, 0, 800, 555, white));

        vec3 lookfrom(1200, 400, 278);
        vec3 lookat(0, 400, 278);

        float dist_to_focus = 10;
        float aperture = 0;
        camera = new Camera(lookfrom, lookat, vec3(0, 0, 1), 40, float(width)/float(height), aperture, dist_to_focus, 0, 1);

        world = new World(list1, camera->start_time(), camera->end_time());
        world->add_light(light_obj);
        world->add_light(light_obj2);
        world->build_world();
    }
};

#endif //RAYTRACING_ROOM_H
