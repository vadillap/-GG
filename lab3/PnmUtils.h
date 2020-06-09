#include "PnmFile.h"


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


void makeGradient(PnmFile &res) {
    for (int i = 0; i < res.getWidth(); ++i) {
        for (int j = 0; j < res.getHeight(); ++j) {
            Pixel p(255 * i / res.getWidth());
            res.setPixel(i, j, p, true);
        }
    }
}


#endif //CGRAPH_PNMUTILS_H
