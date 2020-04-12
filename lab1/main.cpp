#include <iostream>

#include "Pnm.h"

using namespace std;

int main(int argc, char *argv[]) {

    if (argc == 4) {
        string fin = argv[1], fout = argv[2];
        int action = argv[3][0] - '0';

        Pnm *pnm;

        try {
            pnm = new Pnm(fin);
        }
        catch (char const *err) {
            cerr << "unable to open the file: " << err << "\n";
            return 0;
        }
        try {
            switch (action) {
                case 0:
                    pnm->inverse();
                    break;
                case 1:
                    pnm->reflect_x();
                    break;
                case 2:
                    pnm->reflect_y();
                    break;
                case 3:
                    pnm->turn(1);
                    break;
                case 4:
                    pnm->turn(3);
                    break;
                default:
                    cout << "Unkown action\n";
                    return 0;
            }
        }
        catch (exception e) {
            cerr << "unexpected error: " << e.what() << "\n";
            return 0;
        }

        try {
            pnm->savePnm(fout);
        }
        catch (char const *err) {
            cerr << "unable to save the file: " << err << "\n";
            return 0;
        }
        cout << "OK\n";
        delete pnm;
    }
    else {
        cout << argv[0] << " <имя_входного_файла> <имя_выходного_файла> <преобразование>\n";
    }

    return 0;
}
