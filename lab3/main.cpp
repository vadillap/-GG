#include <iostream>
#include "PnmFile.h"
#include "PnmDithering.h"
#include "PnmUtils.h"


using namespace std;


pair<bool, string>
validate(int dithering, int bit, double gamma) {
    if (dithering < 0 || dithering > 7) return {false, "dithering"};
    if (bit <= 0 || bit > 8) return {false, "bit"};
    if (gamma < 0) return {false, "gamma"};
    return {true, "ok"};
}


void cli(int argc, char *argv[]) {
    if (argc == 7) {
        string fin, fout;
        int dithering, bit;
        bool altGamma, gradient;
        double gamma = 0;

        try {
            fin = argv[1];
            fout = argv[2];
            gradient = stoi(argv[3]);
            dithering = stoi(argv[4]);
            bit = stoi(argv[5]);
            gamma = stof(argv[6]);
        }
        catch (exception &ex) {
            cerr << "arg parse error: " << ex.what() << '\n';
            exit(1);
        }
        PnmFile *pnm;
        try {
            pnm = new PnmFile(fin);
        }
        catch (exception &ex) {
            cerr << "file open error: " << ex.what() << '\n';
            exit(1);
        }

        if (gamma != 0) {
            pnm->setGamma(new valueGamma(gamma));
        }

        if (gradient) {
            makeGradient(*pnm);
        }
        auto result = validate(dithering, bit, gamma);

        if (!result.first) {
            cerr << "arg validate error: " << result.second << '\n';
            exit(0);
        }

        Dithering *d;

        switch (dithering) {
            case 0:
                d = new NoDithering();
                break;
            case 1:
                d = new BayerDithering();
                break;
            case 2:
                d = new RandomDithering();
                break;
            case 3:
                d = new FloydDithering();
                break;
            case 4:
                d = new JJNDithering();
                break;
            case 5:
                d = new SierraDithering();
                break;
            case 6:
                d = new AtkinsonDithering();
                break;
            case 7:
                d = new HalftoneDithering();
                break;
        }

        d->dither(*pnm, bit);

        try {
            pnm->savePnm(fout);
        }
        catch (exception &ex) {
            cerr << "file write error:" << ex.what() << '\n';
            exit(1);
        }
    } else {
        cerr << "program.exe <имя_входного_файла> <имя_выходного_файла> <градиент> <дизеринг> <битность> <гамма>\n";
        exit(1);
    }
}


int main(int argc, char *argv[]) {
    cli(argc, argv);
    return 0;
}
