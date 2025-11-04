#pragma once

/**
 * @file DBSchema.h
 * @brief Defines the SQL table creation strings for the application.
 *
 * This namespace contains all the "CREATE TABLE IF NOT EXISTS"
 * strings used by the Repository to initialize the database.
 */
namespace DBSchema {

    /**
     * @brief The CUSTOMER table.
     * Stores the personal profile data for each user.
     * (Based on User.h)
     */
    static const char* CUSTOMER =
        "CREATE TABLE IF NOT EXISTS CUSTOMER ("
        "    ID        INTEGER PRIMARY KEY AUTOINCREMENT," // CHANGED: Was INT
        "    CPF       TEXT NOT NULL UNIQUE,"            // CHANGED: Added UNIQUE
        "    NAME      TEXT NOT NULL,"
        "    EMAIL     TEXT NOT NULL UNIQUE,"            // CHANGED: Added UNIQUE
        "    BIRTHDAY  TEXT NOT NULL"
        ");";

    /**
     * @brief The ACCOUNT table.
     * Stores login credentials and links to a CUSTOMER.
     * (Based on Account.h)
     */
    static const char* ACCOUNT =
        "CREATE TABLE IF NOT EXISTS ACCOUNT ("
        "    ID              TEXT PRIMARY KEY,"
        "    CUSTOMER_ID     INTEGER NOT NULL,"
        "    USERNAME        TEXT NOT NULL UNIQUE,"
        "    PASSWORD_HASH   INTEGER NOT NULL,"
        "    FOREIGN KEY (CUSTOMER_ID) REFERENCES CUSTOMER(ID)"
        ");";

    /**
     * @brief The ITEM table.
     * This is the "parent" table for all rentable products.
     * It stores all common product data.
     * (Based on Product.h)
     */
    static const char* ITEM =
        "CREATE TABLE IF NOT EXISTS ITEM ("
        "    ID              INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    NAME            TEXT NOT NULL,"
        "    DESCRIPTION     TEXT,"
        "    OWNER_CPF       TEXT NOT NULL," // CHANGED: Clearer name
        "    GENRE           TEXT,"
        "    IDIOM           TEXT,"
        "    RENT_VALUE      REAL,"
        "    RECOMMENDED_AGE TEXT,"
        "    TYPE            TEXT NOT NULL," // e.g., "BOOK", "MOVIE"
        "    STATUS          TEXT NOT NULL DEFAULT 'Disponivel'," // "Available"
        "    FOREIGN KEY (OWNER_CPF) REFERENCES CUSTOMER(CPF)" // Owner is a customer
        ");";

    /**
     * @brief The BOOK table.
     * Stores book-specific data, linked to an ITEM.
     * (Based on Book.h)
     */
    static const char* BOOKS =
        "CREATE TABLE IF NOT EXISTS BOOK ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    AUTHOR      TEXT NOT NULL,"
        "    PAGE_NUM    INTEGER NOT NULL,"
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // CHANGED
        ");";

    /**
     * @brief The MOVIE table.
     * Stores movie-specific data, linked to an ITEM.
     * (Based on Movie.h)
     */
    static const char* MOVIES =
        "CREATE TABLE IF NOT EXISTS MOVIE ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    DIRECTOR    TEXT NOT NULL,"
        "    MAIN_ACTORS TEXT,"
        "    DURATION    INTEGER," // in minutes
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // CHANGED
        ");";

    /**
     * @brief The VIDEOGAME table.
     * Stores video game-specific data, linked to an ITEM.
     * (Based on Video_Game.h)
     */
    static const char* VIDEOGAMES =
        "CREATE TABLE IF NOT EXISTS VIDEOGAME ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    STYLE       TEXT,"
        "    PLATAFORM   TEXT NOT NULL,"
        "    DURATION    INTEGER," // in hours
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // CHANGED
        ");";

    /**
     * @brief The BOARDGAME table.
     * Stores board game-specific data, linked to an ITEM.
     * (Based on Board_Game.h)
     */
    static const char* BOARDGAMES =
        "CREATE TABLE IF NOT EXISTS BOARDGAME ("
        "    ITEM_ID     INTEGER PRIMARY KEY,"
        "    STYLE       TEXT,"
        "    PLAYERS_NUM INTEGER,"
        "    DURATION    INTEGER," // in minutes
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID) ON DELETE CASCADE" // CHANGED
        ");";

    /**
     * @brief The RENTALS table.
     * Stores all rental transaction records.
     * (Based on Rental.h)
     * CRITICAL CHANGE: Uses CUSTOMER_ID (int) instead of CPF (string)
     */
    static const char* RENTALS =
        "CREATE TABLE IF NOT EXISTS RENTALS ("
        "    ID            INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    CUSTOMER_ID   INTEGER NOT NULL," // CHANGED
        "    ITEM_ID       INTEGER NOT NULL,"
        "    START_DATE    TEXT NOT NULL,"
        "    DURATION      INTEGER," // in days
        "    DAILY_RATE    REAL,"
        "    STATUS        TEXT," // e.g., "Active", "Complete"
        "    FOREIGN KEY (CUSTOMER_ID) REFERENCES CUSTOMER(ID)," // CHANGED
        "    FOREIGN KEY (ITEM_ID) REFERENCES ITEM(ID)"
        ");";
}