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
#include <sstream>
#include <map>

using namespace std;

//registradores
string PC;
string SP="0x8200";
vector<string> R;
string IR; //aponta para a instrucao atual

//flags
bool C=0, Ov=0, Z=0, S=0;

//estruturas de dados
queue<faixa> instrucoes;
stack<faixa> pilha;
queue<faixa> memoria;

void iniciarPilha(stack<faixa> &pi){
    pilha.push( faixa("0x8200", "")); 
}

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

void iniciaMemoriaDeDados(queue<faixa> &exMem){
    exMem.push(faixa(SP, ""));
}


void saida(){

    //iniciarPilha(pilha);

    for(int i=0;i<R.size();i++){
        string s = converteLongBiHexa(R[i]);
        formatarHexa(s);
        cout << "R" << i << ": " << s << "\n";
    }
    
    cout << "PC: " << PC << "\n" << "SP: " << SP << "\n" << "C:  " << C  << "\n"
    << "Ov: " << Ov << "\n" << "S:  " << S  << "\n" << "Z:  " << Z << "\n";

    cout << "pilha: " << "\n";

    while(!pilha.empty()){
        cout << pilha.top().end <<  " " << pilha.top().valor << "\n";
        pilha.pop();
    }

    cout << "memoria de dados: " << "\n";
    //memoria de dados
    while (!memoria.empty()){
        cout << memoria.front().end << " " << memoria.front().valor << "\n";
        memoria.pop();
    }
    
}

void JEQ(string &exPC, string exBinario) {
    string endereco = exBinario.substr(5, 9);
    formatarBinario(endereco);

    if (Z == 1 && C == 0) {
        string binarioPC = conveterInstrucao(exPC);

        bitset<16> b1(stoi(endereco, nullptr, 2));
        bitset<16> b2(stoi(binarioPC, nullptr, 2));

        int16_t num1 = static_cast<int16_t>(b1.to_ulong());
        int16_t num2 = static_cast<int16_t>(b2.to_ulong());

        int16_t resultado = num1 + num2;

        string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());
        formatarHexa(newPC);

        exPC = newPC;
    } else {
        instrucoes.pop();

        if (!instrucoes.empty()) {
            PC = instrucoes.front().end;
        }
    }
}


void JMP(string &exPC, string exBinario) {
    string b = exBinario.substr(5, 9);
    formatarBinario(b);

    int16_t deslocamento = static_cast<int16_t>(bitset<16>(b).to_ulong());
    if (b[0] == '1') {
        deslocamento -= 512;
    }

    int16_t pcAtual = stoi(exPC, nullptr, 16);
    pcAtual += deslocamento;

    stringstream ss;
    ss << hex << uppercase << pcAtual;
    string newPC = ss.str();

    formatarHexa(newPC);
    exPC = newPC;
}



void JLT(string &exPC, string exBinario) {
    string endereco = exBinario.substr(5, 9);
    formatarBinario(endereco);

    if (Z == 0 && C == 1) {
        string binarioPC = conveterInstrucao(exPC);

        bitset<16> b1(stoi(endereco, nullptr, 2));
        bitset<16> b2(stoi(binarioPC, nullptr, 2));

        int16_t num1 = static_cast<int16_t>(b1.to_ulong());
        int16_t num2 = static_cast<int16_t>(b2.to_ulong());

        int16_t resultado = num1 + num2;

        string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());
        formatarHexa(newPC);

        exPC = newPC;

    } else {
        instrucoes.pop();

        if (!instrucoes.empty()) {
            PC = instrucoes.front().end;
        }
    }
}

void JGT(string &exPC, string exBinario) {
    string endereco = exBinario.substr(5, 9);
    formatarBinario(endereco);

    if (Z == 0 && C == 0) {
        string binarioPC = conveterInstrucao(exPC);

        bitset<16> b1(stoi(endereco, nullptr, 2));
        bitset<16> b2(stoi(binarioPC, nullptr, 2));

        int16_t num1 = static_cast<int16_t>(b1.to_ulong());
        int16_t num2 = static_cast<int16_t>(b2.to_ulong());

        int16_t resultado = num1 + num2;

        string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());
        formatarHexa(newPC);

        exPC = newPC;
    } else {
        instrucoes.pop();

        if (!instrucoes.empty()) {
            PC = instrucoes.front().end;
        }
    }
}

string procurarInstrucao(std::string binario, vector<string> &reg){

    string dado1 = binario.substr(0,4);
    string dado2 = binario.substr(0,5);
    string dado3 = binario.substr(14,2);

    if(dado1=="0011"){//TODOLDR
        string r = binario.substr(5, 3);
        string r2 = binario.substr(8,3);

        string enderecoMemoria = reg[converterBiPraHexa(r2)-'0'];

        string valorMemoria;

        queue<faixa> tempMemoria = memoria;

        while (!tempMemoria.empty())
        {
            if(tempMemoria.front().end == enderecoMemoria){
                valorMemoria = tempMemoria.front().valor;
                break;
            }
            tempMemoria.pop();

        }
        
        if(valorMemoria.empty()) valorMemoria = "0000000000000000";

        reg[converterBiPraHexa(r)-'0']==valorMemoria;

        return "LDR";
    }else if(dado1=="0100"){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        int16_t num2 = static_cast<int16_t>(b2.to_ulong());
        int16_t num3 = static_cast<int16_t>(b3.to_ulong());

        int16_t resultado = num2 + num3;

        C = (resultado < num2) ? 1 : 0;
        Z = (resultado == 0) ? 1 : 0;
        S = (resultado & (1 << 15)) ? 1 : 0;

        bool sign2 = (num2 & (1 << 15)) != 0;
        bool sign3 = (num3 & (1 << 15)) != 0;
        bool signR = (resultado & (1 << 15)) != 0;

        Ov = (sign2 == sign3) && (sign2 != signR); // Se os operandos tinham o mesmo sinal, mas o resultado mudou


        //Ov = ((num2 > 0 && num3 > 0 && resultado < 0) || (num2 < 0 && num3 < 0 && resultado > 0)) ? 1 : 0;

        reg[converterBiPraHexa(r1) - '0'] = bitset<16>(resultado).to_string();

        return "ADD";
    }else if(dado1 == "0101"){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        int16_t num2 = static_cast<int16_t>(b2.to_ulong());
        int16_t num3 = static_cast<int16_t>(b3.to_ulong());

        int16_t resultado = num2 - num3;
        C = (resultado < num2) ? 1 : 0;
        Z = (resultado == 0) ? 1 : 0;
        S = (resultado & (1 << 15)) ? 1 : 0;

        bool sign2 = (num2 & (1 << 15)) != 0;  // Sinal de num2
        bool sign3 = (num3 & (1 << 15)) != 0;  // Sinal de num3
        bool signR = (resultado & (1 << 15)) != 0; // Sinal do resultado

        // Overflow ocorre se os sinais de num2 e num3 são iguais, mas o sinal do resultado é diferente
        Ov = (sign2 == sign3) && (sign2 != signR);


        //Ov = (((num2 ^ num3) < 0) && ((num2 ^ resultado) < 0)) ? 1 : 0;

        reg[converterBiPraHexa(r1) - '0'] = bitset<16>(resultado).to_string();
        return "SUB";
    }else if(dado1 == "0110"){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        int16_t num2 = static_cast<int16_t>(b2.to_ulong());
        int16_t num3 = static_cast<int16_t>(b3.to_ulong());


        int32_t resultado32 = static_cast<int32_t>(num2) * static_cast<int32_t>(num3);
        int16_t resultado = static_cast<int16_t>(resultado32);

        Z = (resultado == 0) ? 1 : 0;
        S = (resultado & (1 << 15)) ? 1 : 0;
        Ov = (resultado32 > INT16_MAX || resultado32 < INT16_MIN) ? 1 : 0;
        C = (resultado32 & 0xFFFF0000) ? 1 : 0; 

        reg[converterBiPraHexa(r1) - '0'] = bitset<16>(resultado).to_string();
                
        return "MUL";
    }else if(dado1 == "0111"){
        //moverPC(PC, instrucoes);
        string rd = binario.substr(6,3);
        string rm = binario.substr(9,3);
        string rn = binario.substr(12,3);

        bitset<16> n1(reg[converterBiPraHexa(rd)-'0']);
        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);
        bitset<16> n3(reg[converterBiPraHexa(rn)-'0']);


        //rd = rm + rn
        bitset<16> resultado(n2&n3);
        Z = (resultado.to_ulong() == 0) ? 1 : 0;
        S = resultado[15];

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();

        return "AND";
    }else if(dado1 == "1000"){
        //moverPC(PC, instrucoes);

        string rd = binario.substr(6,3);
        string rm = binario.substr(9,3);
        string rn = binario.substr(12,3);

        bitset<16> n1(reg[converterBiPraHexa(rd)-'0']);
        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);
        bitset<16> n3(reg[converterBiPraHexa(rn)-'0']);


        //rd = rm + rn
        bitset<16> resultado(n2|n3);

        Z = (resultado.to_ulong() == 0) ? 1 : 0;
        S = resultado[15];

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();
        return "ORR";
    }else if(dado1 == "1001"){
        //moverPC(PC, instrucoes);
        string rd = binario.substr(6,3);
        string rm = binario.substr(9,3);

        bitset<16> n1(reg[converterBiPraHexa(rd)-'0']);
        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);

        bitset<16> resultado(~n2);

        Z = (resultado.to_ulong() == 0) ? 1 : 0;
        S = resultado[15];

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();
        return "NOT";
    }else if(dado1 == "1010"){
        string rd = binario.substr(6,3);
        string rm = binario.substr(9,3);
        string rn = binario.substr(12,3);

        bitset<16> n1(reg[converterBiPraHexa(rd)-'0']);
        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);
        bitset<16> n3(reg[converterBiPraHexa(rn)-'0']);

        bitset<16> resultado(n2^n3);

        Z = (resultado.to_ulong() == 0) ? 1 : 0;
        S = resultado[15];

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();
     
        return "XOR";
    }else if(dado1 == "1011"){
 
        string rd = binario.substr(5,3);
        string rm = binario.substr(8,3);
        string lm = binario.substr(11,5);

        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);
        int shift = stoi(lm, nullptr, 2);

        bitset<16> resultado = n2 >> shift;

        Z = (resultado.to_ulong() == 0) ? 1 : 0;
        S = 0;
        C = (shift > 0) ? (n2[shift - 1]) : 0; 

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();

        return "SHR";
    }else if(dado1 == "1100") {
        string r = binario.substr(5,3);
        string r2 = binario.substr(8,3);
        string valor = binario.substr(11,5);

        int vezes = stoi(valor, nullptr, 2);

        bitset<16> regSrc(reg[converterBiPraHexa(r2) - '0']);
        bitset<16> resultado = regSrc << vezes;

        C = (vezes > 0) ? regSrc[16 - vezes] : 0;
        Z = (resultado.to_ulong() == 0) ? 1 : 0;
        S = resultado[15];
        Ov = (S != regSrc[15]) ? 1 : 0;

        reg[converterBiPraHexa(r) - '0'] = resultado.to_string();
        return "SHL";
    }else if(dado1 == "1101") {
        string r = binario.substr(5,3);
        string rm = binario.substr(8,3);
        //r = rm >> 1
        int regAlvo = converterBiPraHexa(rm) - '0';

        bitset<16> valor(reg[regAlvo]);
        bool ultimoBit = valor[0];

        valor >>= 1;
        if (ultimoBit) valor.set(15);

        C = ultimoBit;
        Z = (valor.to_ulong() == 0) ? 1 : 0;
        S = valor[15];
      
        reg[converterBiPraHexa(r)-'0'] = valor.to_string();
        return "ROR";
    }else if(dado1 == "1110") {
        string r = binario.substr(5,3);
        string rd = binario.substr(8,3);

        int regAlvo = converterBiPraHexa(rd) - '0';

        bitset<16> valor(reg[regAlvo]);
        bool primeiroBit = valor[15];

         valor <<= 1;
        if (primeiroBit) valor.set(0);

        C = primeiroBit;
        Z = (valor.to_ulong() == 0) ? 1 : 0;
        S = valor[15];

        reg[converterBiPraHexa(r)-'0'] = valor.to_string();
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
        string r1 = binario.substr(5,3);
        string r2 = binario.substr(8,3);
        reg[converterBiPraHexa(r1)-'0'] = reg[converterBiPraHexa(r2)-'0'];
        return "MOV1";
    }else if(dado2=="00011"){
        moverPC(PC, instrucoes);
        std::string rMOV = binario.substr(5,3);
        std::string vMOV = binario.substr(8,8);
       

        formatarBinario(vMOV);
        reg[converterBiPraHexa(rMOV)-'0'] = vMOV;

        return "MOV2";
    }else if(dado2=="00000"){
        if(dado3=="01"){
            string r = binario.substr(11,3);

            pilha.push(faixa(SP, converteLongBiHexa(reg[converterBiPraHexa(r)-'0'])));
            string guardar = pilha.top().valor;
            formatarHexa(guardar);
            SP = subHexa(SP, "0x0002");
            memoria.push(faixa(pilha.top().end, guardar));

            return "PUSH";
        }else if(dado3=="10"){
            SP = somaHexa(SP, "0x0002");
            string r = binario.substr(5,3);
            
            string v = conveterInstrucao(getValorSP(SP, pilha));

            formatarBinario(v);

            reg[converterBiPraHexa(r)-'0'] = v ;

            string guardar = converteLongBiHexa(v);
            formatarHexa(guardar);
            memoria.back().valor="";
            memoria.push(faixa(subHexa(SP, "0x0002"), guardar));
            pilha.pop();
            return "POP";
        }else if(dado3=="11"){

            string r1 = binario.substr(8,3);
            string r2 = binario.substr(11,3);
            

            std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
            std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);


            int16_t num1 = static_cast<int16_t>(b1.to_ulong());
            int16_t num2 = static_cast<int16_t>(b2.to_ulong());

            int16_t resultado = num1 - num2;


            Z = (num1 == num2) ? 1 : 0;
            C = (num1 < num2) ? 1 : 0;
            S = (resultado & (1 << 15)) ? 1 : 0;

            bool sign2 = (num1 & (1 << 15)) != 0;
            bool sign3 = (num2 & (1 << 15)) != 0;
            bool signR = (resultado & (1 << 15)) != 0; 

            Ov = (sign2 == sign3) && (sign2 != signR);

           // Ov = (((num1 ^ num2) < 0) && ((num1 ^ resultado) < 0)) ? 1 : 0;

            return "CMP";
        }else{
            return "instrucao nao encontrada";
        }
    }else if(dado2=="00001"){
       // cout << dado3;
        if(dado3=="00"){
            return "JMP";
        }else if(dado3=="01"){
            return "JEQ";
        }else if(dado3=="10"){
            return "JLT";
        }else if(dado3=="11"){
            return "JGT";
        }else{
            return "instrucao nao encontrada";
        }
    }else if(dado2=="00100"){
        string regNumStr;
        string regNumStr2;

        regNumStr = binario.substr(8, 3);
        regNumStr2 = binario.substr(11, 3);

        int regNum = stoi(regNumStr, nullptr, 2);
        int regNum2 = stoi(regNumStr2, nullptr, 2);

        string valor = R[regNum2];

        reg[regNum] = valor;

    return "STR1";;
    }else if (dado2 == "00101") {
        string regNumStr = binario.substr(8, 3);
        int regNum = stoi(regNumStr, nullptr, 2);

        string valor;  
        valor.append(binario.substr(5, 3));
        valor.append(binario.substr(11, 5));
        formatarBinario(valor);

        string valorBinario = conveterInstrucao(valor);
        R[regNum] = valorBinario;

        string enderecoMemoria = R[regNum];

        bool encontrado = false;
        queue<faixa> tempMemoria = memoria;
        queue<faixa> novaMemoria;

        while (!tempMemoria.empty()) {
            if (tempMemoria.front().end == enderecoMemoria) {
                tempMemoria.front().valor = valorBinario;
                encontrado = true;
            }
            novaMemoria.push(tempMemoria.front());
            tempMemoria.pop();
        }

        if (!encontrado) {
            memoria.push(faixa(enderecoMemoria, valorBinario));
        } else {
            memoria = novaMemoria;
        }

        return "STR2";


    }else{
        return "instrucao nao encontrada";
    }
    return "0";
}

void lerArquivo(string nomeDoArquivo) {
    fstream arquivo(nomeDoArquivo, ios::in);
    
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo: " << nomeDoArquivo << endl;
        return;
    }

    iniciaRegistradores(R);
    iniciarPilha(pilha);
    
    string end, valor;
    
    while (arquivo >> end >> valor) {
        end.pop_back();
        formatarHexa(end);
        instrucoes.push(faixa(end, valor));
    }

    arquivo.close();

    if (instrucoes.empty()) {
        cerr << "Erro: Nenhuma instrução encontrada no arquivo!" << endl;
        return;
    }

    PC = instrucoes.front().end;

    //cout << PC << "\n";

    while (!instrucoes.empty()) {
        if (PC == "0xFFFF") {
            cout << "HALT encontrado. Encerrando execução." << endl;
            break;
        }

        string instrucaoBinaria = getInstrucao(PC, instrucoes);
        string instrucao = conveterInstrucao(instrucaoBinaria);
        string resultado = procurarInstrucao(instrucao, R);

        if (resultado == "instrucao nao encontrada") {
            //cout << instrucaoBinaria << "\n";
            cerr << "Erro: Instrução não encontrada ou mal definida." << endl;
            break;
        }

        //cout << resultado << "\n";

        if (resultado == "JMP") {
            JMP(PC, instrucao);
            //instrucoes.pop();
        } else if(resultado == "JEQ"){
            JEQ(PC, instrucao);
            //instrucoes.pop();
        }else if(resultado == "JGT"){
            JGT(PC, instrucao);
            //instrucoes.pop();
        }else if(resultado == "JLT"){
            JLT(PC, instrucao);
            //instrucoes.pop();
        }else {
            instrucoes.pop();
            if (!instrucoes.empty()) {
                PC = instrucoes.front().end;
            }
        }
      //  cout << "\n PC: " << PC << "\n";
    }
}


#endif