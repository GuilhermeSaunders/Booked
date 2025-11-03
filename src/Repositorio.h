#pragma once

#include <sqlite3.h>
#include <string>
#include <vector> // Para futuras funções de "get"

// Modelos de Dados
#include "Product.h"
#include "Book.h"
#include "Movie.h"
#include "video_game.h"
#include "board_game.h"
#include "user.h"
#include "account.h"
#include "rental.h"

class Repositorio {
private:
    sqlite3* db; 

    // Função auxiliar interna
    bool executeSQL(const std::string& sql); 

public:
    // Construtor e Destrutor
    Repositorio(const std::string& db_path);
    ~Repositorio();

    // Inicializa o banco de dados
    void tablesStart();

    // --- Funções de Customer/Account ---
    bool registerUser(User* user);
    bool registerAccount(const Account& account); 
    Account* getAccountByUsername(const std::string& username); 

    // --- Funções de Produto (CREATE) ---
    bool registerBook(Book* book);
    bool registerMovie(Movie* movie);
    bool registerVideoGame(Video_Game* videogame);
    bool registerBoardGame(Board_Game* boardgame);

    // --- Funções de Produto (READ) ---
    void listAllProducts(); // Lista todos
    void listProductsByType(const std::string& type); // Lista por tipo (BOOK, MOVIE, etc)

    // --- Funções de Rental ---
    bool registerRental(Rental* rental);
    bool updateItemStatus(int itrmId, const std::string& newStatus);
}; 