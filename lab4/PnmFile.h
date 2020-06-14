#ifndef CGRAPH_PNMFILE_H
#define CGRAPH_PNMFILE_H


#include <cstdint>
#include <string>

typedef unsigned int uint;

class Pixel {
public:
    uint8_t r, g, b;

    Pixel(uint8_t r, uint8_t g, uint8_t b);

    explicit Pixel(uint8_t br);

    Pixel mix(Pixel other, double alpha);

    uint8_t avg();
};

class Gamma {
public:
    virtual Pixel encode(Pixel p) = 0;

    virtual Pixel decode(Pixel p) = 0;

};

class srgbGamma : public Gamma {
private:
    double a1, a2;

    double f_enc(double c);

    double f_dec(double c);

public:
    srgbGamma();

    Pixel decode(Pixel p) override;

    Pixel encode(Pixel p) override;
};

class valueGamma : public Gamma {
private:
    double gamma;
public:
    valueGamma(double gamma);

    Pixel decode(Pixel p) override;

    Pixel encode(Pixel p) override;
};

class PnmFile {
private:
    char type;
    unsigned int w, h;
    uint8_t *data;
    uint8_t max_value;
    Gamma *g;

public:
    PnmFile(const std::string &path);

    PnmFile(uint w, uint h, char type = '5');

    ~PnmFile();

    void savePnm(const std::string &path);

    uint getWidth() const;

    uint getHeight() const;

    uint getMaxBr() const;

    Pixel getPixel(uint x, uint y, bool gamma = false);

    void setPixel(uint x, uint y, Pixel p, bool gamma = false);

    void setGamma(Gamma *g);

    Gamma *getGamma();
};


#endif //CGRAPH_PNMFILE_H
