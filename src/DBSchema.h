#pragma once

namespace DBSchema {

    // --- Tabela CUSTOMER (baseada em User.h) ---
    static const char* CUSTOMER = 
        "CREATE TABLE IF NOT EXISTS CUSTOMER ("
        "    ID        INTEGER PRIMARY KEY AUTOINCREMENT," // MUDANÇA: Era INT
        "    CPF       TEXT NOT NULL UNIQUE,"            // MUDANÇA: Adicionado UNIQUE
        "    NAME      TEXT NOT NULL,"
        "    EMAIL     TEXT NOT NULL UNIQUE,"            // MUDANÇA: Adicionado UNIQUE
        "    BIRTHDAY  TEXT NOT NULL"
        ");";

    // --- Tabela ACCOUNT (baseada em Account.h) ---
    static const char* ACCOUNT = 
        "CREATE TABLE IF NOT EXISTS ACCOUNT ("
        "    ID              TEXT PRIMARY KEY,"
        "    CUSTOMER_ID     INTEGER NOT NULL,"
        "    USERNAME        TEXT NOT NULL UNIQUE,"
        "    PASSWORD_HASH   INTEGER NOT NULL,"
        "    FOREIGN KEY (CUSTOMER_ID) REFERENCES CUSTOMER(ID)"
        ");";
           
    // --- Tabela ITEM (baseada em Product.h) ---
    static const char* ITEM = 
        "CREATE TABLE IF NOT EXISTS ITEM ("
        "    ID              INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    NAME            TEXT NOT NULL,"
        "    DESCRIPTION     TEXT,"
        "    OWNER_CPF       TEXT NOT NULL," // MUDANÇA: Nome mais claro
        "    GENRE           TEXT,"
        "    IDIOM           TEXT,"
        "    RENT_VALUE      REAL,"      
        "    RECOMMENDED_AGE TEXT,"
        "    TYPE            TEXT NOT NULL,"
        "    STATUS          TEXT NOT NULL DEFAULT 'Disponivel',"
        "    FOREIGN KEY (OWNER_CPF) REFERENCES CUSTOMER(CPF)" // Dono é um cliente
        ");";

    // --- Tabela BOOK (baseada em Book.h) ---
    static const char* BOOKS =
        "CREATE TABLE IF NOT EXISTS BOOK ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    AUTHOR      TEXT NOT NULL,"
        "    PAGE_NUM    INTEGER NOT NULL,"
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // MUDANÇA
        ");";

    // --- Tabela MOVIE (baseada em Movie.h) ---
    static const char* MOVIES =
        "CREATE TABLE IF NOT EXISTS MOVIE ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    DIRECTOR    TEXT NOT NULL,"
        "    MAIN_ACTORS TEXT,"
        "    DURATION    INTEGER,"
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // MUDANÇA
        ");";

    // --- Tabela VIDEOGAME (baseada em Video_Game.h) ---
    static const char* VIDEOGAMES =
        "CREATE TABLE IF NOT EXISTS VIDEOGAME ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    STYLE       TEXT,"
        "    PLATAFORM   TEXT NOT NULL,"
        "    DURATION    INTEGER,"
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // MUDANÇA
        ");";
        
    // --- Tabela BOARDGAME (baseada em Board_Game.h) ---
    static const char* BOARDGAMES =
        "CREATE TABLE IF NOT EXISTS BOARDGAME ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    STYLE       TEXT,"
        "    PLAYERS_NUM INTEGER,"
        "    DURATION    INTEGER,"
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // MUDANÇA
        ");";

    // --- Tabela RENTALS (baseada em Rental.h) ---
    // MUDANÇA CRÍTICA: Usando CUSTOMER_ID (int) ao invés de CPF (string)
    static const char* RENTALS =
        "CREATE TABLE IF NOT EXISTS RENTALS ("
        "    ID            INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    CUSTOMER_ID   INTEGER NOT NULL," // MUDANÇA
        "    ITEM_ID       INTEGER NOT NULL,"
        "    START_DATE    TEXT NOT NULL,"
        "    DURATION      INTEGER,"
        "    DAILY_RATE    REAL,"
        "    STATUS        TEXT,"
        "    FOREIGN KEY (CUSTOMER_ID) REFERENCES CUSTOMER(ID)," // MUDANÇA
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID)"
        ");";
}