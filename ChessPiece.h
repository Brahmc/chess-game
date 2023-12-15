//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <guicode/chessboard.h>

class Game;

enum zw{black,white};

class ChessPiece {
public:
    ChessPiece(zw color): color(color) {}

    virtual Piece piece() const=0;
    virtual std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) = 0;
    virtual std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g);
    virtual void triggerMoveEvent(int r, int k, int newR, int newK, Game &g);

    [[nodiscard]] zw getColor() const { return color; }
private:
    zw color;

};

class Pawn: public ChessPiece {
public:
    Pawn(zw color): ChessPiece(color) {}
    virtual Piece piece() const override {
        return Piece(Piece::Pawn, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

private:
    bool enPassantCapturable = false;
};

class Rook: public ChessPiece {
public:
    Rook(zw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Rook, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

private:
    bool hasMoved = false;
};

class Knight: public ChessPiece {
public:
    Knight(zw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Knight, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class Bishop: public ChessPiece {
public:
    Bishop(zw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Bishop, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class King: public ChessPiece {
public:
    King(zw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::King, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

private:
    bool hasMoved = false;
};

class Queen: public ChessPiece {
public:
    Queen(zw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Queen, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

#endif //SCHAKEN_SCHAAKSTUK_H
