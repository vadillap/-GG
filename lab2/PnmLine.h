#include "PnmFile.h"
#include <cmath>

#ifndef CGRAPH_PNMLINE_H
#define CGRAPH_PNMLINE_H

void
drawLine(double x0, double y0, double x1, double y1, PnmFile &pnm, double thick = 1, double gamma = 2.2,
         unsigned int br = 255);

#endif //CGRAPH_PNMLINE_H
