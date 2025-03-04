#ifndef LOGIC_H
#define LOGIC_H
#include <bitset>
#include <string>
#include <vector>
#include "conversor.h"

using namespace std;

string AND(string b1, string b2){

        bitset<16> n1(b1);
        bitset<16> n2(b2);


        //rd = rm + rn
        bitset<16> resultado(n1&n2);

        return resultado.to_string();
}

string ORR(string b1, string b2){

        bitset<16> n1(b1);
        bitset<16> n2(b2);


        //rd = rm + rn
        bitset<16> resultado(n1|n2);

        return resultado.to_string();
}

string XOR(string b1, string b2){

        bitset<16> n1(b1);
        bitset<16> n2(b2);


        //rd = rm + rn
        bitset<16> resultado(n1^n2);

        return resultado.to_string();
}

string NOT(string b1){

        bitset<16> n1(b1);


        //rd = rm + rn
        bitset<16> resultado(~n1);

        return resultado.to_string();
}


#endif