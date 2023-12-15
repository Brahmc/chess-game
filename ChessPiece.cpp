//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "ChessPiece.h"
#include "Game.h"

bool isInBounds(int r, int k) {
    return r >= 0 && r < 8 && k >= 0 && k < 8;
}

void addOrthogonalMoves(std::vector<std::pair<int, int>> &moves, ChessPiece* p, int r, int k, Game &g) {
    for (int n = 0; n < 4; n++ ) {
        int start = n < 2 ? r : k;
        int dir = n % 2 == 0 ? 1 : -1;

        for (int i = start + dir; i < 8 && i >= 0; i += dir) {
            int newR = n < 2 ? i : r;
            int newK = n < 2 ? k : i;

            if (!isInBounds(newR, newK)) break;

            ChessPiece* piece = g.getPiece(newR, newK);

            if (piece == nullptr) {
                moves.emplace_back(newR, newK);
            } else {
                if (piece->getKleur() != p->getKleur()) {
                    moves.emplace_back(newR, newK);
                }
                break;
            }
        }
    }
}

void addDiagonalMoves(std::vector<std::pair<int, int>> &moves, ChessPiece* p, int r, int k, Game &g) {
    for (int n = 0; n < 4; n++ ) {
        int dirR = n < 2 ? 1 : -1;
        int dirK = n % 2 == 0 ? 1 : -1;

        for (int i = 1; i < 8; i++) {
            int newR = r + i * dirR;
            int newK = k + i * dirK;

            if (!isInBounds(newR, newK)) break;

            ChessPiece* piece = g.getPiece(newR, newK);
            if (piece == nullptr) {
                moves.emplace_back(newR, newK);
            } else {
                if (piece->getKleur() != p->getKleur()) {
                    moves.emplace_back(newR, newK);
                }
                break;
            }
        }
    }
}


std::vector<std::pair<int, int>> Rook::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    addOrthogonalMoves(moves, this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> Knight::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    for (int n = 0; n < 8; n++) {
        int newR = r + (n % 4 < 2 ? 1 : -1) * (n < 4 ? 2 : 1);
        int newK = k + (n % 2 == 0 ? 1 : -1) * (n < 4 ? 1 : 2);

        if (isInBounds(newR, newK)) {
            ChessPiece* piece = g.getPiece(newR, newK);
            if (piece == nullptr || piece->getKleur() != getKleur()) {
                moves.emplace_back(newR, newK);
            }
        }
    }
    return moves;
}

std::vector<std::pair<int, int>> Bishop::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    addDiagonalMoves(moves, this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> Queen::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    addOrthogonalMoves(moves, this, r, k, g);
    addDiagonalMoves(moves, this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> King::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    for (int i = r - 1; i < r + 2; i++) {
        for (int j = k - 1; j < k + 2; j++) {
        if (i == r && j == k) continue;
        ChessPiece* piece = g.getPiece(i, j);
            if (piece == nullptr || piece->getKleur() != getKleur()) {
                moves.emplace_back(i, j);
            }
        }
    }
    return moves;
}

std::vector<std::pair<int, int>> Pawn::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    int dir = getKleur() == wit ? -1 : 1;
    int newR = r + dir;
    if (!isInBounds(newR, k)) return moves;

    if (isInBounds(newR, k)) {
        ChessPiece* piece = g.getPiece(newR, k);
        if (piece == nullptr) {
            moves.emplace_back(newR, k);
        }
    }

    if (k < 7) {
        ChessPiece* p = g.getPiece(r+ dir, k + 1);
        if (p != nullptr && p->getKleur() != getKleur()) {
            moves.emplace_back(r+ dir, k + 1);
        }
    }

    if (k > 1) {
        ChessPiece* p = g.getPiece(r+ dir, k - 1);
        if (p != nullptr && p->getKleur() != getKleur()) {
            moves.emplace_back(r + dir, k - 1);
        }
    }

    if (r != 1 && r != 6) return moves;
    newR += dir;
    if (isInBounds(newR, k)) {
        ChessPiece* piece = g.getPiece(newR, k);
        if (piece == nullptr) {
            moves.emplace_back(newR, k);
        }
    }
    return moves;
}



