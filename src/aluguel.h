#ifndef ALUGUEL_H
#define ALUGUEL_H
#include <string>
using namespace std;

class Aluguel{
    private:
    int idTransacao;
    int tempo;
    string QuemCedeu;
    string QuemRecebeu;
    double valorFinal;
    string dataInicio;
    string status;

    public:
    // Construtor
    Aluguel(int id, string cedente, string recebedor, int dias, string inicio, double valorDia);


    // Métodos
    void setStatus(const string& novoStatus);
    int getIdTransacao();
    int getTempo();
    double getValorFinal();
    string getStatus();
    string getQuemCedeu();
    string getQuemRecebeu();
    void setQuemCedeu(const string& locatario);
    void setQuemRecebeu(const string& locador);

    void setDataInicio(const string& dataInicio);
    string getDataInicio();
    string calcularDataFinal();
 



    // Funcionalidades
    bool estaAtrasado(const string& dataHoje);
    void concluirAluguel();
    
};
#endif

//void iniciarAluguel();
//void concluirAluguel(string dataDevolucao);
//void cancelarAluguel();
//bool estaAtrasado(string dataHoje);
//int diasRestantes(string dataHoje);
//double calcularMulta(string dataHoje, double taxaPorDia);
//double calcularValorFinal(string dataHoje, double taxaPorDia);

