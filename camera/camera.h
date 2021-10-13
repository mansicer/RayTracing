//
// Created by sicer on 2021/4/8.
//

#ifndef RAYTRACE_CAMERA_H
#define RAYTRACE_CAMERA_H

#include "../utils/ray.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class Camera {
public:
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
    vec3 u, v, w;
    float lens_radius;
    float t0, t1;

    Camera() {}
    Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) : t0(t0), t1(t1) {
        lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = unit_vector(cross(w, u));
        lower_left_corner = origin - focus_dist * half_width * u - focus_dist * half_height * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }
    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        float time = t0 + drand48() * (t1 - t0);
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
    }
    float start_time() const {
        return t0;
    }
    float end_time() const {
        return t1;
    }
};

#endif //RAYTRACE_CAMERA_H
