//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "game.h"

Game::Game() {}

Game::~Game() {}

// Zet het bord klaar; voeg de stukken op de jusite plaats toe
void Game::setStartBord() {
    clearBoard();

    for (int i = 0; i < 8; i+= 7) {
        zw color = i == 0 ? zwart : wit;
        board[i][0] = new Rook(color);
        board[i][1] = new Knight(color);
        board[i][2] = new Bishop(color);
        board[i][3] = new Queen(color);
        board[i][4] = new King(color);
        board[i][5] = new Bishop(color);
        board[i][6] = new Knight(color);
        board[i][7] = new Rook(color);
    }
    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn(zwart);
        board[6][i] = new Pawn(wit);
    }
}

void Game::clearBoard() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
}

/* Verplaats stuk s naar positie (r,k)
 * Als deze move niet mogelijk is, wordt false teruggegeven
 * en verandert er niets aan het schaakbord.
 * Anders wordt de move uitgevoerd en wordt true teruggegeven
 */
bool Game::move(ChessPiece* s, int r, int k) {
    int currentR;
    int currentK;
    bool found = false;
    for (int i = 0; i < 8 && !found; i++)
        for (int j = 0; j < 8; j++){
            if (board[i][j] == s) {
                found = true;
                currentR = i;
                currentK = j;
                break;
            }
        }

    std::vector<std::pair<int, int>> moves = s->getMoves(currentR, currentK, *this);
    if (std::find(moves.begin(), moves.end(), std::make_pair(r, k)) == moves.end()) return false;
    board[currentR][currentK] = nullptr;
    board[r][k] = s;
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
ChessPiece* Game::getPiece(int r, int k) {
    return board[r][k];
}

/* Zet het schaakstuk waar s naar verwijst neer op rij r, kolom k.
 * Als er al een schaakstuk staat, wordt het overschreven.
 * Bewaar in jouw datastructuur de *pointer* naar het schaakstuk,
 * niet het schaakstuk zelf.
 */
void Game::setPiece(int r, int k, ChessPiece* s)
{
    board[r][k] = s;
}