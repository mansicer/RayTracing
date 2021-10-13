//
// Created by sicer on 2021/6/18.
//

#ifndef RAYTRACING_DEMO_H
#define RAYTRACING_DEMO_H

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image_load.h"

#include "hitable/hitablelist.h"
#include "hitable/sphere.h"
#include "hitable/cylinder.h"
#include "hitable/plane.h"
#include "hitable/bvhnode.h"
#include "hitable/world.h"
#include "hitable/rectangle.h"
#include "hitable/plate.h"
#include "hitable/transformation.h"
#include "hitable/polygon.h"
#include "hitable/moving_sphere.h"
#include "camera/camera.h"
#include "material/lambertian.h"
#include "material/dielectric.h"
#include "material/metal.h"
#include "material/diffuse_light.h"
#include "texture/constant_texture.h"
#include "texture/checker_texture.h"
#include "texture/noise_texture.h"
#include "texture/image_texture.h"

class Demo {
public:
    World *world;
    Camera *camera;

    Demo() = default;
    virtual ~Demo() {
        world->clear();
        delete world;
        delete camera;
    }
    virtual void create_demo(int width, int height) = 0;
    World &get_world() const {
        return *world;
    }
    Camera &get_camera() const {
        return *camera;
    }

};

#endif //RAYTRACING_DEMO_H
