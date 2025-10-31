//main no terminal e essas merdas

//função do Hub - chama outras funções para cada respectiva página
//funções das páginas

//uma espécie de "recursão" entre funções

#include <iostream>
#include "Customer/user.h"
#include "Customer/account.h"

#include "BusinessRules/login.h"
#include "BusinessRules/validate.h"
#include "BusinessRules/rental.h"

#include "Product/Product.h"
#include "Product/Book.h"
#include "Product/Movie.h"
#include "Product/board_game.h"
#include "Product/video_game.h"

using namespace std;
//OBS DE SPIDER: ACHO LEGAL PEGAR ESSAS FUNÇÕES 
//E IMPLEMENTAÇÕES DE CADA PÁGINA EM ARQUIVOS SEPARADOS DE
//.CPP E .H, PRA DEIXAR O MAIN MAIS LIMPO E ORGANIZADO

//LOGIN OR SIGN UP
void accountPage();
    // cout << "===== Tela de Login =====" << endl;
    // cout << "1. Login" << endl;
    // cout << "2. Cadastrar" << endl;
    // cout << "3. Sair" << endl;
    // cout << "Escolha uma opcao: ";
void loginPage();
    // cout << "===== Login =====" << endl;
    // cout << "Digite seu nome de usuário: ";
    // getline(cin, username);
    // cout << "Digite sua senha: ";
    // getline(cin, password);
    // // Lógica de login
    // cout << "Login realizado com sucesso!" << endl;
    // cout << "======================" << endl;
void registerPage();
    // cout << "===== Cadastro =====" << endl;
    // cout << "Digite seu email: ";
    // getline(cin, email);
    // cout << "Digite seu CPF: ";
    // getline(cin, cpf);
    // cout << "Digite seu nome completo: ";
    // getline(cin, fullname);
    // cout << "Digite sua data de nascimento (DD/MM/AAAA): ";
    // getline(cin, birthday);
    // cout << "Digite seu nome de usuário: ";
    // getline(cin, username);
    // cout << "Digite sua senha: ";
    // getline(cin, password);
    // // Lógica de cadastro
    // cout << "Cadastro realizado com sucesso!" << endl;
    // cout << "======================" << endl;

//MAIN HUB
void showMainMenu();
    // cout << "===== Menu Principal =====" << endl;
    // cout << "1. Catálogo" << endl;
    // cout << "2. Adicionar Produto" << endl;
    // cout << "3. Perfil" << endl;
    // cout << "4. Meus Produtos" << endl;
    // cout << "5. Produtos Alugados" << endl;
    // cout << "6. Logout" << endl;";
    // cout << "Escolha uma opcao: ";

//SHOW STUFF PAGES
void catalogPage();
    // cout << "===== Catálogo de Produtos =====" << endl;
    // cout << "1. Ver Todos os Produtos" << endl;
    // cout << "2. Filtro: Ver Livros" << endl;
    // cout << "3. Filtro: Ver Video Games" << endl;
    // cout << "4. Filtro: Ver Board Games" << endl;
    // cout << "5. Filtro: Ver Filmes" << endl;
    // cout << "6. Voltar ao Menu Principal" << endl;
    // cout << "Escolha uma opcao: ";
void showBooks();
    // cout << "===== Lista de Livros =====" << endl;
    // for (const auto& book : books) {
    //     cout << "ID: " << book.getId() << ", Nome: " << book.getName() << ", Gênero: " << book.getGenre() << endl;
    // }
    // cout << "===========================" << endl;
void showVideoGames();
    // cout << "===== Lista de Video Games =====" << endl;
    // for (const auto& videoGame : videoGames) {
    //     cout << "ID: " << videoGame.getId() << ", Nome: " << videoGame.getName() << ", Gênero: " << videoGame.getGenre() << endl;
    // }
    // cout << "===============================" << endl;
void showBoardGames();
    // cout << "===== Lista de Board Games =====" << endl;
    // for (const auto& boardGame : boardGames) {
    //     cout << "ID: " << boardGame.getId() << ", Nome: " << boardGame.getName() << ", Gênero: " << boardGame.getGenre() << endl;
    // }
    // cout << "===============================" << endl;
void showMovies();
    // cout << "===== Lista de Filmes =====" << endl;
    // for (const auto& movie : movies) {
    //     cout << "ID: " << movie.getId() << ", Nome: " << movie.getName() << ", Gênero: " << movie.getGenre() << endl;
    // }
    // cout << "===========================" << endl; 

//ADD STUFF PAGES
void addProductPage();
    // cout << "===== Adicionar Novo Produto =====" << endl;
    // cout << "1. Adicionar Livro" << endl;
    // cout << "2. Adicionar Video Game" << endl;
    // cout << "3. Adicionar Board Game" << endl;
    // cout << "4. Adicionar Filme" << endl;
    // cout << "5. Voltar ao Menu Principal" << endl;
    // cout << "Escolha uma opcao: ";
void addBook();
    // cout << "===== Adicionar Livro =====" << endl;
    // cout << "Digite o nome do livro: ";
    // getline(cin, name);
    // cout << "Digite a descrição do livro: ";
    // getline(cin, description);
    // cout << "Digite o gênero do livro: ";
    // getline(cin, genre);
    // cout << "Digite o idioma do livro: ";
    // getline(cin, idiom);
    // cout << "Digite a idade recomendada para o livro: ";
    // getline(cin, recommendedAge);
    // cout << "Digite o valor de aluguel do livro: ";
    // cin >> rentValue;
    // cin.ignore(); // Limpa o buffer do cin
    // // Gerar um ID único para o livro (pode ser melhorado)
    // string id = "book_" + to_string(books.size() + 1);
    // Book newBook(rentValue, id, currentUser.getId(), name, description, idiom, recommendedAge, genre, false);
    // books.push_back(newBook);
    // cout << "Livro adicionado com sucesso!" << endl;
void addVideoGame();
    // cout << "===== Adicionar Video Game =====" << endl;
    // cout << "Digite o nome do video game: ";
    // getline(cin, name);
    // cout << "Digite a descrição do video game: ";
    // getline(cin, description);
    // cout << "Digite o gênero do video game: ";
    // getline(cin, genre);
    // cout << "Digite o idioma do video game: ";
    // getline(cin, idiom);
    // cout << "Digite a idade recomendada para o video game: ";
    // getline(cin, recommendedAge);
    // cout << "Digite o valor de aluguel do video game: ";
    // cin >> rentValue;
    // cin.ignore(); // Limpa o buffer do cin
    // // Gerar um ID único para o video game (pode ser melhorado)
    // string id = "videogame_" + to_string(videoGames.size() + 1);
    // VideoGame newVideoGame(rentValue, id, currentUser.getId(), name, description, idiom, recommendedAge, genre, false);
    // videoGames.push  _back(newVideoGame);
    // cout << "Video Game adicionado com sucesso!" << endl;
void addBoardGame();
    // cout << "===== Adicionar Board Game =====" << endl;
    // cout << "Digite o nome do board game: ";
    // getline(cin, name);
    // cout << "Digite a descrição do board game: ";
    // getline(cin, description);
    // cout << "Digite o gênero do board game: ";
    // getline(cin, genre); 
    // cout << "Digite o idioma do board game: ";
    // getline(cin, idiom);
    // cout << "Digite a idade recomendada para o board game: ";
    // getline(cin, recommendedAge);
    // cout << "Digite o valor de aluguel do board game: ";
    // cin >> rentValue;
    // cin.ignore(); // Limpa o buffer do cin
    // // Gerar um ID único para o board game (pode ser melhorado)
    // string id = "boardgame_" + to_string(boardGames.size() + 1
    // BoardGame newBoardGame(rentValue, id, currentUser.getId(), name, description, idiom, recommendedAge, genre, false);
    // boardGames.push_back(newBoardGame);
    // cout << "Board Game adicionado com sucesso!" << endl;
void addMovie();
    // cout << "===== Adicionar Filme =====" << endl;
    // cout << "Digite o nome do filme: ";
    // getline(cin, name);
    // cout << "Digite a descrição do filme: ";
    // getline(cin, description);
    // cout << "Digite o gênero do filme: ";
    // getline(cin, genre);
    // cout << "Digite o idioma do filme: ";
    // getline(cin, idiom);
    // cout << "Digite a idade recomendada para o filme: ";
    // getline(cin, recommendedAge);
    // cout << "Digite o valor de aluguel do filme: ";
    // cin >> rentValue;
    // cin.ignore(); // Limpa o buffer do cin
    // // Gerar um ID único para o filme (pode ser melhorado)
    // string id = "movie_" + to_string(movies.size() + 1);
    // Movie newMovie(rentValue, id, currentUser.getId(), name, description, idiom, recommendedAge, genre, false);
    // movies.push_back(newMovie);
    // cout << "Filme adicionado com sucesso!" << endl;

//PROFILE, MY PRODUCTS, RENTED PRODUCTS, LOGOUT PAGES
void profilePage();
    // cout << "===== Perfil do Usuário =====" << endl;
    // cout << "1. Ver Informações do Perfil" << endl;
    // cout << "2. Editar Informações do Perfil" << endl;
    // cout << "3. Deletar Conta" << endl;
    // cout << "3. Voltar ao Menu Principal" << endl;
    // cout << "Escolha uma opcao: ";
void myProductsPage();
    // cout << "===== Meus Produtos =====" << endl;
    // cout << "1. Ver Meus Produtos" << endl;
    // cout << "2. Remover Produto" << endl;
    // cout << "3. Voltar ao Menu Principal" << endl;
    // cout << "Escolha uma opcao: ";
void rentedProductsPage();
    // cout << "===== Produtos Alugados =====" << endl;
    // cout << "1. Ver Produtos Alugados" << endl;
    // cout << "2. Devolver Produto" << endl;
    // cout << "3. Voltar ao Menu Principal" << endl;
    // cout << "Escolha uma opcao: ";
void logOutPage();
    // cout << "Você saiu com sucesso!" << endl;
    // cout << "============================" << endl;  

//RENTAL PAGE
void rentProductPage();
    // cout << "===== Alugar Produto =====" << endl;
    // cout << "Digite o ID do produto que deseja alugar: ";
    // getline(cin, productId);
    // // Lógica para alugar o produto
    // cout << "Produto alugado com sucesso!" << endl;
    // cout << "==========================" << endl;

int main() {
    //IMPLEMENTAÇÃO DAS FUNÇÕES AQUI!!!!!!!!!!!!!!!!
    return 0;
}