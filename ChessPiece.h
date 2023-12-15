//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <guicode/chessboard.h>

class Game;

enum zw{zwart,wit};

class ChessPiece {
public:
    ChessPiece(zw kleur): kleur(kleur) {}

    virtual Piece piece() const=0;
    virtual std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) = 0;
    virtual std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g);
    virtual void triggerMoveEvent(int r, int k, int newR, int newK, Game &g);

    zw getKleur() const { return kleur; }
private:
    zw kleur;

};

class Pawn: public ChessPiece {
public:
    Pawn(zw kleur): ChessPiece(kleur) {}
    virtual Piece piece() const override {
        return Piece(Piece::Pawn, getKleur() == wit ? Piece::White : Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

private:
    bool enPassantCapturable = false;
};

class Rook: public ChessPiece {
public:
    Rook(zw kleur): ChessPiece(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Rook,getKleur()==wit?Piece::White:Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

private:
    bool hasMoved = false;
};

class Knight: public ChessPiece {
public:
    Knight(zw kleur): ChessPiece(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Knight,getKleur()==wit?Piece::White:Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class Bishop: public ChessPiece {
public:
    Bishop(zw kleur): ChessPiece(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

class King: public ChessPiece {
public:
    King(zw kleur): ChessPiece(kleur) {}

    Piece piece() const override {
        return Piece(Piece::King,getKleur()==wit?Piece::White:Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;

    std::vector<std::pair<int, int>> getAllowedMoves(int r, int k, Game &g) override;

    void triggerMoveEvent(int r, int k, int newR, int newK, Game &g) override;

private:
    bool hasMoved = false;
};

class Queen: public ChessPiece {
public:
    Queen(zw kleur): ChessPiece(kleur) {}

    Piece piece() const override {
        return Piece(Piece::Queen,getKleur()==wit?Piece::White:Piece::Black);
    }
    std::vector<std::pair<int, int>> getMoves(int r, int k, const Game &g) override;
};

#endif //SCHAKEN_SCHAAKSTUK_H
