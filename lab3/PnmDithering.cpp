#include "PnmDithering.h"
#include <vector>
#include <algorithm>


int findNearestPaletteColor(int color, int bit) {
    color = std::max(0, color);
    color = std::min(255, color);
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        res += ((color >> (7 - (i % bit))) % 2) << (7 - i);
    }
    return res;
}

void OrderedDithering::dither(PnmFile &pnm, uint bit) {
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            Pixel p = pnm.getPixel(i, j, true);
            double color = findNearestPaletteColor(p.avg() + (255 / bit) * getThreshold(j, i), bit);
            pnm.setPixel(i, j, Pixel(color), true);
        }
    }
}


void ErrorDithering::dither(PnmFile &pnm, uint bit) {
    std::vector<double> bias1(pnm.getWidth()), bias2(pnm.getWidth()), now(pnm.getWidth());
    double next1 = 0, next2 = 0;
    for (int j = 0; j < pnm.getHeight(); ++j) {
        for (int i = 0; i < pnm.getWidth(); ++i) {
            Pixel p = pnm.getPixel(i, j, true);
            double value = p.avg() + next1 + now[i];
            double new_value = findNearestPaletteColor(value, bit);
            double d = -new_value + value;

            for (int k = 0; k < 5; ++k) {
                int pos = i + k - 2;
                if (pos >= 0 && pos < pnm.getWidth()) {
                    bias1[pos] += d * layer1[k] / all;
                    bias2[pos] += d * layer2[k] / all;
                }
            }
            next1 = next2 + d * k1 / all;
            next2 = d * k2 / all;

            pnm.setPixel(i, j, Pixel(new_value), true);
        }
        next2 = 0;
        std::swap(now, bias1);
        std::swap(bias1, bias2);
        bias2 = std::vector<double>(pnm.getWidth());
    }
}

HalftoneDithering::HalftoneDithering() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            matrix[i][j] /= 17;
            matrix[i][j] -= 0.5;
        }
    }
}

double HalftoneDithering::getThreshold(int i, int j) {
    return matrix[i % 4][j % 4];
}

BayerDithering::BayerDithering() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            matrix[i][j] /= 64;
            matrix[i][j] -= 0.5;
        }
    }
}

double BayerDithering::getThreshold(int i, int j) {
    return matrix[i % 8][j % 8];
}


double NoDithering::getThreshold(int i, int j) {
    return 0;
}

double RandomDithering::getThreshold(int i, int j) {
    return rand() / (RAND_MAX + 1.) - .5;
}
