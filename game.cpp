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
        bw color = i == 0 ? black : white;
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
        board[1][i] = new Pawn(black);
        board[6][i] = new Pawn(white);
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
    for (int i = 0; i < 8 && !found; i++) {
        for (int j = 0; j < 8; j++){
            if (board[i][j] == s) {
                found = true;
                currentR = i;
                currentK = j;
                break;
            }
        }
    }

    std::vector<std::pair<int, int>> moves = s->getAllowedMoves(currentR, currentK, *this);
    if (std::find(moves.begin(), moves.end(), std::make_pair(r, k)) == moves.end()) return false;
    board[currentR][currentK] = nullptr;
    board[r][k] = s;
    s->triggerMoveEvent(currentR, currentK, r, k, *this);
    return true;
}

// Geeft true als kleur inCheck staat
bool Game::inCheck(bw kleur) const {
    // brute force can be optimized
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* p = getPiece(i, j);
            if (p == nullptr || p->getColor() == kleur) continue;
            for (auto move : p->getMoves(i, j, *this)) {
                auto chessPiece = board[move.first][move.second];
                if (chessPiece != nullptr && chessPiece->piece().type() == Piece::King &&
                        chessPiece->getColor() == kleur) {
                    return true;
                }
            }
        }
    }
    return false;
}

// geeft true als kleur checkMate staat
bool Game::checkMate(bw kleur) {
    return noValidMoves(kleur) && inCheck(kleur);
}

/* Geeft true als kleur pat staat
 *  (staleMate = geen geldige zet mogelijk, maar kleur staat niet inCheck;
 * dit resulteert in een gelijkspel)
 */
bool Game::staleMate(bw kleur) {
    return noValidMoves(kleur) && !inCheck(kleur);
}

bool Game::noValidMoves(bw kleur) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j]->getAllowedMoves(i, j, *this).empty()) {
                return false;
            }
        }
    }
    return true;
}

/* Geeft een pointer naar het schaakstuk dat op rij r, kolom k staat
 * Als er geen schaakstuk staat op deze positie, geef nullptr terug
 */
ChessPiece* Game::getPiece(int r, int k) const {
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

std::pair<int, int> Game::getPosition(Piece::Type type, bw kleur) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++){
            ChessPiece* p = board[i][j];
            if (p != nullptr && p->piece().type() == type && p->getColor() == kleur) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}
