#pragma once

namespace DBSchema {

    // --- Tabela CUSTOMER (baseada em User.h) ---
    // Usamos CPF como Chave Primária, pois é uma string única
    static const char* CUSTOMER = 
        "CREATE TABLE IF NOT EXISTS CUSTOMER ("
        "    ID        INT PRIMARY KEY AUTOINCREMENT,"
        "    CPF       TEXT NOT NULL,"
        "    NAME      TEXT NOT NULL,"
        "    EMAIL     TEXT NOT NULL,"
        "    BIRTHDAY  TEXT NOT NULL"
        ");";

    // --- Tabela ITEM (baseada em Product.h) ---
    static const char* ITEM = 
        "CREATE TABLE IF NOT EXISTS ITEM ("
        "    ID              INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    NAME            TEXT NOT NULL,"    // de product->getName()
        "    DESCRIPTION     TEXT,"
        "    OWNER           TEXT,"
        "    GENRE           TEXT,"
        "    IDIOM           TEXT,"
        "    RENT_VALUE      REAL,"      
        "    RECOMMENDED_AGE TEXT,"
        "    TYPE            TEXT NOT NULL," // "BOOK", "MOVIE", etc.
        "    STATUS          TEXT NOT NULL DEFAULT 'Disponivel'"
        ");";

    // --- Tabela BOOK (baseada em Book.h) ---
    static const char* BOOKS =
        "CREATE TABLE IF NOT EXISTS BOOK ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    AUTHOR      TEXT NOT NULL,"      // de book->getAuthor()
        "    PAGE_NUM    INTEGER NOT NULL,"   // de book->getNumPages()
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID)"
        ");";

    // --- Tabela MOVIE (baseada em Movie.h) ---
    static const char* MOVIES =
        "CREATE TABLE IF NOT EXISTS MOVIE ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    DIRECTOR    TEXT NOT NULL,"      // de movie->getDirector()
        "    MAIN_ACTORS TEXT,"               // de movie->getMainActors()
        "    DURATION    INTEGER,"           // de movie->getDuration()
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID)"
        ");";

    // --- Tabela VIDEOGAME (baseada em Video_Game.h) ---
    static const char* VIDEOGAMES =
        "CREATE TABLE IF NOT EXISTS VIDEOGAME ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    STYLE       TEXT,"               // de videogame->getStyle()
        "    PLATAFORM   TEXT NOT NULL,"      // de videogame->getPlataform()
        "    DURATION    INTEGER"            // de videogame->getDuration()
        ");";
        
    // --- Tabela BOARDGAME (baseada em Board_Game.h) ---
    static const char* BOARDGAMES =
        "CREATE TABLE IF NOT EXISTS BOARDGAME ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    STYLE       TEXT,"               // de boardgame->getStyle()
        "    PLAYERS_NUM INTEGER,"           // de boardgame->getNumPlayers()
        "    DURATION    INTEGER,"           // de boardgame->getDuration()
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID)"
        ");";

    static const char* RENTALS =
        "CREATE TABLE IF NOT EXISTS RENTALS ("
        "    ID            INTEGER PRIMARY KEY AUTOINCREMENT," // Para o transactionId
        "    CUSTOMER_CPF  TEXT NOT NULL,"    // Para o borrower (User.cpf)
        "    ITEM_ID       INTEGER NOT NULL," // Para o productId (Item.ID)
        "    START_DATE    TEXT NOT NULL,"    // de rental->getStartDate()
        "    DURATION      INTEGER,"         // de rental->getDuration()
        "    DAILY_RATE    REAL,"           // de rental->getDailyRate()
        "    STATUS        TEXT,"           // de rental->getStatus()
        "    FOREIGN KEY (CUSTOMER_CPF) REFERENCES CUSTOMER(CPF),"
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID)"
        ");";
}