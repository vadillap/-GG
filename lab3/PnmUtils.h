#include "PnmFile.h"
#include <set>

#ifndef CGRAPH_PNMUTILS_H
#define CGRAPH_PNMUTILS_H


PnmFile fillColor(int w, int h) {
    PnmFile res(w, h);
    for (int i = 0; i < res.getWidth(); ++i) {
        for (int j = 0; j < res.getHeight(); ++j) {
            Pixel p(255);
            res.setPixel(i, j, p);
        }
    }
    return res;
}


void countColors(PnmFile &f) {
    std::set<int> colors;
    for (int i = 0; i < f.getWidth(); ++i) {
        for (int j = 0; j < f.getHeight(); ++j) {
            colors.insert(f.getPixel(i, j, true).avg());
        }
    }
    std::cout << "Distinct colors=" << colors.size() << '\n';
}

void makeGradient(PnmFile &res) {
    for (int i = 0; i < res.getWidth(); ++i) {
        for (int j = 0; j < res.getHeight(); ++j) {
            Pixel p(255 * i / (res.getWidth() - 1));
            res.setPixel(i, j, p, false);
        }
    }
}


#endif //CGRAPH_PNMUTILS_H
