#ifndef CONVERSOR_H
#define CONVERSOR_H

#include <string>
#include <bitset>

using namespace std;

char converterBiPraHexa(std::string hexa){
    if(hexa.size()<4){
        std::string novoBi;
        int s = hexa.size(), i=0;
        while(s<4){
            novoBi.push_back('0');
            i++;
            s++;
        }
        novoBi.append(hexa);
        hexa = novoBi;
    }

    if (hexa=="0000") {
        return '0';
    } else if (hexa == "1111") {
        return 'F';
    } else if (hexa == "1110") {
        return 'E';
    } else if (hexa == "1101") {
        return 'D';
    } else if (hexa == "1100") {
        return 'C';
    } else if (hexa == "1011") {
        return 'B';
    } else if (hexa == "1010") {
        return 'A';
    } else if (hexa == "1001") {
        return '9';
    } else if (hexa == "1000") {
        return '8';
    } else if (hexa == "0111") {
        return '7';
    } else if (hexa == "0110") {
        return '6';
    } else if (hexa == "0101") {
        return '5';
    } else if (hexa == "0100") {
        return '4';
    } else if (hexa == "0011") {
        return '3';
    } else if (hexa == "0010") {
        return '2';
    } else if (hexa == "0001") {
        return '1';
    } else {
        return 'n';
    }

};

std::string conveterHexaPraBi(char hexa){
    if(hexa=='0'){
        return "0000";
    }else if(hexa=='1'){
        return "0001";
    }else if(hexa=='2'){
        return "0010";
    }else if(hexa=='3'){
        return "0011";
    }else if(hexa=='4'){
        return "0100";
    }else if(hexa=='5'){
        return "0101";
    }else if(hexa=='6'){
        return "0110";
    }else if(hexa == '7'){
        return "0111";
    }else if(hexa=='8'){
        return "1000";
    }else if(hexa=='9'){
        return "1001";
    }else if(hexa=='A'){
        return "1010";
    }else if(hexa=='B'){
        return "1011";
    }else if(hexa=='C'){
        return "1100";
    }else if(hexa=='D'){
        return "1101";
    }else if(hexa=='E'){
        return "1110";
    }else if(hexa=='F'){
        return "1111";
    }else{
        return "valor incorreto";
    }

};

//converte um binario de 16 bits
string converteLongBiHexa(string bi){
    string retorno;

    char hexa1 = converterBiPraHexa(bi.substr(0,4));
    char hexa2 = converterBiPraHexa(bi.substr(4,4));
    char hexa3 = converterBiPraHexa(bi.substr(8,4));
    char hexa4 = converterBiPraHexa(bi.substr(12,4));

    retorno.push_back(hexa1);
    retorno.push_back(hexa2);
    retorno.push_back(hexa3);
    retorno.push_back(hexa4);

    return retorno;
}

string conveterInstrucao(string instrucao){
    string retorno;

    for(int i=2;i<instrucao.size();i++){
        retorno.append(conveterHexaPraBi(instrucao[i]));
    }

    return retorno;
}


string somaHexa(string v1, string v2){
    string resultado;

    //converte a string 1 pra binario


    bitset<16> b1(conveterInstrucao(v1));
    bitset<16> b2(conveterInstrucao(v2));

    unsigned long num1 = b1.to_ulong();
    unsigned long num2 = b2.to_ulong();

    bitset<16> rB(num1 + num2);
    resultado.append("0x");
    resultado.append( converteLongBiHexa(rB.to_string()));

    return resultado;
    //converte a string 2 pra binario

    //soma os resultados

    //converte o resultado pra hexa

    //retorna o resultado;
}

string subHexa(string v1, string v2){
    string resultado;

    //converte a string 1 pra binario


    bitset<16> b1(conveterInstrucao(v1));
    bitset<16> b2(conveterInstrucao(v2));

    unsigned long num1 = b1.to_ulong();
    unsigned long num2 = b2.to_ulong();

    bitset<16> rB(num1 - num2);
    resultado.append("0x");
    resultado.append( converteLongBiHexa(rB.to_string()));

    return resultado;
}

void formatarBinario(string &bi){
    
    std::string novoBi;
        
    int s = bi.size(), i=0;

    while(s<16){
        novoBi.push_back('0');
        i++;
        s++;
    }

    novoBi.append(bi);
    bi = novoBi;
    
}

void formatarHexa(string &hexa){

    string novoHexa;
    novoHexa.append("0x");


    novoHexa.append(hexa);

    hexa = novoHexa;
    
}

string ajustarComplementoDois(string binario) {
    int tamanho = binario.size();
    int valorDecimal = stoi(binario, nullptr, 2); 

    if (binario[0] == '1') {  
        valorDecimal -= (1 << tamanho); 
    }
    return bitset<16>(valorDecimal).to_string();
}



#endif