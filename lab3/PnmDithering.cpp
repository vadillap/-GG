#include "PnmDithering.h"
#include <vector>

int findNearestPaletteColor(int color, int bit) {
    color = std::max(0, color);
    color = std::min(255, color);
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        res += ((color >> (7 - (i % bit))) % 2) << (7 - i);
    }
    return res;
}

void RandomDithering::dither(PnmFile &pnm, uint bit) {
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            Pixel p = pnm.getPixel(i, j, true);
            double color = findNearestPaletteColor(
                    p.avg() + (255 / (1 << (bit - 1))) * ((rand() % (1000000000)) / 1000000000.0 - 0.5),
                    bit
            );

            pnm.setPixel(i, j, Pixel(color), true);
        }
    }
}

void OrderedDithering::dither(PnmFile &pnm, uint bit) {
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            Pixel p = pnm.getPixel(i, j, true);

            double color = findNearestPaletteColor(p.avg() + (255 / (1 << (bit - 1))) * matrix[j % 8][i % 8], bit);
            pnm.setPixel(i, j, Pixel(color), true);
        }
    }
}

OrderedDithering::OrderedDithering() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            matrix[i][j] /= 64;
            matrix[i][j] -= 0.5;
        }
    }
}

void FloydDithering::dither(PnmFile &pnm, uint bit) {
    std::vector<double> bias(pnm.getWidth()), now(pnm.getWidth());
    double next = 0;
    for (int j = 0; j < pnm.getHeight(); ++j) {
        for (int i = 0; i < pnm.getWidth(); ++i) {
            Pixel p = pnm.getPixel(i, j, true);
            double value = p.avg() + next + now[i];
            double new_value = findNearestPaletteColor(value, bit);
            double d = -new_value + value;
            if (i > 0) {
                bias[i - 1] += d * 3 / 16;
            }
            if (i + 1 < pnm.getWidth()) {
                bias[i + 1] += d / 16;
            }
            bias[i] += d * 5 / 16;
            next = d * 7 / 16;

            pnm.setPixel(i, j, Pixel(new_value), true);
        }
        swap(bias, now);
        bias = std::vector<double>(pnm.getWidth());
    }
}

void NoDithering::dither(PnmFile &pnm, uint bit) {
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            Pixel p = pnm.getPixel(i, j, true);
            double color = findNearestPaletteColor(p.avg(), bit);
            pnm.setPixel(i, j, Pixel(color), true);
        }
    }
}
