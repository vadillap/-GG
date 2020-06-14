#include <iostream>
#include "PnmFile.h"
#include "PnmUtils.h"
#include "PnmColor.h"


using namespace std;


pair<bool, string>
validate(int count_fin, int count_fout) {
    if (count_fin != 1 && count_fin != 3) return {false, "input file count"};
    if (count_fout != 1 && count_fout != 3) return {false, "output file count"};

    return {true, "ok"};
}

PnmFile *open_pnm(string name) {
    PnmFile *pnm;
    try {
        pnm = new PnmFile(name);
    }
    catch (exception &ex) {
        cerr << "file open error: " << ex.what() << '\n';
        exit(1);
    }
    return pnm;
}

void save_pnm(string name, PnmFile *pnm) {
    try {
        pnm->savePnm(name);
    }
    catch (exception &ex) {
        cerr << "file write error:" << ex.what() << '\n';
        exit(1);
    }
}

string part_name(string name, int part) {
    name.insert(name.find('.'), "_" + to_string(part));
    return name;
}

PnmFile *multi_open_pnm(string &name) {
    PnmFile *r = open_pnm(part_name(name, 1));
    PnmFile *g = open_pnm(part_name(name, 2));
    PnmFile *b = open_pnm(part_name(name, 3));

    return merge_pnm(*r, *g, *b);
}

void multi_save_pnm(string &name, PnmFile *pnm) {
    int w = pnm->getWidth(), h = pnm->getHeight();
    PnmFile r(w, h), g(w, h), b(w, h);

    split_pnm(*pnm, r, g, b);

    save_pnm(part_name(name, 1), &r);
    save_pnm(part_name(name, 2), &g);
    save_pnm(part_name(name, 3), &b);
}


Color *get_by(string &type) {
    if (type == "RGB") return new RGBColor();
    if (type == "HSL") return new HSLColor();
    if (type == "HSV") return new HSVColor();
    if (type == "YCbCr.601") return new YBR601Color();
    if (type == "YCbCr.709") return new YBR709Color();
    if (type == "YCoCg") return new YOGColor();
    if (type == "CMY") return new CMYColor();

    cerr << "argument error: wrong color space " << type << '\n';
    exit(1);
}

void cli(int argc, char *argv[]) {
    if (argc == 11) {
        string fin, fout, from_cs, to_cs;
        int count_fin, count_fout;


        int i = 1;
        while (i != argc) {
            if (argv[i][0] != '-') {
                cerr << "argument error: specify option with '-'\n";
                exit(1);
            }
            try {
                switch (argv[i][1]) {
                    case 'f':
                        from_cs = argv[i + 1];
                        break;
                    case 't':
                        to_cs = argv[i + 1];
                        break;
                    case 'i':
                        count_fin = stoi(argv[i + 1]);
                        fin = argv[i + 2];
                        i += 1;
                        break;
                    case 'o':
                        count_fout = stoi(argv[i + 1]);
                        fout = argv[i + 2];
                        i += 1;
                        break;
                    default:
                        cerr << "argument error: unknown option " << argv[i] << '\n';
                        exit(1);
                }
            }
            catch (exception &ex) {
                cerr << "argument error: " << ex.what() << '\n';
                exit(1);
            }
            i += 2;
        }

        auto result = validate(count_fin, count_fout);

        if (!result.first) {
            cerr << "validate error: " << result.second << '\n';
            exit(0);
        }

        PnmFile *pnm;

        if (count_fin == 3) {
            pnm = multi_open_pnm(fin);
        } else {
            pnm = open_pnm(fin);
        }

        Color *in_cs = get_by(from_cs);
        Color *out_cs = get_by(to_cs);

        transformCS(*pnm, *in_cs, *out_cs);

        if (count_fout == 3) {
            multi_save_pnm(fout, pnm);
        } else {
            save_pnm(fout, pnm);
        }

    } else {
        cerr
                << "lab4.exe -f <from_color_space> -t <to_color_space> -i <count> <input_file_name> -o <count> <output_file_name>";
        exit(1);
    }
    exit(0);
}


int main(int argc, char *argv[]) {
    cli(argc, argv);
    return 0;
}
