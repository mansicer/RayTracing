//
// Created by sicer on 2021/6/21.
//

#ifndef RAYTRACING_NOISE_H
#define RAYTRACING_NOISE_H

#include "utils.h"

inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w) {
    float uu = u*u*(3-2*u);
    float vv = v*v*(3-2*v);
    float ww = w*w*(3-2*w);
    float accum = 0;
    for (int i=0; i < 2; i++)
        for (int j=0; j < 2; j++)
            for (int k=0; k < 2; k++) {
                vec3 weight_v(u-i, v-j, w-k);
                accum += (i*uu + (1-i)*(1-uu))*
                         (j*vv + (1-j)*(1-vv))*
                         (k*ww + (1-k)*(1-ww))*dot(c[i][j][k], weight_v);
            }
    return accum;
}

std::vector<vec3> perlin_generate() {
    std::vector<vec3> p = std::vector<vec3>(256);
    for (int i = 0; i < p.size(); i++) {
        p[i] = unit_vector(vec3(-1 + 2 * drand48(), -1 + 2 * drand48(), -1 + 2 * drand48()));
    }
    return p;
}

void permute(std::vector<int> &p) {
    int n = p.size();
    for (int i = n - 1; i > 0; i--) {
        int target = int(drand48() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static std::vector<int> perlin_generate_permutation() {
    std::vector<int> p = std::vector<int>(256, 0);
    for (int i = 0; i < p.size(); i++) {
        p[i] = i;
    }
    permute(p);
    return p;
}

class Perlin {
public:
    static std::vector<vec3> random_vec;
    static std::vector<int> perm_x;
    static std::vector<int> perm_y;
    static std::vector<int> perm_z;
    Perlin() = default;
    float noise(const vec3 &p) const {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        vec3 c[2][2][2];
        for (int di=0; di < 2; di++)
            for (int dj=0; dj < 2; dj++)
                for (int dk=0; dk < 2; dk++)
                    c[di][dj][dk] = random_vec[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];
        return perlin_interp(c, u, v, w);
    }
    float turb(const vec3& p, int depth=7) const {
        float accum = 0;
        vec3 temp_p = p;
        float weight = 1.0;
        for (int i = 0; i < depth; i++) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }
        return fabs(accum);
    }
};

std::vector<vec3> Perlin::random_vec = perlin_generate();
std::vector<int> Perlin::perm_x = perlin_generate_permutation();
std::vector<int> Perlin::perm_y = perlin_generate_permutation();
std::vector<int> Perlin::perm_z = perlin_generate_permutation();

#endif //RAYTRACING_NOISE_H
