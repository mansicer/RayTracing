#include <fstream>
#include <thread>
#include <vector>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utils/stb_image_write.h"

#include "demos/playground.h"
#include "demos/stage.h"
#include "demos/texture_demo.h"
#include "demos/room.h"

#include "utils/pdf.h"


enum {
    QUALITY_VERY_LOW, QUALITY_LOW, QUALITY_HIGH, QUALITY_4K
};

enum {
    DEMO_STAGE, DEMO_PLAYGROUND, DEMO_TEXTURE, DEMO_ROOM
};

#define RESOLUTION_QUALITY QUALITY_LOW
#define DEMO_NAME DEMO_ROOM
#define RANDOM_SEED

static int width = 1920;
static int height = 1080;
static int num_samples = 100;
static int num_threads = 100;

static Demo *initialize_demo() {
#ifdef RANDOM_SEED
    srand48(time(nullptr));
#endif
    if (RESOLUTION_QUALITY == QUALITY_VERY_LOW) {
        width = 960;
        height = 540;
    }
    else if (RESOLUTION_QUALITY == QUALITY_LOW) {
        width = 1280;
        height = 720;
    } else if (RESOLUTION_QUALITY == QUALITY_HIGH) {
        width = 1920;
        height = 1080;
    } else if (RESOLUTION_QUALITY == QUALITY_4K) {
        width = 3840;
        height = 2160;
    }
    Demo * demo;
    if (DEMO_NAME == DEMO_STAGE) {
        demo = new StageDemo();
    } else if (DEMO_NAME == DEMO_PLAYGROUND) {
        demo = new PlaygroundDemo();
    } else if (DEMO_NAME == DEMO_TEXTURE) {
        demo = new TextureDemo();
    } else if (DEMO_NAME == DEMO_ROOM) {
        demo = new RoomDemo();
    }
    demo->create_demo(width, height);
    return demo;
}

inline vec3 de_nan(const vec3 &v) {
    vec3 ret = v;
    if (ret[0] != ret[0]) ret[0] = 0;
    if (ret[1] != ret[1]) ret[1] = 0;
    if (ret[2] != ret[2]) ret[2] = 0;
    return ret;
}

vec3 color(const ray& r, World &world, int depth) {
    hit_record hrec;
    auto col = vec3(0, 0, 0);
    if (world.hit(r, 1e-5, MAXFLOAT, hrec)) {
        hrec.world = &world;
        scatter_record srec;
        col += hrec.material_ptr->emitted(r, hrec);
        if (depth < 50 && hrec.material_ptr->scatter(r, hrec, srec)) {
            if (srec.is_specular) {
                return col + srec.attenuation * color(srec.specular_ray, world, depth + 1);
            } else {
                if (world.has_light()) {
                    HitablePDF plight(world.get_lights(), hrec.p);
                    MixedPDF p(plight, *srec.pdf_ptr);
                    ray scattered = ray(hrec.p, p.sample(), r.time());
                    float pdf_val = p.value(scattered.direction());
                    delete srec.pdf_ptr;
                    return col + srec.attenuation * hrec.material_ptr->scattering_pdf(r, hrec, scattered) *
                                 color(scattered, world, depth + 1) / pdf_val;
                } else {
                    PDF &p = *srec.pdf_ptr;
                    ray scattered = ray(hrec.p, p.sample(), r.time());
                    float pdf_val = p.value(scattered.direction());
                    delete srec.pdf_ptr;
                    return col + srec.attenuation * hrec.material_ptr->scattering_pdf(r, hrec, scattered) *
                                 color(scattered, world, depth + 1) / pdf_val;
                }
            }
        } else {
            return col;
        }
    } else {
        // environment
        return col + vec3(1, 1, 1);
    }
}

void coloring_thread(int p_start, int p_end, Camera &camera, World &world, unsigned char *image) {
    for (int p = p_start; p < p_end; p++) {
        int j = p / width;
        int i = p % width;

        vec3 col(0, 0, 0);
        for (int s = 0; s < num_samples; s++) {
            float u = float(i + drand48()) / float(width);
            float v = float(j + drand48()) / float(height);
            ray r = camera.get_ray(u, v);
            col += de_nan(color(r, world, 0));
        }
        col = sqrt(col / float(num_samples));

        image[(height - 1 - j) * width * 3 + i * 3 + 0] = int(255.99 * col.r());
        image[(height - 1 - j) * width * 3 + i * 3 + 1] = int(255.99 * col.g());
        image[(height - 1 - j) * width * 3 + i * 3 + 2] = int(255.99 * col.b());
    }
}

int main() {
    auto *image = new unsigned char [width * height * 3];

    Demo * demo = initialize_demo();
    World &world = demo->get_world();
    Camera &camera = demo->get_camera();

    std::vector<std::thread> thread_list;
    thread_list.reserve(num_threads);

    for (int j = 0; j < num_threads; j++) {
        int pixels_per_thread = width * height / num_threads;
        int start = j * pixels_per_thread;
        int end = j != num_threads - 1 ? (j + 1) * pixels_per_thread : width * height;
        thread_list.emplace_back(coloring_thread, start, end, std::ref(camera), std::ref(world), std::ref(image));
    }
    for (int j = 0; j < num_threads; j++) {
        thread_list[j].join();
    }

    stbi_write_png(get_save_path().data(), width, height, 3, image, width * 3);
    delete[] image;
}
