#include <iostream>
#include "Pnm.h"


Pnm::Pnm(std::string path) {
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) {
        throw "File open error";
    }

    if (fscanf(f, "P%c\n", &type) != 1 || (type != '5'  && type != '6')) {
        throw "File not supported";
    }

    if (fscanf(f, "%d%d%d\n", &w, &h, &max_value) != 3) {
        throw "Header error";
    }

    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    data = (uint8_t *) malloc(sizeof(uint8_t) * (w * h * byte_per_pixel));

        if (fread(data, byte_per_pixel, w * h, f) != w * h) {
            throw "Read error";
        }
    fclose(f);
}

Pnm::~Pnm() {
    free(data);
}

void Pnm::savePnm(std::string path) {
    FILE *f = fopen(path.c_str(), "wb");
    if (!f) {
        throw "File open error";
    }
    if (fprintf(f, "P%c\n%d %d\n%d\n", type, w, h, max_value) < 0) {
        fclose(f);
        throw "Header write error";
    }
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    if (fwrite(data, byte_per_pixel , w * h,  f) != w * h) {
        fclose(f);
        throw "Write error";
    }
    fclose(f);
}

void Pnm::inverse() {
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    for (int i = 0; i < w * h * byte_per_pixel; ++i) {
        data[i] = max_value - data[i];
    }
}

void Pnm::turn(int count) {
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    uint8_t *new_data;

    for (int t = 0; t < count % 4; ++t) {
        new_data = (uint8_t *) malloc(sizeof(uint8_t) * (w * h * byte_per_pixel));
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                for (int k = 0; k < byte_per_pixel; ++k) {
                    new_data[(i * h + h - 1 - j) * byte_per_pixel + k] = data[(j * w + i) * byte_per_pixel + k];
                }
            }
        }
        std::swap(w, h);
        free(data);
        data = new_data;
    }
}


void Pnm::reflect_x() {
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    for (int i = 0; i * 2 < w; ++i) {
        for (int j = 0; j < h; ++j) {
            for (int k = 0; k < byte_per_pixel; ++k) {
                std::swap(data[(j * w + i) * byte_per_pixel + k], data[(j * w + w - i - 1) * byte_per_pixel + k]);
            }
        }
    }
}

void Pnm::reflect_y() {
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j * 2 < h; ++j) {
            for (int k = 0; k < byte_per_pixel; ++k) {
                std::swap(data[(j * w + i) * byte_per_pixel + k], data[((h - j - 1) * w + i) * byte_per_pixel + k]);
            }
        }
    }
}
