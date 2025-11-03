// main.cpp
#include <iostream>
#include <string>
#include <limits> // Para limpar o buffer de entrada

// --- NOSSOS ARQUIVOS ---
#include "Repositorio.h"
#include "Customer\user.h"
#include "Product\Book.h"
// (Inclua os outros .h de produto quando for usá-los)

// --- Funções Auxiliares do Menu ---

// Limpa o buffer de entrada (previne bugs do std::cin)
void limparBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void mostrarMenuPrincipal() {
    std::cout << "\n=====================================" << std::endl;
    std::cout << "   Sistema de Locadora (Terminal)    " << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "1. Gerenciar Clientes" << std::endl;
    std::cout << "2. Gerenciar Produtos" << std::endl;
    std::cout << "3. Gerenciar Aluguéis" << std::endl;
    std::cout << "0. Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void mostrarMenuClientes() {
    std::cout << "\n--- Gerenciar Clientes ---" << std::endl;
    std::cout << "1. Cadastrar Novo Cliente" << std::endl;
    std::cout << "2. Listar Todos os Clientes" << std::endl;
    std::cout << "9. Voltar" << std::endl;
    std::cout << "Escolha uma opção: ";
}

void mostrarMenuProdutos() {
    std::cout << "\n--- Gerenciar Produtos ---" << std::endl;
    std::cout << "1. Cadastrar Novo Livro (BOOK)" << std::endl;
    std::cout << "2. Listar Produtos por Categoria" << std::endl;
    std::cout << "9. Voltar" << std::endl;
    std::cout << "Escolha uma opção: ";
}


// --- Funções de Ação ---

void cadastrarNovoCliente(Repositorio& repositorio) {
    std::string cpf, nome, email, aniversario;
    
    std::cout << "-- Novo Cliente --" << std::endl;
    std::cout << "CPF: ";
    std::getline(std::cin, cpf);
    std::cout << "Nome Completo: ";
    std::getline(std::cin, nome);
    std::cout << "Email: ";
    std::getline(std::cin, email);
    std::cout << "Data Nasc. (AAAA-MM-DD): ";
    std::getline(std::cin, aniversario);

    // 1. Cria o objeto POO (na memória)
    User novoUser(email, cpf, nome, aniversario);

    // 2. Entrega ao Repositório para salvar no banco
    if (repositorio.registerUser(&novoUser)) {
        std::cout << "Cliente cadastrado com sucesso! (ID: " << novoUser.getId() << ")" << std::endl;
    } else {
        std::cout << "ERRO: Falha ao cadastrar cliente." << std::endl;
    }
}

// (Função de exemplo - você precisa implementar a captura de dados)
void cadastrarNovoLivro(Repositorio& repositorio) {
    std::cout << "ERRO: Esta função ainda não foi implementada." << std::endl;
    // --- COMO IMPLEMENTAR ---
    // 1. Peça ao usuário todos os 9 campos (rentValue, owner, name, ... author, numPages)
    // 2. Crie o objeto: Book* novoLivro = new Book(rentValue, owner, ...);
    // 3. Chame o repositório: repositorio.cadastrarProduto(novoLivro);
    // 4. Lembre-se do 'delete novoLivro;'
}

void listarProdutos(Repositorio& repositorio) {
    std::string categoria;
    std::cout << "Qual categoria? (BOOK, MOVIE, VIDEOGAME, BOARDGAME): ";
    std::getline(std::cin, categoria);
    
    // 2. Pede ao Repositório para fazer o SELECT e imprimir
    repositorio.listProducts(categoria);
}


// =======================================================
// === PROGRAMA PRINCIPAL ===
// =======================================================
int main() {
    // 1. Cria o Repositório.
    // O construtor dele (em Repositorio.cpp) abre o banco.
    Repositorio repositorio("locadora.db");

    // 2. Manda o Repositório criar as tabelas (se não existirem)
    repositorio.tablesStart();

    int escolha, subEscolha;

    do {
        mostrarMenuPrincipal();
        std::cin >> escolha;
        limparBuffer(); // Sempre limpe o buffer após um 'cin >>'

        switch (escolha) {
            case 1: // Gerenciar Clientes
                do {
                    mostrarMenuClientes();
                    std::cin >> subEscolha;
                    limparBuffer();
                    
                    if (subEscolha == 1) {
                        cadastrarNovoCliente(repositorio);
                    } else if (subEscolha == 2) {
                        repositorio.listUsers(); // Chama o Repositório direto
                    }
                } while (subEscolha != 9);
                break;

            case 2: // Gerenciar Produtos
                do {
                    mostrarMenuProdutos();
                    std::cin >> subEscolha;
                    limparBuffer();
                    
                    if (subEscolha == 1) {
                        cadastrarNovoLivro(repositorio);
                    } else if (subEscolha == 2) {
                        listarProdutos(repositorio);
                    }
                } while (subEscolha != 9);
                break;
            
            case 3: // Gerenciar Aluguéis
                std::cout << "Menu de Aluguéis ainda não implementado." << std::endl;
                break;
        }

    } while (escolha != 0);

    std::cout << "Saindo do programa..." << std::endl;
    
    // 3. Fim do programa.
    // O destrutor do 'repositorio' (em Repositorio.cpp)
    // será chamado automaticamente, fechando o banco.
    return 0;
}