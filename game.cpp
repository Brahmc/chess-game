//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {}

Game::~Game() {}

// Zet het bord klaar; voeg de stukken op de jusite plaats toe
void Game::setStartBord() {}

/* Verplaats stuk s naar positie (r,k)
 * Als deze move niet mogelijk is, wordt false teruggegeven
 * en verandert er niets aan het schaakbord.
 * Anders wordt de move uitgevoerd en wordt true teruggegeven
 */
bool Game::move(SchaakStuk* s, int r, int k) {
    return true;
}

// Geeft true als kleur inCheck staat
bool Game::inCheck(zw kleur) {
    return false;
}

// geeft true als kleur checkMate staat
bool Game::checkMate(zw kleur) {
    return false;
}

/* Geeft true als kleur pat staat
 *  (staleMate = geen geldige zet mogelijk, maar kleur staat niet inCheck;
 * dit resulteert in een gelijkspel)
 */
bool Game::staleMate(zw kleur) {
    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         if (board[i][j].getMoves().size() == 0) {
    //             return false;
    //         }
    //     }
    // }
    // return true;
    return false;
}

/* Geeft een pointer naar het schaakstuk dat op rij r, kolom k staat
 * Als er geen schaakstuk staat op deze positie, geef nullptr terug
 */
SchaakStuk* Game::getPiece(int r, int k) {
    return board[r][k];
}

/* Zet het schaakstuk waar s naar verwijst neer op rij r, kolom k.
 * Als er al een schaakstuk staat, wordt het overschreven.
 * Bewaar in jouw datastructuur de *pointer* naar het schaakstuk,
 * niet het schaakstuk zelf.
 */
void Game::setPiece(int r, int k, SchaakStuk* s)
{
    board[r][k] = s;
}