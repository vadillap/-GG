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


PnmFile makeGradient(int w, int h) {
    PnmFile res(w, h);
    for (int i = 0; i < res.getWidth(); ++i) {
        for (int j = 0; j < res.getHeight(); ++j) {
            Pixel p(255 * i / res.getWidth());
            res.setPixel(i, j, p);
        }
    }
    return res;
}


#endif //CGRAPH_PNMUTILS_H
