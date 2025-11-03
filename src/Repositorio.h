// Repositorio.h
#pragma once

#include <sqlite3.h>
#include <string>
#include "Product\Product.h" 
#include "Customer\user.h"
#include "BusinessRules\rental.h"

class Repositorio {
private:
    sqlite3* db; // O ponteiro da conexão vive aqui dentro

    // Função auxiliar interna, só o repositório usa
    bool executeSQL(const std::string& sql); 

public:
    // Construtor: Abre a conexão com o banco
    Repositorio(const std::string& db_path);

    // Destrutor: Fecha a conexão
    ~Repositorio();

    // Inicializa o banco de dados (cria as 7 tabelas)
    void tablesStart();

    bool registerProduct(Product* product); 
    bool registerUser(User* user);
    bool registerRental(Rental* rental);
    void listProducts(const std::string& categoria);
    void listUsers();
    void listRentals();
};