#pragma once
#include "generalIncludes.h"

void savePPM(std::string path, int height, int width, uc* data) {
    std::ofstream ofs; 
    ofs.open("./out.ppm");
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < height * width; i++) {
        ofs << (int)data[i * 3] << " " << (int)data[i * 3 + 1] << " " << (int)data[i * 3 + 2] <<'\n';
    }
}
