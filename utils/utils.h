//
// Created by sicer on 2021/4/18.
//

#ifndef RAYTRACE_UTILS_H
#define RAYTRACE_UTILS_H

#include <vector>

#include "vec3.h"

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 sqrt(vec3 vec) {
    return vec3(sqrt(vec[0]), sqrt(vec[1]), sqrt(vec[2]));
}

inline vec3 minimum(vec3 a, vec3 b) {
    vec3 result;
    result.x() = a.x() > b.x() ? b.x() : a.x();
    result.y() = a.y() > b.y() ? b.y() : a.y();
    result.z() = a.z() > b.z() ? b.z() : a.z();
    return result;
}

inline vec3 maximum(vec3 a, vec3 b) {
    vec3 result;
    result.x() = a.x() < b.x() ? b.x() : a.x();
    result.y() = a.y() < b.y() ? b.y() : a.y();
    result.z() = a.z() < b.z() ? b.z() : a.z();
    return result;
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1);
    return p;
}

vec3 random_on_unit_sphere() {
    vec3 p;
    do {
        p = 2 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1);
    return unit_vector(p);
}

inline vec3 random_to_sphere(float radius, float distance_squared) {
    float r1 = drand48();
    float r2 = drand48();
    float z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);
    float phi = 2 * M_PI * r1;
    float x = cos(phi) * sqrt(1 - z * z);
    float y = sin(phi) * sqrt(1 - z * z);
    return vec3(x, y, z);
}

vec3 random_cosine_direction() {
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1 - r2);
    float phi = 2 * M_PI * r1;
    float x = cos(phi) * 2 * sqrt(r2);
    float y = sin(phi) * 2 * sqrt(r2);
    return vec3(x, y, z);
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

void get_sphere_uv(const vec3& p, float& u, float& v) {
    float phi = atan2(p.x(), p.y());
    float theta = asin(p.z());
    u = 1-(phi + M_PI) / (2*M_PI);
    v = (theta + M_PI/2) / M_PI;
}

std::string get_save_path() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);

    std::string filename = "RuntimeImage/Test-";
    filename += std::to_string(1900 + ltm->tm_year);
    std::string month = std::to_string(ltm->tm_mon);
    if (month.length() < 2) {
        month = "0" + month;
    }
    filename += month;
    std::string day = std::to_string(ltm->tm_mday);
    if (day.length() < 2) {
        day = "0" + day;
    }
    filename += day;
    filename += "-";
    std::string hour = std::to_string(ltm->tm_hour);
    if (hour.length() < 2) {
        hour = "0" + hour;
    }
    filename += hour;
    std::string minute = std::to_string(ltm->tm_min);
    if (minute.length() < 2) {
        minute = "0" + minute;
    }
    filename += minute;
    std::string second = std::to_string(ltm->tm_sec);
    if (second.length() < 2) {
        second = "0" + second;
    }
    filename += second;
    filename += ".jpg";
    return filename;
}

#endif //RAYTRACE_UTILS_H
