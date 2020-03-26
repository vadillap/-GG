//
// Created by Вадим Гаврилов on 20.02.2020.
//

#ifndef CGRAPH_PNM_H
#define CGRAPH_PNM_H


#include <cstdint>
#include <string>

class Pnm {
private:
    char type;
    unsigned int w, h;
    uint8_t *data;
    uint8_t max_value;


public:
    Pnm(std::string path);

    ~Pnm();

    void savePnm(std::string path);

    void inverse();

    void reflect_x();
    void reflect_y();

    void turn(int count);
};


#endif //CGRAPH_PNM_H
