//
// Created by Вадим Гаврилов on 13.06.2020.
//

#include <cmath>
#include <algorithm>
#include "PnmColor.h"

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
    double r = p.r, g = p.g, b = p.b;
    return Pixel(r + g - b, r + b, r - g - b);
}

Pixel YOGColor::fromRGB(Pixel p) {
    double r = p.r, g = p.g, b = p.b;
    return Pixel(r / 4 + g / 2 + b / 4, r / 2 - b / 2, -r / 4 + g / 2 - b / 4);
}

Pixel YBR709Color::toRGB(Pixel p) {
    double y = p.r, cb = p.g, cr = p.b;

    double r = y + 1.402 * (cr - 128);
    double g = y - 0.34414 * (cb - 128) - 0.71414 * (cr - 128);
    double b = y + 1.772 * (cb - 128);

    return Pixel(r, g, b);
}

Pixel YBR709Color::fromRGB(Pixel p) {
    double r = p.r, g = p.g, b = p.b;

    double y = 0.299 * r + 0.587 * g + 0.114 * b;
    double cb = 128 - 0.168736 * r - 0.331264 * g + 0.5 * b;
    double cr = 128 + 0.5 * r - 0.418688 * g - 0.081312 * b;

    return Pixel(y, cb, cr);
}

Pixel YBR601Color::toRGB(Pixel p) {
    double y = p.r / 256.0, cb = p.g / 256.0, cr = p.b / 256.0;

    double r = 298.082 * y + 408.583 * cr - 222.921;
    double g = 298.082 * y - 100.291 * cb - 208.120 * cr + 135.576;
    double b = 298.082 * y + 516.412 * cb - 276.836;

    return Pixel(r, g, b);
}

Pixel YBR601Color::fromRGB(Pixel p) {
    double r = p.r / 256.0, g = p.g / 256.0, b = p.b / 256.0;

    double y = 16 + 65.738 * r + 129.057 * g + 25.064 * b;
    double cb = 128 - 37.945 * r - 74.494 * g + 112.439 * b;
    double cr = 128 + 112.439 * r - 94.154 * g - 18.285 * b;

    return Pixel(y, cb, cr);
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
