#include <iostream>
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

    Vec trans() const {
        return Vec(-y, x);
    }

    Vec resize(double l) const {
        return Vec((x / len()) * l, (y / len()) * l);
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }

    double len() const {
        return sqrt(x * x + y * y);
    }
};

struct Line {
private:
    Vec a, b;
    double thick;
public:
    Line(Vec a, Vec b) : a(a), b(b), thick(0) {}

    Line(Vec a, Vec b, double thick) : a(a), b(b), thick(thick / 2) {}

    bool aboveLine(Vec p) {
        Vec v1 = a - p, v2 = b - p, l = b - a;
        return ((v1 * l) > 0) ^ ((v2 * l) > 0);
    }

    double disToPoint(Vec p) {
        Vec v1 = a - p, v2 = b - p, l = b - a;
        if (aboveLine(p)) {
            return fabs(v1 ^ v2) / l.len() - thick;
        } else {
            if (thick <= 0) return std::min(v1.len(), v2.len());
            Vec t = l.trans().resize(thick);
            Line l1(a - t, a + t);
            Line l2(b - t, b + t);
            return std::min(l1.disToPoint(p), l2.disToPoint(p));
        }
    }

    double k() {
        double dx = (a.getX() - b.getX());
        double dy = (a.getY() - b.getY());

        if (dy == 0) return 0.5;
        if (dx == 0) return 0.5;

        double d = dy / dx;
        if (d <= 1) d -= 2;
        return 0.5 * std::sqrt(1 / (d * d) + 1);
    }


};


void drawLine(double x0, double y0, double x1, double y1, PnmFile &pnm, double thick, unsigned int br) {
    Line line(Vec(x0, y0), Vec(x1, y1), thick - 1);

    Pixel linePixel = pnm.getGamma()->decode(Pixel(br));

    const double near = 2 * line.k();

    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            double distance = line.disToPoint(Vec(i, j));
            if (distance <= near) {
                double alpha = 1 - std::max(0.0, distance) / near;
                Pixel p = pnm.getPixel(i, j, true).mix(linePixel, alpha);
                pnm.setPixel(i, j, p, true);
            }
        }
    }
}

