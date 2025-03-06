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
    while (!memoria.empty()){
        cout << memoria.front().end << " " << memoria.front().valor << "\n";
        memoria.pop();
    }
    
}

void JEQ(string &exPC, string exBinario){

    string endereco = exBinario.substr(5, 9);
    formatarBinario(endereco);

    if (Z==1&&C==0) {
        string binarioPC = conveterInstrucao(exPC);


    bitset<16> b1(endereco);
    bitset<16> b2(binarioPC);

    int16_t num1 = static_cast<int16_t>(b1.to_ulong());
    int16_t num2 = static_cast<int16_t>(b2.to_ulong());
    int16_t resultado = num1 + num2;

    string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());

    formatarHexa(newPC);
    exPC = newPC;
    }else{
         instrucoes.pop();
            if (!instrucoes.empty()) {
                PC = instrucoes.front().end;
            }
    }

}

void JMP(string &exPC, string exBinario) { //TODO ajeitar essa trocha
    string b = exBinario.substr(5, 9);
    //cout << "Binário extraído: " << b << "\n";

    cout << b;

    formatarBinario(b);

    //fazer tratamento de negativo

    string binarioPC = conveterInstrucao(exPC);
    /*
    std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
        std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);
        std::bitset<16> b3(reg[converterBiPraHexa(r3)-'0']);

        int16_t num2 = static_cast<int16_t>(b2.to_ulong());
        int16_t num3 = static_cast<int16_t>(b3.to_ulong());

        int16_t resultado = num2 - num3;*/


    bitset<16> b1(b);
    bitset<16> b2(binarioPC);

    int16_t num1 = static_cast<int16_t>(b1.to_ulong());
    int16_t num2 = static_cast<int16_t>(b2.to_ulong());
    int16_t resultado = num1 + num2;

    string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());

    formatarHexa(newPC);
    exPC = newPC;
    
   /* int deslocamento = stoi(b, nullptr, 2);


    if (deslocamento & 0x100) { 
        deslocamento -= 0x200;  
    }

    int pcAtual = stoi(exPC, nullptr, 16);

    pcAtual += deslocamento;

    stringstream ss;
    ss << hex << uppercase << pcAtual;
    exPC = ss.str();

    string formatar = "0x00";
    formatar.append(exPC);
    exPC = formatar;

    cout << "Novo PC: " << exPC << endl;*/
}


void JLT(string &exPC, string exBinario){
    string endereco = exBinario.substr(5, 9);
    formatarBinario(endereco);

    if (Z==0&&C==1) {


    string binarioPC = conveterInstrucao(exPC);


    bitset<16> b1(endereco);
    bitset<16> b2(binarioPC);

    int16_t num1 = static_cast<int16_t>(b1.to_ulong());
    int16_t num2 = static_cast<int16_t>(b2.to_ulong());
    int16_t resultado = num1 + num2;

    string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());

    formatarHexa(newPC);
    exPC = newPC;
    }else{
         instrucoes.pop();
            if (!instrucoes.empty()) {
                PC = instrucoes.front().end;
            }
    }

}

void JGT(string &exPC, string  exBinario){
      string endereco = exBinario.substr(5, 9);
    formatarBinario(endereco);

    if (Z==0&&C==0) {
     
        string binarioPC = conveterInstrucao(exPC);


    bitset<16> b1(endereco);
    bitset<16> b2(binarioPC);

    int16_t num1 = static_cast<int16_t>(b1.to_ulong());
    int16_t num2 = static_cast<int16_t>(b2.to_ulong());
    int16_t resultado = num1 + num2;

    string newPC = converteLongBiHexa(bitset<16>(resultado).to_string());

    formatarHexa(newPC);
    exPC = newPC;
    }else{
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

    if(dado1.compare("0011")==0){//TODOLDR
        string r = binario.substr(5, 3);
        string r2 = binario.substr(8,3);

        //r = [r2]

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
    }else if(dado1.compare("0100")==0){
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
        if (resultado > 0xFFFF) C = 1;

        reg[converterBiPraHexa(r1) - '0'] = bitset<16>(resultado).to_string();
        Z = (resultado == 0) ? 1 : 0;
        S = (resultado & (1 << 15)) ? 1 : 0;
        Ov = ((num2 > 0 && num3 > 0 && resultado < 0) || (num2 < 0 && num3 < 0 && resultado > 0)) ? 1 : 0;

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
        C = (num2 < num3) ? 1 : 0;
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

        int16_t num2 = static_cast<int16_t>(b2.to_ulong());
        int16_t num3 = static_cast<int16_t>(b3.to_ulong());

        int16_t resultado = num2 * num3;
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
    }else if(dado1 == "1100"){ //TODO usar as minhas funcoes aqui
        //reg
        //valor numero de vezes que vou deslocar pra esquerda
        string r = binario.substr(5,3);
        string r2 = binario.substr(8,3);
        string valor = binario.substr(11,4);

        int vezes = converterBiPraHexa(valor)-'0'; 

        string copia = reg[converterBiPraHexa(r2)-'0'];

        copia = copia.substr(vezes) + string(vezes, '0');

        cout << "copia : "<< copia << "\n";

        reg[converterBiPraHexa(r)-'0'] = copia;
        
        return "SHL";
    }else if(dado1 == "1101"){ //TODO ROR[
        string r = binario.substr(5, 3); 
    int regAlvo = converterBiPraHexa(r)-'0';

    string valor = reg[regAlvo];

    char ultimoBit = valor.back(); 
    valor.pop_back();              
    valor = ultimoBit + valor;  

    reg[regAlvo] = valor;

        //moverPC(PC, instrucoes);
        return "ROR";
    }else if(dado1 == "1110"){ //TODO Rol
        string r = binario.substr(5, 3);
        int regAlvo = converterBiPraHexa(r)-'0';

        string valor = reg[regAlvo];

        char primeiroBit = valor.front(); 
        valor.erase(0, 1);                
        valor += primeiroBit;            

        reg[regAlvo] = valor;
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

            pilha.push(faixa(SP, converteLongBiHexa(reg[converterBiPraHexa(r)-'0'])));
            SP = subHexa(SP, "0x0002");

            return "PUSH";
        }else if(dado3=="10"){
            SP = somaHexa(SP, "0x0002");
            string r = binario.substr(5,3);
            
            string v = conveterInstrucao(getValorSP(SP, pilha));

            formatarBinario(v);

            reg[converterBiPraHexa(r)-'0'] = v ;

            pilha.pop();
            return "POP";
        }else if(dado3=="11"){

            string r1 = binario.substr(8,3);
            string r2 = binario.substr(11,3);
            

            std::bitset<16> b1(reg[converterBiPraHexa(r1)-'0']);
            std::bitset<16> b2(reg[converterBiPraHexa(r2)-'0']);


            int16_t num1 = static_cast<int16_t>(b1.to_ulong());
            int16_t num2 = static_cast<int16_t>(b2.to_ulong());

            int16_t resultado = num1 + num2;


            Z = (num1 == num2) ? 1 : 0;
            C = (num1 < num2) ? 1 : 0;
            S = (resultado & (1 << 15)) ? 1 : 0;
            Ov = (((num1 ^ num2) < 0) && ((num1 ^ resultado) < 0)) ? 1 : 0;

            return "CMP";
        }else{
            return "instrucao nao encontrada";
        }
    }else if(dado2=="00001"){
        cout << dado3;
        if(dado3=="00"){
            return "JMP";
        }else if(dado3=="01"){//TODO terminar JEQ
            return "JEQ";
        }else if(dado3=="10"){ //TODO JLT
            return "JLT";
        }else if(dado3=="11"){ //TODO JGT
            return "JGT";
        }else{
            return "instrucao nao encontrada";
        }
    }else if(dado2=="00100"){ //TODO fazer mexer na memoria de dados
        string regNumStr;
        string regNumStr2;
        
        regNumStr = binario.substr(8, 3);
        regNumStr2 = binario.substr(11, 3);
        
        int regNum = stoi(regNumStr, nullptr, 2);
        int regNum2 = stoi(regNumStr2, nullptr, 2);
        
        if (regNum < 0 || regNum > 7 || regNum2 < 0 || regNum2 > 7) {
    cerr << "Erro: Número de registrador inválido: " << regNum << endl;
    return "";
        }
    
        string valor = R[regNum2];
    
        R[regNum] =  valor;
    
        
        return "STR1";
    }else if(dado2=="00101"){ //TODO fazer mexer na memoria de dados
        string regNumStr;

        regNumStr = binario.substr(8, 3);
        
        int regNum = stoi(regNumStr, nullptr, 2);
        
        /*if (regNum < 0 || regNum > 7) {
            cerr << "Erro: Número de registrador inválido: " << regNum << endl;
            return "";
        }*/

        string valor;
        
        valor.append(binario.substr(5,3));
        valor.append(binario.substr(11,5));
        formatarBinario(valor);

        string valorBinario = conveterInstrucao(valor); 

        R[regNum] = valorBinario;

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

    cout << PC << "\n";

    while (!instrucoes.empty()) {
        if (PC == "0xFFFF") {
            cout << "HALT encontrado. Encerrando execução." << endl;
            break;
        }

        string instrucaoBinaria = getInstrucao(PC, instrucoes);
        string instrucao = conveterInstrucao(instrucaoBinaria);
        string resultado = procurarInstrucao(instrucao, R);

        if (resultado == "instrucao nao encontrada") {
            cout << instrucaoBinaria << "\n";
            cerr << "Erro: Instrução não encontrada ou mal definida." << endl; //TODO ver esse aqui
            break;
        }

        cout << resultado << "\n";

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
        cout << "\n PC: " << PC << "\n";
    }
}


#endif