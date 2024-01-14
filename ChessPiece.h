//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef CHESS_CHESSPIECE_H
#define CHESS_CHESSPIECE_H
#include <guicode/chessboard.h>

class Game;

enum bw{black,white};

class ChessPiece {
public:
    explicit ChessPiece(bw color): color(color) {}

    [[nodiscard]] virtual Piece piece() const=0;
    virtual std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) = 0;
    virtual std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g);
    virtual ChessPiece * move(int r, int k, int newR, int newK, Game &g);

    virtual ~ChessPiece();

    [[nodiscard]] bw getColor() const { return color; }
private:
    bw color;

};

class Pawn: public ChessPiece {
public:
    explicit Pawn(bw color): ChessPiece(color) {}
    [[nodiscard]] Piece piece() const override {
        return {Piece::Pawn, getColor() == white ? Piece::White : Piece::Black};
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    ChessPiece * move(int r, int k, int newR, int newK, Game &g) override;

};

class Rook: public ChessPiece {
public:
    explicit Rook(bw color): ChessPiece(color) {}

    [[nodiscard]] Piece piece() const override {
        return {Piece::Rook, getColor() == white ? Piece::White : Piece::Black};
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    ChessPiece * move(int r, int k, int newR, int newK, Game &g) override;

    [[nodiscard]] bool hasMoved() const { return moved; }

private:
    bool moved = false;
};

class Knight: public ChessPiece {
public:
    explicit Knight(bw color): ChessPiece(color) {}

    [[nodiscard]] Piece piece() const override {
        return {Piece::Knight, getColor() == white ? Piece::White : Piece::Black};
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class Bishop: public ChessPiece {
public:
    explicit Bishop(bw color): ChessPiece(color) {}

    [[nodiscard]] Piece piece() const override {
        return {Piece::Bishop, getColor() == white ? Piece::White : Piece::Black};
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class King: public ChessPiece {
public:
    explicit King(bw color): ChessPiece(color) {}

    [[nodiscard]] Piece piece() const override {
        return {Piece::King, getColor() == white ? Piece::White : Piece::Black};
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g) override;

    ChessPiece * move(int r, int k, int newR, int newK, Game &g) override;

private:
    bool hasMoved = false;
};

class Queen: public ChessPiece {
public:
    explicit Queen(bw color): ChessPiece(color) {}

    [[nodiscard]] Piece piece() const override {
        return {Piece::Queen, getColor() == white ? Piece::White : Piece::Black};
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

#endif //CHESS_CHESSPIECE_H
