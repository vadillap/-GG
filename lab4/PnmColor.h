#include "PnmFile.h"

#ifndef CGRAPH_PNMCOLOR_H
#define CGRAPH_PNMCOLOR_H


class Color {
public:
    virtual Pixel toRGB(Pixel p) = 0;

    virtual Pixel fromRGB(Pixel p) = 0;
};

void transformCS(PnmFile &pnm, Color &from, Color &to);


class RGBColor : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

class HSLColor : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

class HSVColor : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

class YBR601Color : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

class YBR709Color : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

class YOGColor : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

class CMYColor : public Color {
public:

    Pixel toRGB(Pixel p) override;

    Pixel fromRGB(Pixel p) override;
};

#endif //CGRAPH_PNMCOLOR_H
