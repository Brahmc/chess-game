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

    bool inCheck(bw kleur) const;
    bool checkMate(bw kleur);
    bool staleMate(bw kleur);
    void setStartBord();

    [[nodiscard]] ChessPiece* getPiece(int r, int k) const;
    void setPiece(int r, int k, ChessPiece* s);

    std::pair<int, int> getPosition(Piece::Type type, bw kleur) const;

private:
    ChessPiece* board[8][8];

    void clearBoard();

    bool noValidMoves(bw kleur);
};

#endif //SCHAKEN_GAME_H


