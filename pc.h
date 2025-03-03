#ifndef PC_H
#define PC_H

#include <string>
#include <iostream>
#include <queue>
#include "faixa.h"

using namespace std;

string getInstrucao(string PC, queue<faixa> ex){

    while(!ex.empty()){
        if(ex.front().end==PC){
            return ex.front().valor;
        }
        ex.pop();
    }

    return "nao encontrado";
}

void moverPC(string &PC, queue<faixa> instrucoes){
    instrucoes.pop();
    PC = instrucoes.front().end;
    PC.pop_back();
}



#endif