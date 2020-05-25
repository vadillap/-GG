#include "PnmLine.h"

struct Vec {
private:
    double x, y;
public:
    Vec(double x, double y) : x(x), y(y) {}

    Vec operator+(Vec &o) const {
        return Vec(x + o.x, y + o.y);
    }

    Vec operator-(Vec &o) const {
        return Vec(x - o.x, y - o.y);
    }

    double operator*(Vec &o) const {
        return x * o.x + y * o.y;
    }

    double operator^(Vec &o) const {
        return y * o.x - x * o.y;
    }

    double len() const {
        return sqrt(x * x + y * y);
    }
};

struct Line {
private:
    Vec a, b;
public:
    Line(Vec a, Vec b) : a(a), b(b) {}

    bool aboveLine(Vec p) {
        Vec v1 = a - p, v2 = b - p, l = b - a;
        return ((v1 * l) > 0) ^ ((v2 * l) > 0);
    }

    double disToPoint(Vec p) {
        Vec v1 = a - p, v2 = b - p, l = b - a;
        if (aboveLine(p)) {
            return fabs(v1 ^ v2) / l.len();
        } else {
            return std::min(v1.len(), v2.len());
        }
    }


};


void drawLine(int x0, int y0, int x1, int y1, PnmFile &pnm, double thick, double gamma, unsigned int br) {
    Line line(Vec(x0, y0), Vec(x1, y1));

    br = floor(pnm.getMaxBr() * std::pow((double) br / pnm.getMaxBr(), gamma));

    const double near = sqrt(2) / 2;
    thick -= 1;
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            double distance = line.disToPoint(Vec(i, j)) - thick;
            if (distance <= near) {
                double alpha = 1 - std::max(0.0, distance) / near;
                Pixel p = pnm.getPixel(i, j, gamma).mix(Pixel(br), alpha);
                pnm.setPixel(i, j, p, gamma);
            }
        }
    }
}

