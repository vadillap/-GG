#include <fstream>
#include "Pnm.h"


Pnm::Pnm(const std::string &path) {
    std::ifstream fin(path);

    if (!fin.is_open()) {
        throw "File open error";
    }

    std::string s_type, s_w, s_h, s_max_value;
    fin >> s_type >> s_w >> s_h >> s_max_value;

    if (s_type == "P6" || s_type == "P5") {
        type = s_type[1];
    } else {
        throw "File not supported";
    }
    int tmp_w, tmp_h, tmp_max_value;
    try {
        tmp_w = std::stoi(s_w);
        tmp_h = std::stoi(s_h);
        tmp_max_value = std::stoi(s_max_value);
    }
    catch (std::exception &ex) {
        throw ex.what();
    }
    if (tmp_w < 0 || tmp_h < 0 || tmp_max_value < 0 || tmp_max_value > 255) {
        throw "Header error";
    }

    w = tmp_w;
    h = tmp_h;
    max_value = tmp_max_value;

    fin.ignore(1,'\n');

    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    data = (uint8_t *) malloc(sizeof(uint8_t) * (w * h * byte_per_pixel));

    fin.read(reinterpret_cast<char *>(data), w * h * byte_per_pixel);

    for (int i = 0; i < w * h * byte_per_pixel; ++i) {
        if (data[i] > max_value) {
            throw "Pixel value overflow";
        }
    }

    if (!fin) {
        throw "Unexpected end";
    }

    fin.ignore(1);

    if (!fin.eof()) {
        throw "Unexpected data";
    }
    fin.close();
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

    if (fwrite(data, byte_per_pixel, w * h, f) != w * h) {
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
