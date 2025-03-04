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

    //memoria de dados
}

void JEQ(string &exPC, string exBinario, vector<string> reg){

    string b = exBinario.substr(5,9);
    formatarBinario(b);
    string l = converteLongBiHexa(b);

    if(Z==1&&C==0){
        //string s = somaHexa(exPC, l);
        //exPC = s;
    }

}

void JMP(string &exPC, string exBinario) {
    // Extrai os 9 bits do deslocamento (posição 5 até 13)
    string b = exBinario.substr(5, 9);
    cout << "Binário extraído: " << b << "\n";

    // Converte para decimal
    int deslocamento = stoi(b, nullptr, 2);

    // Ajuste de complemento de dois (caso negativo)
    if (deslocamento & 0x100) { // Bit 9 indica número negativo
        deslocamento -= 0x200;  // Converte para -512 a 511
    }

    // Converte PC de string hexadecimal para decimal
    int pcAtual = stoi(exPC, nullptr, 16);

    // Soma deslocamento
    pcAtual += deslocamento;

    // Converte de volta para hexadecimal
    stringstream ss;
    ss << hex << uppercase << pcAtual;
    exPC = ss.str();

    string formatar = "0x00";
    formatar.append(exPC);
    exPC = formatar;
  //  formatarHexa(exPC);

    cout << "Novo PC: " << exPC << endl;
}





string procurarInstrucao(std::string binario, vector<string> &reg){

    string dado1 = binario.substr(0,4);
    string dado2 = binario.substr(0,5);
    string dado3 = binario.substr(14,2);

    if(dado1.compare("0111")==0){//TODOLDR
        //moverPC(PC, instrucoes);


        return "LDR";
    }else if(dado1.compare("0100")==0){
        //moverPC(PC, instrucoes);
        std::string r1 = binario.substr(5,3);//5,6,7
        std::string r2 = binario.substr(8,3);
        std::string r3 = binario.substr(11,3);

        std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        unsigned long num1 = b1.to_ulong();
        unsigned long num2 = b2.to_ulong();
        unsigned long num3 = b3.to_ulong();

        unsigned long resultado = num2 + num3;
        if (resultado > 0xFFFF) C = 1; // Carry ativado se passar de 16 bits

        reg[converterBiPraHexa(r1) - '0'] = bitset<16>(resultado).to_string();
        Z = (resultado == 0) ? 1 : 0; // Z ativado se o resultado for zero
        S = (resultado & (1 << 15)) ? 1 : 0;
        Ov = (((num2 ^ num3) >= 0) && ((num2 ^ resultado) < 0)) ? 1 : 0;

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

        unsigned long resultado = num2 - num3;
        C = (num2 < num3) ? 1 : 0; // Carry ativado se houve underflow
        Z = (resultado == 0) ? 1 : 0;
        S = (resultado & (1 << 15)) ? 1 : 0;
        Ov = (((num2 ^ num3) < 0) && ((num2 ^ resultado) < 0)) ? 1 : 0;

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

        unsigned long num1 = b1.to_ulong();
        unsigned long num2 = b2.to_ulong();
        unsigned long num3 = b3.to_ulong();

        unsigned long resultado = num2 * num3;
        Z = (resultado == 0) ? 1 : 0;
        S = (resultado & (1 << 15)) ? 1 : 0;
        Ov = (resultado > 32767 || resultado < -32768) ? 1 : 0;

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

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();
        return "ORR";
    }else if(dado1 == "1001"){
        //moverPC(PC, instrucoes);
        string rd = binario.substr(6,3);
        string rm = binario.substr(9,3);

        bitset<16> n1(reg[converterBiPraHexa(rd)-'0']);
        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);


        //rd = rm + rn
        bitset<16> resultado(~n2);

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();
        return "NOT";
    }else if(dado1 == "1010"){
        string rd = binario.substr(6,3);
        string rm = binario.substr(9,3);
        string rn = binario.substr(12,3);

        bitset<16> n1(reg[converterBiPraHexa(rd)-'0']);
        bitset<16> n2(reg[converterBiPraHexa(rm)-'0']);
        bitset<16> n3(reg[converterBiPraHexa(rn)-'0']);


        //rd = rm + rn
        bitset<16> resultado(n2^n3);

        reg[converterBiPraHexa(rd)-'0'] = resultado.to_string();
        //moverPC(PC, instrucoes);
        return "XOR";
    }else if(dado1 == "1011"){ //TODO SHR
        //moverPC(PC, instrucoes);

        //oi
        cout << " oi " << endl;
        return "SHR";
    }else if(dado1 == "1100"){ //TODO SHL
        //moverPC(PC, instrucoes);

        //reg
        //valor numero de vezes que vou deslocar pra esquerda

        //string binario do registrador

        //fila <char> binario

        //valor vezes
            //pop apagaria o valor da frente
            //push "0"


        return "SHL";
    }else if(dado1 == "1101"){ //TODO ROR
        //moverPC(PC, instrucoes);
        return "ROR";
    }else if(dado1 == "1110"){ //TODO Rol
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
        string r1 = binario.substr(5,3);
        string r2 = binario.substr(8,3);
        //cout << r1 << " " << r2;
        reg[converterBiPraHexa(r1)-'0'] = reg[converterBiPraHexa(r2)-'0'];
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
            pilha.pop();
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
            long resultado = num1 - num2;


            Z = (num1 == num2) ? 1 : 0;
            C = (num1 < num2) ? 1 : 0;
            S = (resultado & (1 << 15)) ? 1 : 0;
            Ov = (((num1 ^ num2) < 0) && ((num1 ^ resultado) < 0)) ? 1 : 0;

            return "CMP";
        }else{
            return "instrucao nao encontrada";
        }
    }else if(dado2=="00001"){
        if(dado3=="00"){
            //moverPC(PC, instrucoes);
            return "JMP";
        }else if(dado3=="01"){//TODO terminar JEQ
            //cout <<"\n" << PC << "\n";
            return "JEQ";
        }else if(dado3=="10"){ //TODO JLT
            return "JLT";
            //moverPC(PC, instrucoes);
        }else if(dado3=="11"){ //TODO JGT
            //moverPC(PC, instrucoes);
            return "JGT";
        }else{
            return "instrucao nao encontrada";
        }
    }else if(dado2=="00100"){

        return "STR1";
    }else if(dado2=="00101"){
        
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
    
    // Lendo o arquivo e armazenando instruções na fila
    while (arquivo >> end >> valor) {
        end.pop_back();
        formatarHexa(end);
        instrucoes.push(faixa(end, valor));
    }

    arquivo.close();

    // Se não houver instruções, retorna
    if (instrucoes.empty()) {
        cerr << "Erro: Nenhuma instrução encontrada no arquivo!" << endl;
        return;
    }

    // PC recebe o endereço da primeira instrução
    PC = instrucoes.front().end;

    cout << PC << "\n";

    // Executa as instruções
    while (!instrucoes.empty()) {
        if (PC == "0xFFFF") {
            cout << "HALT encontrado. Encerrando execução." << endl;
            break;
        }

        // Obtém e converte a instrução
        string instrucaoBinaria = getInstrucao(PC, instrucoes);
        string instrucao = conveterInstrucao(instrucaoBinaria);
        string resultado = procurarInstrucao(instrucao, R);

        if (resultado == "instrucao nao encontrada") {
            cerr << "Erro: Instrução não encontrada ou mal definida." << endl;
            break;
        }

        cout << resultado << "\n";

        // Executa a instrução e altera o PC
        if (resultado == "JMP") {
            JMP(PC, instrucao);
            instrucoes.pop();
        } else {
            instrucoes.pop();
            if (!instrucoes.empty()) {
                PC = instrucoes.front().end;
            }
        }
        cout << "\n PC: " << PC << "\n";
    }
}


#endif