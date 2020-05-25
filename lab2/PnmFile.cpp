#include <fstream>
#include <cmath>
#include <stdexcept>
#include "PnmFile.h"


PnmFile::PnmFile(uint w, uint h) : w(w), h(h) {
    type = '6';
    max_value = 255;
    data = (uint8_t *) malloc(sizeof(uint8_t) * w * h * 3);
}

PnmFile::PnmFile(const std::string &path) {
    std::ifstream fin(path, std::ios::binary);

    if (!fin.is_open()) {
        throw std::runtime_error("File open error");
    }

    std::string s_type, s_w, s_h, s_max_value;
    fin >> s_type >> s_w >> s_h >> s_max_value;

    if (s_type == "P6" || s_type == "P5") {
        type = s_type[1];
    } else {
        throw std::runtime_error("File not supported");
    }
    int tmp_w, tmp_h, tmp_max_value;
    try {
        tmp_w = std::stoi(s_w);
        tmp_h = std::stoi(s_h);
        tmp_max_value = std::stoi(s_max_value);
    }
    catch (std::exception &ex) {
        throw ex;
    }
    if (tmp_w < 0 || tmp_h < 0 || tmp_max_value < 0 || tmp_max_value > 255) {
        throw std::runtime_error("Header error");
    }

    w = tmp_w;
    h = tmp_h;
    max_value = tmp_max_value;

    fin.ignore(1, '\n');

    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    data = (uint8_t *) malloc(sizeof(uint8_t) * (w * h * byte_per_pixel));

    fin.read(reinterpret_cast<char *>(data), w * h * byte_per_pixel);

    for (int i = 0; i < w * h * byte_per_pixel; ++i) {
        if (data[i] > max_value) {
            throw std::overflow_error("Pixel value overflow");
        }
    }

    if (fin.eof()) {
        throw std::runtime_error("Unexpected end");
    }

    fin.ignore(1);

    if (!fin.eof()) {
        throw std::runtime_error("Unexpected data");
    }
    fin.close();
}

PnmFile::~PnmFile() {
    free(data);
}

void PnmFile::savePnm(const std::string &path) {
    FILE *f = fopen(path.c_str(), "wb");
    if (!f) {
        throw std::runtime_error("File open error");
    }
    if (fprintf(f, "P%c\n%d %d\n%d\n", type, w, h, max_value) < 0) {
        fclose(f);
        throw std::runtime_error("Header write error");
    }
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;

    if (fwrite(data, byte_per_pixel, w * h, f) != w * h) {
        fclose(f);
        throw std::runtime_error("Write error");
    }
    fclose(f);
}

Pixel PnmFile::getPixel(uint x, uint y) {
    uint pos = w * y + x;
    if (type == '6') {
        return Pixel(
                data[pos * 3],
                data[pos * 3 + 1],
                data[pos * 3 + 2]
        );
    } else {
        return Pixel(
                data[pos],
                data[pos],
                data[pos]
        );
    }
}

void PnmFile::setPixel(uint x, uint y, Pixel p) {
    assert(p.r <= max_value && p.g <= max_value && p.b <= max_value);
    assert(x < w && y < h);
    uint pos = w * y + x;
    if (type == '6') {
        data[pos * 3] = p.r;
        data[pos * 3 + 1] = p.g;
        data[pos * 3 + 2] = p.b;
    } else {
        data[pos] = (p.r + p.g + p.b) / 3;
    }
}

void PnmFile::setPixel(uint x, uint y, Pixel p, double gamma) {
    p.r = max_value * std::pow((double) p.r / max_value, 1.0 / gamma);
    p.g = max_value * std::pow((double) p.g / max_value, 1.0 / gamma);
    p.b = max_value * std::pow((double) p.b / max_value, 1.0 / gamma);
    setPixel(x, y, p);
}

uint PnmFile::getWidth() const {
    return w;
}

uint PnmFile::getHeight() const {
    return h;
}

Pixel PnmFile::getPixel(uint x, uint y, double gamma) {
    Pixel p = getPixel(x, y);

    p.r = max_value * std::pow((double) p.r / max_value, gamma);
    p.g = max_value * std::pow((double) p.g / max_value, gamma);
    p.b = max_value * std::pow((double) p.b / max_value, gamma);

    return p;
}

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

Pixel Pixel::mix(Pixel other, double alpha) {
    r = r * (1 - alpha) + other.r * alpha;
    g = g * (1 - alpha) + other.g * alpha;
    b = b * (1 - alpha) + other.b * alpha;
    return *this;
}

Pixel::Pixel(uint8_t br) : r(br), g(br), b(br) {}

