//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <guicode/chessboard.h>

class Game;

enum bw{black,white};

class ChessPiece {
public:
    ChessPiece(bw color): color(color) {}

    virtual Piece piece() const=0;
    virtual std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) = 0;
    virtual std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g);
    virtual void triggerMoveEvent(int r, int k, int newR, int newK, Game &g);

    [[nodiscard]] bw getColor() const { return color; }
private:
    bw color;

};

class Pawn: public ChessPiece {
public:
    Pawn(bw color): ChessPiece(color) {}
    virtual Piece piece() const override {
        return Piece(Piece::Pawn, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

};

class Rook: public ChessPiece {
public:
    Rook(bw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Rook, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

    [[nodiscard]] bool hasMoved() const { return moved; }

private:
    bool moved = false;
};

class Knight: public ChessPiece {
public:
    Knight(bw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Knight, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class Bishop: public ChessPiece {
public:
    Bishop(bw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Bishop, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class King: public ChessPiece {
public:
    King(bw color): ChessPiece(color) {}

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
    Queen(bw color): ChessPiece(color) {}

    Piece piece() const override {
        return Piece(Piece::Queen, getColor() == white ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

#endif //SCHAKEN_SCHAAKSTUK_H
