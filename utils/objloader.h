//
// Created by sicer on 2021/4/18.
//

#ifndef RAYTRACE_OBJLOADER_H
#define RAYTRACE_OBJLOADER_H

#include <fstream>
#include "utils.h"

class ObjLoader {
private:
    vec3 min_point, max_point;

public:
    ObjLoader(const std::string path) {
        std::ifstream  in;
        in.open(path, std::ifstream::in);
        if (in.fail()) {
            std::cerr << "Failed to load obj file: " << path << std::endl;
            exit(-1);
        }
        std::string line;
        min_point = vec3(MAXFLOAT, MAXFLOAT, MAXFLOAT);
        max_point = vec3(-1e100, -1e100, -1e100);

        std::list<vec3> vertices;
        std::list<vec3> normals;
        std::list<vec3> textures;

        while (!in.eof()) {
            getline(in, line);
            std::string dummy;
            float x, y, z;
            if (!line.compare(0, 2, "v ")) {
                std::istringstream iss(line.c_str());
                iss >> dummy;
                iss >> x >> y >> z;
                vec3 vertex(x, y, z);
                min_point = minimum(min_point, vertex);
                max_point = maximum(max_point, vertex);
            }
            else if (!line.compare(0, 3, "vn ")) {
                std::istringstream iss(line.c_str());
                iss >> dummy;
                iss >> x >> y >> z;
                vec3 normal(x, y, z);
                normals.push_back(unit_vector(normal));
            }
            else if (!line.compare(0, 3, "vt ")) {
                std::istringstream iss(line.c_str());
                iss >> dummy;
                iss >> x >> y;
                textures.push_back(vec3(x, y, 0));
            }
            else if (!line.compare(0, 2, "f ")) {
                std::istringstream iss(line.c_str());
                iss >> dummy;
                int a, b, c;
                iss >> a >> b >> c;

            }
        }
    }

};

#endif //RAYTRACE_OBJLOADER_H
