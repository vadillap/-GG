#include <fstream>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include "PnmFile.h"


PnmFile::PnmFile(uint w, uint h, char type) : w(w), h(h), type(type) {
    max_value = 255;
    unsigned int byte_per_pixel = type == '6' ? 3 : 1;
    data = (uint8_t *) malloc(sizeof(uint8_t) * w * h * byte_per_pixel);
    g = new srgbGamma();
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

    g = new srgbGamma();
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

Pixel PnmFile::getPixel(uint x, uint y, bool gamma) {
    uint pos = w * y + x;
    Pixel res(0);
    if (type == '6') {
        res = Pixel(
                data[pos * 3],
                data[pos * 3 + 1],
                data[pos * 3 + 2]
        );
    } else {
        res = Pixel(
                data[pos],
                data[pos],
                data[pos]
        );
    }

    if (gamma) {
        return g->decode(res);
    }
    return res;
}

void PnmFile::setPixel(uint x, uint y, Pixel p, bool gamma) {
    assert(p.r <= max_value && p.g <= max_value && p.b <= max_value);
    assert(x < w && y < h);

    if (gamma) {
        p = g->encode(p);
    }
    uint pos = w * y + x;
    if (type == '6') {
        data[pos * 3] = p.r;
        data[pos * 3 + 1] = p.g;
        data[pos * 3 + 2] = p.b;
    } else {
        data[pos] = (p.r + p.g + p.b) / 3;
    }
}


uint PnmFile::getWidth() const {
    return w;
}

uint PnmFile::getHeight() const {
    return h;
}


uint PnmFile::getMaxBr() const {
    return max_value;
}

void PnmFile::setGamma(Gamma *g) {
    this->g = g;
}

Gamma *PnmFile::getGamma() {
    return g;
}

Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

Pixel Pixel::mix(Pixel other, double alpha) {
    r = r * (1 - alpha) + other.r * alpha;
    g = g * (1 - alpha) + other.g * alpha;
    b = b * (1 - alpha) + other.b * alpha;
    return *this;
}

Pixel::Pixel(uint8_t br) : r(br), g(br), b(br) {}

uint8_t Pixel::avg() {
    return ((int) r + g + b) / 3;
}

Pixel valueGamma::decode(Pixel p) {
    return Pixel(
            255 * std::pow((double) p.r / 255, gamma),
            255 * std::pow((double) p.g / 255, gamma),
            255 * std::pow((double) p.b / 255, gamma)
    );

}

Pixel valueGamma::encode(Pixel p) {
    return Pixel(
            255 * std::pow((double) p.r / 255, 1.0 / gamma),
            255 * std::pow((double) p.g / 255, 1.0 / gamma),
            255 * std::pow((double) p.b / 255, 1.0 / gamma)
    );
}

valueGamma::valueGamma(double gamma) : gamma(gamma) {}

srgbGamma::srgbGamma() {
    a1 = 0.0031308;
    a2 = 0.04045;
}

double srgbGamma::f_enc(double c) {
    if (c <= a1) return 12.92 * c;
    return (211 * std::pow(c, 5.0 / 12) - 11) / 200;
}

Pixel srgbGamma::encode(Pixel p) {
    return Pixel(
            255 * f_enc(p.r / 255.0),
            255 * f_enc(p.g / 255.0),
            255 * f_enc(p.b / 255.0)
    );
}

double srgbGamma::f_dec(double c) {
    if (c <= a2) return c / 12.92;
    return std::pow(double(200 * c + 11) / 211, 12.0 / 5);
}


Pixel srgbGamma::decode(Pixel p) {
    return Pixel(
            255 * f_dec(p.r / 255.0),
            255 * f_dec(p.g / 255.0),
            255 * f_dec(p.b / 255.0)
    );
}





