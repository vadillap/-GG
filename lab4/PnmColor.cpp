//
// Created by Вадим Гаврилов on 13.06.2020.
//

#include <cmath>
#include <algorithm>
#include "PnmColor.h"

int strip_value(double v) {
    if (v > 255.) return 255;
    if (v < 0.) return 0;
    return floor(v);
}

void transformCS(PnmFile &pnm, Color &from, Color &to) {
    for (int i = 0; i < pnm.getWidth(); ++i) {
        for (int j = 0; j < pnm.getHeight(); ++j) {
            Pixel p = pnm.getPixel(i, j, false);
            p = from.toRGB(p);
            p = to.fromRGB(p);
            pnm.setPixel(i, j, p, false);
        }
    }
}


Pixel RGBColor::toRGB(Pixel p) {
    return p;
}

Pixel RGBColor::fromRGB(Pixel p) {
    return p;
}

Pixel HSLColor::toRGB(Pixel p) {
    double h = p.r / 255.0, s = p.g / 255.0, l = p.b / 255.0;

    double q;
    if (l < 0.5) {
        q = l * (1 + s);
    } else {
        q = l + s - l * s;
    }

    double P = 2 * l - q;
    double ans[3] = {h + 1.0 / 3, h, h - 1.0 / 3};

    for (int i = 0; i < 3; ++i) {
        double t = ans[i];
        if (t < 0) {
            t += 1;
        }
        if (t > 1) {
            t -= 1;
        }

        if (t < 1.0 / 6) {
            t = P + ((q - P) * 6 * t);
        } else if (1.0 / 6 <= t && t < 1.0 / 2) {
            t = q;
        } else if (1.0 / 2 <= t && t < 2.0 / 3) {
            t = P + (q - P) * (2.0 / 3 - t) * 6;
        } else {
            t = P;
        }
        ans[i] = t * 255;
    }

    return Pixel(ans[0], ans[1], ans[2]);
}

Pixel HSLColor::fromRGB(Pixel p) {
    double r = p.r / 255.0, g = p.g / 255.0, b = p.b / 255.0;
    double max = std::max(r, std::max(g, b));
    double min = std::min(r, std::min(g, b));

    double h, s, l;

    if (r == max) {
        if (g >= b) {
            h = 60 * (g - b) / (max - min);
        } else {
            h = 60 * (g - b) / (max - min) + 360;
        }
    } else if (g == max) {
        h = 60 * (b - r) / (max - min) + 120;
    } else {
        h = 60 * (r - g) / (max - min) + 240;
    }

    s = (max - min) / (1 - fabs(1 - (max + min)));
    l = (max + min) / 2;

    return Pixel(h * 255 / 360, s * 255, l * 255);

}

Pixel CMYColor::toRGB(Pixel p) {
    return Pixel(255 - p.r, 255 - p.g, 255 - p.b);
}

Pixel CMYColor::fromRGB(Pixel p) {
    return Pixel(255 - p.r, 255 - p.g, 255 - p.b);
}

Pixel YOGColor::toRGB(Pixel p) {
    double y = p.r / 255.0, co = p.g / 255.0 - 0.5, cg = p.b / 255.0 - 0.5;

    double r = y + co - cg;
    double g = y + cg;
    double b = y - co - cg;

    return Pixel(strip_value(255 * r), strip_value(255 * g), strip_value(255 * b));
}

Pixel YOGColor::fromRGB(Pixel p) {
    double r = p.r / 255.0, g = p.g / 255.0, b = p.b / 255.0;

    double y = r / 4 + g / 2 + b / 4;
    double co = r / 2 - b / 2;
    double cg = -r / 4 + g / 2 - b / 4;

    return Pixel(strip_value(255 * y), strip_value(255 * (co + 0.5)), strip_value(255 * (cg + 0.5)));
}

Pixel YBR709Color::toRGB(Pixel p) {
    double y = p.r / 255.0, cb = p.g / 255.0 - 0.5, cr = p.b / 255.0 - 0.5;

    double kb = 0.0722, kr = 0.2126, kg = 0.7152;

    double r = y + cr * (2 - 2 * kr);
    double g = y - cb * (kb / kg) * (2 - 2 * kb) - cr * (kr / kg) * (2 - 2 * kr);
    double b = y + cb * (2 - 2 * kb);

    return Pixel(strip_value(255 * r), strip_value(255 * g), strip_value(255 * b));
}

Pixel YBR709Color::fromRGB(Pixel p) {
    double r = p.r / 255.0, g = p.g / 255.0, b = p.b / 255.0;

    double kb = 0.0722, kr = 0.2126, kg = 0.7152;

    double y = r * kr + g * kg + b * kb;
    double cb = (b - y) / (2.0 * (1.0 - kb));
    double cr = (r - y) / (2.0 * (1.0 - kr));

    return Pixel(strip_value(255 * y), strip_value(255 * (cb + 0.5)), strip_value(255 * (cr + 0.5)));
}

Pixel YBR601Color::toRGB(Pixel p) {
    double y = p.r / 255.0, cb = p.g / 255.0 - 0.5, cr = p.b / 255.0 - 0.5;

    double kr = 0.299, kg = 0.587, kb = 0.114;

    double r = y + cr * (2 - 2 * kr);
    double g = y - cb * (kb / kg) * (2 - 2 * kb) - cr * (kr / kg) * (2 - 2 * kr);
    double b = y + cb * (2 - 2 * kb);

    return Pixel(strip_value(255 * r), strip_value(255 * g), strip_value(255 * b));
}

Pixel YBR601Color::fromRGB(Pixel p) {
    double r = p.r / 255.0, g = p.g / 255.0, b = p.b / 255.0;

    double kr = 0.299, kg = 0.587, kb = 0.114;

    double y = r * kr + g * kg + b * kb;
    double cb = (b - y) / (2.0 * (1.0 - kb));
    double cr = (r - y) / (2.0 * (1.0 - kr));

    return Pixel(strip_value(255 * y), strip_value(255 * (cb + 0.5)), strip_value(255 * (cr + 0.5)));
}

Pixel HSVColor::toRGB(Pixel p) {
    double h = p.r / 255.0, s = p.g / 255.0, v = p.b / 255.0;

    int i = floor(6 * h);
    double min = (1 - s) * v;
    double a = (v - min) * (double(int(360 * h) % 60) / 60);
    double inc = min + a;
    double dec = v - a;

    double r, g, b;
    switch (i) {
        case 0:
            r = v;
            g = inc;
            b = min;
            break;
        case 1:
            r = dec;
            g = v;
            b = min;
            break;
        case 2:
            r = min;
            g = v;
            b = inc;
            break;
        case 3:
            r = min;
            g = dec;
            b = v;
            break;
        case 4:
            r = inc;
            g = min;
            b = v;
            break;
        case 5:
            r = v;
            g = min;
            b = dec;
            break;
    }
    return Pixel(255 * r, 255 * g, 255 * b);
}

Pixel HSVColor::fromRGB(Pixel p) {
    double r = p.r / 255.0, g = p.g / 255.0, b = p.b / 255.0;
    double max = std::max(r, std::max(g, b));
    double min = std::min(r, std::min(g, b));

    double h, s, v;

    if (r == max) {
        if (g >= b) {
            h = 60 * (g - b) / (max - min);
        } else {
            h = 60 * (g - b) / (max - min) + 360;
        }
    } else if (g == max) {
        h = 60 * (b - r) / (max - min) + 120;
    } else {
        h = 60 * (r - g) / (max - min) + 240;
    }

    s = max != 0 ? (1 - min / max) : 0;
    v = max;

    return Pixel(h * 255 / 360, s * 255, v * 255);
}
