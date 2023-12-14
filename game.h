//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAAK_GAME_H
#define SCHAAK_GAME_H

#include "ChessPiece.h"

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(ChessPiece* s, int r, int k); // Verplaats stuk s naar rij r en kolom k

    bool inCheck(zw kleur);
    bool checkMate(zw kleur);
    bool staleMate(zw kleur);
    void setStartBord();

    ChessPiece* getPiece(int r, int k);
    void setPiece(int r, int k, ChessPiece* s);

private:
    ChessPiece* board[8][8];
};

#endif //SCHAKEN_GAME_H


