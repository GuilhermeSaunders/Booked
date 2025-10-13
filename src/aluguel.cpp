#include "aluguel.h"

// Métodos
    // Settar o status do livro (Alugado, disponivel, em falta...)
    void Aluguel::setStatus(const string& novoStatus){
        status = novoStatus;
    }
    // Id da transação
    int Aluguel::getIdTransacao(){
        return idTransacao;
    }
    // Tempo que a pessoa deseja alugar o livro
    int Aluguel::getTempo(){
        if (tempo <= 0){
            tempo = 0;
        }
        return tempo;
    }
    // Preço final do livro (Dependendo do tempo, de multas, se estendeu, etc)
    double Aluguel::getValorFinal(){
        return valorFinal;
    }
    // Status
    string Aluguel::getStatus(){
        return status;
    }
    // Quem cedeu o livro
    string Aluguel::getQuemCedeu(){
        return QuemCedeu;
    }
    // Quem pegou o livro
    string Aluguel::getQuemRecebeu(){
        return QuemRecebeu;
    }
    // Settar quem cedeu
    void Aluguel::setQuemCedeu(const string& locatario){
        QuemCedeu = locatario;
    }
    // Settar quem recebeu
    void Aluguel::setQuemRecebeu(const string& locador){
        QuemRecebeu = locador;
    }
    // Pegar data de inicio para depois somar com tempo e verificar se esta atrasado
    void Aluguel::setDataInicio(const string& dataInicio){
        this->dataInicio = dataInicio;
    }
    string Aluguel::getDataInicio(){
        return dataInicio;
    }

    // Essa o chat fez, para calcular a Data final e ver se esta atrasado
    string Aluguel::calcularDataFinal() {
        // Exemplo simples: se dataInicio = "2025-10-08" e tempo = 7 dias
        // Aqui você precisa converter a string em ano, mes, dia
        int ano = stoi(dataInicio.substr(0,4));
        int mes = stoi(dataInicio.substr(5,2));
        int dia = stoi(dataInicio.substr(8,2));

        dia += tempo; // somando dias

        // Ajuste simples de meses (não cobre todos os casos, só exemplo)
        int diasNoMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        while(dia > diasNoMes[mes-1]){
            dia -= diasNoMes[mes-1];
            mes++;
            if(mes > 12){
                mes = 1;
                ano++;
            }
        }

        char buffer[11];
        sprintf(buffer,"%04d-%02d-%02d", ano, mes, dia);
        return string(buffer);
    }


    // Funcionalidades
    bool Aluguel::estaAtrasado(const string& dataHoje){
        string dataFim = calcularDataFinal();

        if(dataHoje > dataFim){
            return true;
        }
        return false;
    }
    void Aluguel::concluirAluguel(){
        status = "concluído";
    }
    
