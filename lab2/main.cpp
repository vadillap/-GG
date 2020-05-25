#include <iostream>
#include "PnmFile.h"
#include "PnmLine.h"
#include <stdexcept>

using namespace std;


const string usage = " <имя_входного_файла> <имя_выходного_файла> <яркость_линии> <толщина_линии> <x_начальный> <y_начальный> <x_конечный> <y_конечный> <гамма>";


pair<bool, string> validate(int x0, int y0, int x1, int y1, double thick, double gamma, double br, int w, int h) {
    if (x0 < 0 || x0 > w) return {false, "x0"};
    if (y0 < 0 || y0 > h) return {false, "y0"};
    if (x1 < 0 || x1 > w) return {false, "x1"};
    if (y1 < 0 || y1 > h) return {false, "y1"};
    if (thick <= 0) return {false, "thick"};
    if (gamma <= 0) return {false, "gamma"};
    if (br < 0 || br > 255) return {false, "brightness"};
    return {true, "ok"};
}

int main(int argc, char *argv[]) {

    if (argc == 9 || argc == 10) {
        string fin, fout;
        int x0, y0, x1, y1, br;
        double thick, gamma = 2.2;

        try {
            fin = argv[1];
            fout = argv[2];
            br = stoi(argv[3]);
            thick = stof(argv[4]);
            x0 = stoi(argv[5]);
            y0 = stoi(argv[6]);
            x1 = stoi(argv[7]);
            y1 = stoi(argv[8]);
            if (argc == 10) {
                gamma = stof(argv[9]);
            }
        }
        catch (exception &ex) {
            cerr << "arg parse error: " << ex.what() << '\n';
            return 1;
        }
        PnmFile *pnm;
        try {
            pnm = new PnmFile(fin);
        }
        catch (exception &ex) {
            cerr << "file open error: " << ex.what() << '\n';
            return 1;
        }

        auto result = validate(x0, y0, x1, y1, thick, gamma, br, pnm->getWidth(), pnm->getHeight());

        if (!result.first) {
            cerr << "arg validate error: " << result.second << '\n';
            return 1;
        }

        drawLine(x0, y0, x1, y1, *pnm, thick, gamma, br);

        try {
            pnm->savePnm(fout);
        }
        catch (exception &ex) {
            cerr << "file write error:" << ex.what() << '\n';
            return 1;
        }
        cout << "ok" << '\n';

    } else {
        cerr << argv[0] << usage;
    }

    return 0;
}
