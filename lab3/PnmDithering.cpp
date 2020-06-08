#include "PnmDithering.h"
#include <vector>

int findNearestPaletteColor(int color, int bit) {
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
            double l = 0, r = 255;
            for (int k = 0; k < bit; ++k) {

                if (p.avg() > l + (rand() % int(r - l))) {
                    l = (r + l) / 2;
                } else {
                    r = (r + l) / 2;
                }
            }
            double a = (1 << bit);
            double b = (255.0 / a) * (a - 1);

            l = 255 * (double) l / b;

            pnm.setPixel(i, j, Pixel(l), true);
        }
    }
}

void OrderedDithering::dither(PnmFile &pnm, uint bit) {
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            Pixel p = pnm.getPixel(i, j, true);
            double color = findNearestPaletteColor(p.avg() + (255 / (1 << bit)) * matrix[i % 8][j % 8], bit);
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
            bias[i] = d * 5 / 16;
            next = d * 7 / 16;

            pnm.setPixel(i, j, Pixel(new_value), true);
        }
        std::swap(bias, now);
        bias.clear();
        next = 0;
    }
}

void NoDithering::dither(PnmFile &pnm, uint bit) {

}
