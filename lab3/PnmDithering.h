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
public:
    OrderedDithering();
    void dither(PnmFile &pnm, uint bit) override;
};

class FloydDithering : public Dithering {
public:
    void dither(PnmFile &pnm, uint bit) override;
};

class NoDithering : public Dithering {
public:
    void dither(PnmFile &pnm, uint bit) override;
};

class RandomDithering : public Dithering {
public:
    void dither(PnmFile &pnm, uint bit) override;
};


#endif //CGRAPH_PNMDITHERING_H
