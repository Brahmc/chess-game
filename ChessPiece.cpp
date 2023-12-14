//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include "ChessPiece.h"
#include "Game.h"

std::vector<std::tuple<int, int>> addVerticalAndHorizontalMoves(ChessPiece* p, int r, int k, Game &g) {
    std::vector<std::tuple<int, int>> moves;
    for (int n = 0; n < 4; n++ ) {
        int start = n > 2 ? r : k;
        int dir = n > 2 ? 1 : -1;

        for (int i = start; i < 8; i += dir) {
            ChessPiece* piece = g.getPiece(i, k);

            if (piece == nullptr) {
                moves.emplace_back(i, k);
            } else {
                if (piece->getKleur() != p->getKleur()) {
                    moves.emplace_back(i, k);
                }
                break;
            }
        }
    }
}

std::vector<std::pair<int, int>> Rook::getMoves(int r, int k, Game &g) {
    std::vector<std::pair<int, int>> moves;
    addVerticalAndHorizontalMoves(this, r, k, g);
    return moves;
}

std::vector<std::pair<int, int>> Knight::getMoves(int r, int k, Game &g) {

}

std::vector<std::pair<int, int>> Bishop::getMoves(int r, int k, Game &g) {

}

std::vector<std::pair<int, int>> Queen::getMoves(int r, int k, Game &g) {

}

std::vector<std::pair<int, int>> King::getMoves(int r, int k, Game &g) {

}

std::vector<std::pair<int, int>> Pawn::getMoves(int r, int k, Game &g) {

}



