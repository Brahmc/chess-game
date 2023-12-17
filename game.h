//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "ChessPiece.h"

struct Move {
    std::pair<int, int> from;
    std::pair<int, int> to;

    ChessPiece* captured;
};

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(ChessPiece* s, int r, int k); // Verplaats stuk s naar rij r en kolom k

    [[nodiscard]] bool inCheck(bw color) const;
    bool checkMate(bw color);
    bool staleMate(bw color);
    void setStartBord();

    [[nodiscard]] ChessPiece* getPiece(int r, int k) const;

    void setPiece(int r, int k, ChessPiece* s);


    std::pair<int, int> getPosition(ChessPiece *piece);

    [[nodiscard]] const std::optional<std::pair<int, int>> &getPawnWaitingForPromotion() const;

    void setPawnWaitingForPromotion(const std::optional<std::pair<int, int>> &pawnWaitingForPromotion);

    bool isWaitingForPromotion();

    std::vector<ChessPiece *> getPromotionPieces();

    bool promotePawn(ChessPiece *piece);

    [[nodiscard]] bw getTurn() const;

    void setTurn(bw turn);

    [[nodiscard]] std::vector<std::pair<int, int>> getPositionsUnderThreat(bw color);

    [[nodiscard]] Pawn *getEnPassantPawn() const;

    void setEnPassantPawn(Pawn *enPassantPawn);

    bool undo();

    bool redo();
private:
    ChessPiece* board[8][8];

    Pawn* enPassantPawn = nullptr;

    std::vector<Move*> history;
    int historyIndex = -1;

    std::optional<std::pair<int, int>> pawnWaitingForPromotion;

    std::vector<ChessPiece*> promotionPieces;

    void clearBoard();

    bool noValidMoves(bw color);

    bw turn = white;

    [[nodiscard]] std::pair<int, int> getPosition(Piece::Type type, bw color) const;
};

#endif //CHESS_GAME_H


