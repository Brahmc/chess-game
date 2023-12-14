//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAAK_GAME_H
#define SCHAAK_GAME_H

#include "SchaakStuk.h"

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(SchaakStuk* s,int r, int k); // Verplaats stuk s naar rij r en kolom k

    bool inCheck(zw kleur);
    bool checkMate(zw kleur);
    bool staleMate(zw kleur);
    void setStartBord();

    SchaakStuk* getPiece(int r, int k);
    void setPiece(int r, int k, SchaakStuk* s);

private:
    SchaakStuk* board[8][8];
};

#endif //SCHAKEN_GAME_H


