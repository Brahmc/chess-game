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
                if (piece->getColor() != p->getColor()) {
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
                if (piece->getColor() != p->getColor()) {
                    moves.emplace_back(newR, newK);
                }
                break;
            }
        }
    }
}

void removeDiscoveredCheckMoves(std::vector<std::pair<int, int>> &moves, ChessPiece* p, int r, int k, Game &g) {
    auto kingPos = g.getPosition(Piece::King, p->getColor());

    std::vector<std::pair<int, int>> discoveredCheckPieceOrth;
    std::vector<std::pair<int, int>> discoveredCheckPieceDia;

    g.setPiece(r, k, nullptr);
    addOrthogonalMoves(discoveredCheckPieceOrth, p, kingPos.first, kingPos.second, g, true);
    addDiagonalMoves(discoveredCheckPieceDia, p, kingPos.first, kingPos.second, g, true);
    g.setPiece(r, k , p);

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
    } else if (discoveredCheckPieceOrth.size() == 1) {
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
            if (piece == nullptr || piece->getColor() != getColor()) {
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
            if (piece == nullptr || piece->getColor() != getColor()) {
                moves.emplace_back(i, j);
            }
        }
    }
    return moves;
}

std::vector<std::pair<int, int>> Pawn::getMoves(int r, int k, const Game &g) {
    std::vector<std::pair<int, int>> moves;
    int dir = getColor() == white ? -1 : 1;
    if (!isInBounds(r + dir, k)) return moves;

    if (g.getPiece(r + dir, k) == nullptr) {
        moves.emplace_back(r + dir, k);

        if ((r == 6 || r == 1) && isInBounds(r + 2 * dir, k)) {
            ChessPiece *p2 = g.getPiece(r + 2 * dir, k);
            if (p2 == nullptr) moves.emplace_back(r + 2 * dir, k);
        }
    }

    if (k < 7) {
        ChessPiece* p = g.getPiece(r+ dir, k + 1);
        if (p != nullptr && p->getColor() != getColor()) {
            moves.emplace_back(r+ dir, k + 1);
        }
    }

    if (k > 0) {
        ChessPiece* p = g.getPiece(r+ dir, k - 1);
        if (p != nullptr && p->getColor() != getColor()) {
            moves.emplace_back(r + dir, k - 1);
        }
    }

    for (int i = -1; i <= 1; i += 2) {
        Pawn* pawn = dynamic_cast<Pawn*>(g.getPiece(r, k + i));
        if (pawn != nullptr && pawn->getColor() != getColor() && pawn->enPassantCapturable) {
            moves.emplace_back(r + dir, k + i);
        }
    }

    return moves;
}

std::vector<std::pair<int, int>> King::getAllowedMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves = getMoves(r, k, g);

    auto it = moves.begin();
    while (it < moves.end()) {
        int newR = it->first;
        int newK = it->second;
        auto replaced = g.getPiece(newR, newK);
        g.setPiece(newR, newK, this);
        g.setPiece(r, k, nullptr);
        if (g.inCheck(getColor())) {
            it = moves.erase(it);
        } else it++;

        g.setPiece(newR, newK, replaced);
        g.setPiece(r, k, this);
    }

    if (hasMoved) return moves;

    for (int n = 0; n < 2; n++){
        Rook* rook = dynamic_cast<Rook*>(g.getPiece(r, n ? 0 : 7));
        if (rook == nullptr) continue;

        if (rook->hasMoved()) continue;
        bool canCastle = true;
        for (int i = (n ? 1 : 5); i < (n ? 4 : 7); i++) {
            if (g.getPiece(r, i) != nullptr) {
                canCastle = false;
                break;
            }
        }
        if (canCastle) {
            moves.emplace_back(r, n ? 2 : 6);
        }
    }

    return moves;
}


std::vector<std::pair<int, int>> ChessPiece::getAllowedMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves = getMoves(r, k, g);
    removeDiscoveredCheckMoves(moves, this, r, k, g);
    return moves;
}

void ChessPiece::triggerMoveEvent(int r, int k, int newR, int newK, Game &g) {}

void Pawn::triggerMoveEvent(int r, int k, int newR, int newK, Game &g) {
    enPassantCapturable = std::abs(r - newR) == 2;

    if (newR == 0 || newR == 7) {
        g.setWaitingForPromotion(std::optional<std::pair<int, int>>(std::make_pair(newR, newK)));
    };

    if (newR == r) return;
    for (int i = -1; i <= 1; i += 2) {
        Pawn* pawn = dynamic_cast<Pawn*>(g.getPiece(r, k + i));
        if (pawn != nullptr && pawn->getColor() != getColor() && pawn->enPassantCapturable) {
            g.setPiece(r, k + i, nullptr);
        }
    }
}

void Rook::triggerMoveEvent(int r, int k, int newR, int newK, Game &g) {
    moved = true;
}

void King::triggerMoveEvent(int r, int k, int newR, int newK, Game &g) {
    hasMoved = true;

    if (std::abs(newK - k) == 2) { // castle
        int rookK = k - newK > 0 ? 0 : 7;
        ChessPiece* rook = g.getPiece(r, rookK);
        g.setPiece(r, rookK, nullptr);
        g.setPiece(r, k - newK > 0 ? 3 : 5, rook);
    }
}