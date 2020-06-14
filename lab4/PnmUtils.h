#include "PnmFile.h"
#include <set>
#include <cassert>

#ifndef CGRAPH_PNMUTILS_H
#define CGRAPH_PNMUTILS_H


PnmFile *merge_pnm(PnmFile &r, PnmFile &g, PnmFile &b) {
    assert(r.getWidth() == g.getWidth() && g.getWidth() == b.getWidth());
    assert(r.getHeight() == g.getHeight() && g.getHeight() == b.getHeight());

    auto *merged = new PnmFile(r.getWidth(), r.getHeight(), '6');

    for (int i = 0; i < r.getWidth(); ++i) {
        for (int j = 0; j < r.getHeight(); ++j) {
            Pixel p(
                    r.getPixel(i, j).r,
                    g.getPixel(i, j).g,
                    b.getPixel(i, j).b
            );
            merged->setPixel(i, j, p);
        }
    }
    return merged;
}

void split_pnm(PnmFile &source, PnmFile &r, PnmFile &g, PnmFile &b) {
    assert(source.getWidth() == r.getWidth() && source.getHeight() == r.getHeight());
    assert(r.getWidth() == g.getWidth() && g.getWidth() == b.getWidth());
    assert(r.getHeight() == g.getHeight() && g.getHeight() == b.getHeight());

    for (int i = 0; i < source.getWidth(); ++i) {
        for (int j = 0; j < source.getHeight(); ++j) {
            Pixel p = source.getPixel(i, j);
            r.setPixel(i, j, Pixel(p.r));
            g.setPixel(i, j, Pixel(p.g));
            b.setPixel(i, j, Pixel(p.b));
        }
    }
}

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
