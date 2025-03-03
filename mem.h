#ifndef MEM_H
#define MEM_H

#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>
#include "conversor.h"
#include "pc.h"
#include "faixa.h"

using namespace std;



std::string PC;


bool C=0, Ov=0, Z=0, S=0;

vector<string> R;

queue<faixa> instrucoes;

stack<faixa> pilha;

void iniciarPilha(stack<faixa> &pi){
    pilha.push( faixa("0x8200", "")); 
}

string SP="0x8200";



//implementacao do SP e pilha

//set do SP

//achar valor correspondente ao SP

string getValorSP(string SP, stack<faixa> pilha){

    while(!pilha.empty()){

        if(pilha.top().end==SP){
            return pilha.top().valor;
        }

        pilha.pop();
    }

    return "valor invalido";
}

void setValorSP(string SP, string valor, stack<faixa> &pilha){
    stack<faixa> auxiliar;

    while(!pilha.empty()){
        if(pilha.top().end==SP){
            pilha.top().valor=valor;
            auxiliar.push(pilha.top());
            pilha.pop();
            break;
        }
        auxiliar.push(pilha.top());
        pilha.pop();
    }

    while(!auxiliar.empty()){
        pilha.push(auxiliar.top());
        auxiliar.pop();
    }

}


void iniciaRegistradores(vector<string> &R){
    for(int i=0;i<8;i++){
        R.push_back("0000000000000000");
    }
}




//mov


//string getValorPC(string PC, queue<faixa> )
//add
//nop
//CMP
//SUB
//POP
//PSH
//JMP

//flags

//
void saida(){

    iniciarPilha(pilha);

    for(int i=0;i<R.size();i++){
        cout << "R" << i << ": " << converteLongBiHexa(R[i]) << "\n";
    }

    cout << "PC: " << PC << "\n" << "SP: " << SP << "\n" << "C:  " << C  << "\n"
    << "Ov: " << Ov << "\n" << "S:  " << S  << "\n" << "Z:  " << Z;

}
std::string procurarInstrucao(std::string binario, vector<string> &reg){
    std::string dado1 = binario.substr(0,4);
    std::string dado2 = binario.substr(0,5);
    std::string dado3 = binario.substr(14,2);

    if(dado1.compare("0111")==0){
        //moverPC(PC, instrucoes);
        return "LDR";
    }else if(dado1.compare("0100")==0){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        unsigned long num1 = b1.to_ulong();
        unsigned long num2 = b2.to_ulong();
        unsigned long num3 = b3.to_ulong();

        std::bitset<16> resultado(num2+num3);

        reg[converterBiPraHexa(r1)-'0'] = resultado.to_string();

        return "ADD";
    }else if(dado1 == "0101"){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        unsigned long num1 = b1.to_ulong();
        unsigned long num2 = b2.to_ulong();
        unsigned long num3 = b3.to_ulong();

        std::bitset<16> resultado(num2-num3);

        reg[converterBiPraHexa(r1)-'0'] = resultado.to_string();

        return "SUB";
    }else if(dado1 == "0110"){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        unsigned long num1 = b1.to_ulong();
        unsigned long num2 = b2.to_ulong();
        unsigned long num3 = b3.to_ulong();

        std::bitset<16> resultado(num2*num3);

        reg[converterBiPraHexa(r1)-'0'] = resultado.to_string();

        return "MUL";
    }else if(dado1 == "0111"){
        //moverPC(PC, instrucoes);
        return "AND";
    }else if(dado1 == "1000"){
        //moverPC(PC, instrucoes);
        return "ORR";
    }else if(dado1 == "1001"){    
        //moverPC(PC, instrucoes);
        return "NOT";
    }else if(dado1 == "1010"){
        //moverPC(PC, instrucoes);
        return "XOR";
    }else if(dado1 == "1011"){
        //moverPC(PC, instrucoes);
        return "SHR";
    }else if(dado1 == "1100"){
        //moverPC(PC, instrucoes);
        return "SHL";
    }else if(dado1 == "1101"){
        //moverPC(PC, instrucoes);
        return "ROR";
    }else if(dado1 == "1110"){
        //moverPC(PC, instrucoes);
        return "ROL";
    }else if(binario=="0000000000000000"){
        //moverPC(PC, instrucoes);
        saida();
        return "NOP";
    }else if(binario=="1111111111111111"){
        //moverPC(PC, instrucoes);
        return "HALT";
    }else if(dado2=="00010"){
        //moverPC(PC, instrucoes);
        /*string r1 = binario.substr(5,3);
        string r2 = binario.substr(8,3);
        cout << r1 << " " << r2;
        reg[converterBiPraHexa(r1)-'0'] = reg[converterBiPraHexa(r2)-'0'];*/
        return "MOV1";
    }else if(dado2=="00011"){
        moverPC(PC, instrucoes);
        std::string rMOV = binario.substr(5,3);
        std::string vMOV = binario.substr(8,8);
        //cout << binario << endl;
        //registrador
        //cout << converterBiPraHexa(rMOV);
        formatarBinario(vMOV);
        reg[converterBiPraHexa(rMOV)-'0'] = vMOV;
        //valor
        //registrador <- valor
        return "MOV2";
    }else if(dado2=="00000"){
        if(dado3=="01"){
            string r = binario.substr(11,3);
            //somar hexadecimal
            pilha.push(faixa(somaHexa(SP,"0x0002"), converteLongBiHexa(reg[converterBiPraHexa(r)-'0'])));

            SP = somaHexa(SP, "0x0002");

            //moverPC(PC, instrucoes);
            return "PUSH";
        }else if(dado3=="10"){
            string r = binario.substr(5,3);
            string v = conveterInstrucao(getValorSP(SP, pilha));
            formatarBinario(v);
            reg[converterBiPraHexa(r)-'0'] = v ;
            SP = subHexa(SP, "0x0002");
            //moverPC(PC, instrucoes);
            return "POP";
        }else if(dado3=="11"){
            //moverPC(PC, instrucoes);

            string r1 = binario.substr(8,3);
            string r2 = binario.substr(11,3);
            

            std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
            std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);

            unsigned long num1 = b1.to_ulong();
            unsigned long num2 = b2.to_ulong();

            std::bitset<16> resultado(num1-num2);

            string r = resultado.to_string();

            if(num1==num2){
                Z=1;
            }else if(num1<num2){
                C=1;
            }
            return "CMP";
        }else{
            return "instrucao nao definida";
        }
    }else if(dado2=="00001"){
        if(dado3=="00"){
            //moverPC(PC, instrucoes);
            return "JMP";
        }else if(dado3=="01"){
            string b = binario.substr(5,9);
            formatarBinario(b);
            string l = converteLongBiHexa(b);

            if(Z==1&&C==0){
                PC = somaHexa(PC, l);
            }
            //cout <<"\n" << PC << "\n";
            //moverPC(PC, instrucoes);
            return "JEQ";
        }else if(dado3=="10"){
            return "JLT";
            //moverPC(PC, instrucoes);
        }else if(dado3=="11"){
            //moverPC(PC, instrucoes);
            return "JGT";
        }else{
            return "instrucao nao encontrada";
        }
    }else{
        return "instrucao nao achada";
    }
    return "0";
}






void lerArquivo(string nomeDoArquivo){

    fstream arquivo(nomeDoArquivo,ios::in);

    iniciaRegistradores(R);

    iniciarPilha(pilha);

    string end, valor;

    while(arquivo>>end>>valor){
        instrucoes.push(faixa(end,valor));
    }

    //PC recebe o da proxima instrucao (a primeira)
    PC = instrucoes.front().end;

    while(!instrucoes.empty()){

        //executa a instrucao
        cout << procurarInstrucao(conveterInstrucao(getInstrucao(PC, instrucoes)),R) << "\n";

        //muda o valor da instrucao
        instrucoes.pop();
        if(instrucoes.front().end!=""){
            PC = instrucoes.front().end;
        }
        //

    }

    arquivo.close();
}



//para o push eu vou ter que implementar a pilha mesmo




#endif