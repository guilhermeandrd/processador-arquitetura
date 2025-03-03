#ifndef FAIXA_H
#define FAIXA_H

#include <iostream>
#include <string>

using namespace std;

struct faixa{
    std::string end;
    std::string valor;

    faixa(std::string end, std::string valor){
        this->end = end;
        this->valor = valor;
    }
};

#endif