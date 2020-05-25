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
};

class PnmFile {
private:
    char type;
    unsigned int w, h;
    uint8_t *data;
    uint8_t max_value;


public:
    PnmFile(const std::string &path);

    PnmFile(uint w, uint h);

    ~PnmFile();

    void savePnm(const std::string &path);

    uint getWidth() const;

    uint getHeight() const;

    Pixel getPixel(uint x, uint y);

    Pixel getPixel(uint x, uint y, double gamma);

    void setPixel(uint x, uint y, Pixel p);

    void setPixel(uint x, uint y, Pixel p, double gamma);
};


#endif //CGRAPH_PNMFILE_H
