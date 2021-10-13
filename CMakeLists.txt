cmake_minimum_required(VERSION 3.17)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(RayTrace main.cpp utils/vec3.h utils/ray.h hitable/hitable.h hitable/sphere.h hitable/hitablelist.h camera/camera.h material/material.h material/lambertian.h material/metal.h material/dielectric.h hitable/cylinder.h hitable/plane.h utils/objloader.h utils/utils.h demos/playground.h demos/stage.h utils/vec2.h utils/boundingbox.h hitable/bvhnode.h texture/texture.h texture/constant_texture.h texture/checker_texture.h demos/demo.h utils/stb_image_write.h utils/stb_image_load.h hitable/world.h material/diffuse_light.h demos/texture_demo.h utils/noise.h texture/noise_texture.h hitable/rectangle.h demos/room.h hitable/transformation.h utils/sample.h utils/pdf.h hitable/plate.h hitable/polygon.h texture/image_texture.h hitable/moving_sphere.h)
