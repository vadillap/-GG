#include <vector>
#include "PnmFile.h"

#ifndef CGRAPH_PNMDITHERING_H
#define CGRAPH_PNMDITHERING_H

int findNearestPaletteColor(int color, int bit);

class Dithering {
public:
    virtual void dither(PnmFile &pnm, uint bit) = 0;
};


class OrderedDithering : public Dithering {
private:
    virtual double getThreshold(int i, int j) = 0;

public:
    void dither(PnmFile &pnm, uint bit) override;
};

class BayerDithering : public OrderedDithering {
private:
    double matrix[8][8] =
            {
                    {0,  48, 12, 60, 3,  51, 15, 63},
                    {32, 16, 44, 28, 35, 19, 47, 31},
                    {8,  56, 4,  52, 11, 59, 7,  55},
                    {40, 24, 36, 20, 43, 27, 39, 23},
                    {2,  50, 14, 62, 1,  49, 13, 61},
                    {34, 18, 46, 30, 33, 17, 45, 29},
                    {10, 58, 6,  54, 9,  57, 5,  53},
                    {42, 26, 38, 22, 41, 25, 37, 21},
            };

    double getThreshold(int i, int j) override;

public:
    BayerDithering();
};

class HalftoneDithering : public OrderedDithering {
private:
    double matrix[4][4] = {{7,  13, 11, 4},
                           {12, 16, 14, 8},
                           {10, 15, 6,  2},
                           {5,  9,  3,  1}};

    double getThreshold(int i, int j) override;

public:
    HalftoneDithering();
};


class NoDithering : public OrderedDithering {
    double getThreshold(int i, int j) override;
};

class RandomDithering : public OrderedDithering {
    double getThreshold(int i, int j) override;
};


class ErrorDithering : public Dithering {
protected:
    int k1, k2, all;
    std::vector<int> layer1, layer2;
public:
    void dither(PnmFile &pnm, uint bit) override;
};

class FloydDithering : public ErrorDithering {
public:
    FloydDithering() {
        k1 = 7;
        k2 = 0;
        all = 16;
        layer1 = {0, 3, 5, 1, 0};
        layer2 = {0, 0, 0, 0, 0};
    }
};


class JJNDithering : public ErrorDithering {
public:
    JJNDithering() {
        k1 = 7;
        k2 = 5;
        all = 48;
        layer1 = {3, 5, 7, 5, 3};
        layer2 = {1, 3, 5, 3, 1};
    }
};


class AtkinsonDithering : public ErrorDithering {
public:
    AtkinsonDithering() {
        k1 = 1;
        k2 = 1;
        all = 8;
        layer1 = {0, 1, 1, 1, 0};
        layer2 = {0, 0, 1, 0, 0};
    }
};

class SierraDithering : public ErrorDithering {
public:
    SierraDithering() {
        k1 = 5;
        k2 = 3;
        all = 32;
        layer1 = {2, 4, 5, 4, 2};
        layer2 = {0, 2, 3, 2, 0};
    }
};


#endif //CGRAPH_PNMDITHERING_H
