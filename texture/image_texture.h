//
// Created by sicer on 2021/7/7.
//

#ifndef RAYTRACING_IMAGE_TEXTURE_H
#define RAYTRACING_IMAGE_TEXTURE_H

#include "texture.h"

class ImageTexture : public Texture {
public:
    unsigned char *data;
    int nx, ny;
    ImageTexture() {}
    ImageTexture(unsigned char *pixels, int A, int B) : data(pixels), nx(A), ny(B) {}
    vec3 value(float u, float v, const vec3 &p) const override {
        int i = (u) * nx;
        int j = (1-v) * ny - 0.001;
        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx-1) i = nx-1;
        if (j > ny-1) j = ny-1;
        float r = int(data[3*i + 3*nx*j]  ) / 255.0;
        float g = int(data[3*i + 3*nx*j+1]) / 255.0;
        float b = int(data[3*i + 3*nx*j+2]) / 255.0;
        return vec3(r, g, b);
    }
    void clear() override {

    }
};

#endif //RAYTRACING_IMAGE_TEXTURE_H
