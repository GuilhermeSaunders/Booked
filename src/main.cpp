#include <iostream>
#include "user.h"
#include "account.h"

#include "login.h"
#include "validate.h"
#include "rental.h"

#include "Product.h"
#include "Book.h"
#include "Movie.h"
#include "board_game.h"
#include "video_game.h"

#include "app.h" 
#include "repository.h" 

int main() {
    
    Repositorio db("aluguel.db");
    
    
    db.tablesStart();


    App application(db);

    application.run(); 

    std::cout << "Saindo do sistema. Até logo!" << std::endl;
    return 0;
}