//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <unordered_set>
#include "ChessPiece.h"
#include "Game.h"

bool isInBounds(int r, int k) {
    return r >= 0 && r < 8 && k >= 0 && k < 8;
}

void addOrthogonalMoves(std::vector<std::pair<int, int>> &moves, ChessPiece* p, int r, int k, const Game &g, bool captureOnly = false) {
    for (int n = 0; n < 4; n++ ) {
        int start = n < 2 ? r : k;
        int dir = n % 2 == 0 ? 1 : -1;

        for (int i = start + dir; i < 8 && i >= 0; i += dir) {
            int newR = n < 2 ? i : r;
            int newK = n < 2 ? k : i;

            if (!isInBounds(newR, newK)) break;

            ChessPiece* piece = g.getPiece(newR, newK);

            if (piece == nullptr && !captureOnly) {
                moves.emplace_back(newR, newK);
            } else if (piece != nullptr) {
                if (piece->getKleur() != p->getKleur()) {
                    moves.emplace_back(newR, newK);
                }
                break;
            }
        }
    }
}

void addDiagonalMoves(std::vector<std::pair<int, int>> &moves, ChessPiece* p, int r, int k, const Game &g, bool captureOnly = false) {
    for (int n = 0; n < 4; n++ ) {
        int dirR = n < 2 ? 1 : -1;
        int dirK = n % 2 == 0 ? 1 : -1;

        for (int i = 1; i < 8; i++) {
            int newR = r + i * dirR;
            int newK = k + i * dirK;

            if (!isInBounds(newR, newK)) break;

            ChessPiece* piece = g.getPiece(newR, newK);
            if (piece == nullptr && !captureOnly) {
                moves.emplace_back(newR, newK);
            } else if (piece != nullptr) {
                if (piece->getKleur() != p->getKleur()) {
                    moves.emplace_back(newR, newK);
                }
                break;
            }
        }
    }
}

void removeDiscoveredCheckMoves(std::vector<std::pair<int, int>> &moves, ChessPiece* p, int r, int k, Game &g) {
    auto kingPos = g.getPosition(Piece::King, p->getKleur());

    std::vector<std::pair<int, int>> discoveredCheckPieceOrth;
    std::vector<std::pair<int, int>> discoveredCheckPieceDia;

    g.setPiece(r, k, nullptr);
    addOrthogonalMoves(discoveredCheckPieceOrth, p, kingPos.first, kingPos.second, g, true);
    addDiagonalMoves(discoveredCheckPieceDia, p, kingPos.first, kingPos.second, g, true);

    for (auto it = discoveredCheckPieceOrth.begin(); it != discoveredCheckPieceOrth.end();) {
        auto pt = g.getPiece(it->first, it->second)->piece().type();
        if (pt != Piece::Queen && pt != Piece::Rook) {
            it = discoveredCheckPieceOrth.erase(it);
        } else it++;
    }
    for (auto it = discoveredCheckPieceDia.begin(); it != discoveredCheckPieceDia.end();) {
        auto piece = g.getPiece(it->first, it->second)->piece().type();
        if (piece != Piece::Queen && piece != Piece::Bishop) {
            it = discoveredCheckPieceDia.erase(it);
        } else it++;
    }
    g.setPiece(r, k , p);

    size_t amount = discoveredCheckPieceOrth.size() + discoveredCheckPieceDia.size();
    if (amount > 1) { // King is in check by 2 pieces
        moves.clear();
        return;
    }

    // Remove moves that don't block the discovered check piece
    if (discoveredCheckPieceDia.size() == 1) {
        auto threatPos = discoveredCheckPieceDia[0];
        for (auto it = moves.begin(); it != moves.end();) {
            if (threatPos.first - it->first != threatPos.second - it->second || // Move is not on the same diagonal
                ((threatPos.first < kingPos.first) != (it->first < kingPos.first) || (threatPos.second < kingPos.second) != (it->second < kingPos.second) ) ) { // Move is not in the same direction
                it = moves.erase(it);
            } else it++;
        }
    } else {
        auto threatPos = discoveredCheckPieceOrth[0];
        for (auto it = moves.begin(); it != moves.end(); ) {
            if ((threatPos.first == kingPos.first) != (it->first == kingPos.first) || (threatPos.second == kingPos.second) != (it->second == kingPos.second) // Move is not on the same orthogonal
                || ((threatPos.first < kingPos.first) != (it->first < kingPos.first) || (threatPos.second < kingPos.second) != (it->second < kingPos.second) )) {  // Move is not in the same direction
                it = moves.erase(it);
            } else it++;
        }
    }
}

std::vector<std::pair<int, int>> Rook::getMoves(int r, int k, const Game &g) {
    std::vector<std::pair<int, int>> moves;
    addOrthogonalMoves(moves, this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> Knight::getMoves(int r, int k, const Game &g) {
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

std::vector<std::pair<int, int>> Bishop::getMoves(int r, int k, const Game &g) {
    std::vector<std::pair<int, int>> moves;
    addDiagonalMoves(moves, this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> Queen::getMoves(int r, int k, const Game &g) {
    std::vector<std::pair<int, int>> moves;
    addOrthogonalMoves(moves, this, r, k, g);
    addDiagonalMoves(moves, this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> King::getMoves(int r, int k, const Game &g) {
    std::vector<std::pair<int, int>> moves;
    for (int i = r - 1; i < r + 2; i++) {
        for (int j = k - 1; j < k + 2; j++) {
        if (i == r && j == k || !isInBounds(i, j)) continue;
        ChessPiece* piece = g.getPiece(i, j);
            if (piece == nullptr || piece->getKleur() != getKleur()) {
                moves.emplace_back(i, j);
            }
        }
    }
    return moves;
}

std::vector<std::pair<int, int>> Pawn::getMoves(int r, int k, const Game &g) {
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

std::vector<std::pair<int, int>> King::getAllowedMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves = getMoves(r, k, g);

    // could be optimized
    auto it = moves.begin();
    while (it < moves.end()) {
        int newR = it->first;
        int newK = it->second;
        auto replaced = g.getPiece(newR, newK);
        g.setPiece(newR, newK, this);
        g.setPiece(r, k, nullptr);
        if (g.inCheck(getKleur())) {
            it = moves.erase(it);
        } else it++;

        g.setPiece(newR, newK, replaced);
        g.setPiece(r, k, this);
    }
    return moves;
}


std::vector<std::pair<int, int>> ChessPiece::getAllowedMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves = getMoves(r, k, g);
    removeDiscoveredCheckMoves(moves, this, r, k, g);
    return moves;
}
