//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#include <unordered_set>
#include "game.h"

Game::Game() {
    clearBoard();
}

Game::~Game() {
    for (const auto &h: history) {
        delete h->captured;
        delete h;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
        }
    }
}

// Zet het bord klaar; voeg de stukken op de jusite plaats toe
void Game::setStartBord() {

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
    if (isWaitingForPromotion() || turn != s->getColor()) return false;

    auto pos = getPosition(s);

    std::vector<std::pair<int, int>> moves = s->getAllowedMoves(pos.first,pos.second, *this);
    if (std::find(moves.begin(), moves.end(), std::make_pair(r, k)) == moves.end()) return false;

    ChessPiece* captured = s->move(pos.first, pos.second, r, k, *this);

    if (history.size() != historyIndex + 1) {
        for (size_t i = history.size() - 1; i > historyIndex; i--) {
            delete history[i];
            history.pop_back();
        }
    }
    history.emplace_back(new Move{pos, std::make_pair(r,k), captured});
    historyIndex++;


    turn = turn == white ? black : white;
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
            if (auto *p = board[i][j]; p != nullptr && p->getColor() == kleur && !p->getAllowedMoves(i, j, *this).empty()) {
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

std::pair<int, int> Game::getPosition(ChessPiece* piece) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8 ; j++) {
            if (board[i][j] == piece) {
                return std::make_pair(i, j);
            }
        }
    }
    return std::make_pair(-1, -1);
}

const std::optional<std::pair<int, int>> &Game::getPawnWaitingForPromotion() const {
    return pawnWaitingForPromotion;
}

void Game::setPawnWaitingForPromotion(const std::optional<std::pair<int, int>> &pawnWaitingForPromotion) {
    Game::pawnWaitingForPromotion = pawnWaitingForPromotion;
}

bool Game::isWaitingForPromotion() {
    return pawnWaitingForPromotion.has_value();
}

std::vector<ChessPiece*> Game::getPromotionPieces() {
    if (!promotionPieces.empty()) return promotionPieces;
    std::vector<ChessPiece*> pieces;
    ChessPiece* promotionPiece = board[pawnWaitingForPromotion->first][pawnWaitingForPromotion->second];
    if (promotionPiece == nullptr) return pieces;
    bw color = promotionPiece->getColor();
    pieces.push_back(new Queen(color));
    pieces.push_back(new Knight(color));
    pieces.push_back(new Rook(color));
    pieces.push_back(new Bishop(color));

    promotionPieces = pieces;
    return pieces;
}

bool Game::promotePawn(ChessPiece* piece) {
    if (!isWaitingForPromotion()) return false;
    if (std::find(promotionPieces.begin(), promotionPieces.end(), piece) == promotionPieces.end()) return false;

    delete getPiece(pawnWaitingForPromotion->first, pawnWaitingForPromotion->second);
    setPiece(pawnWaitingForPromotion->first, pawnWaitingForPromotion->second, piece);
    for (const auto &item: promotionPieces) {
        if (item != piece) delete item;
    }
    pawnWaitingForPromotion = std::nullopt;
    promotionPieces.clear();

    return true;
}

std::vector<std::pair<int, int>> Game::getPositionsUnderThreat(bw color) {
    std::unordered_map<ChessPiece*, std::pair<int, int>> positionsUnderThreat;
    for (int r = 0; r < 8; r++) {
        for (int k = 0; k < 8; k++) {
            ChessPiece* p = getPiece(r, k);
            if (p == nullptr || p->getColor() == color) continue;
            for (const auto &move: p->getAllowedMoves(r, k, *this)) {
                ChessPiece* underThread = getPiece(move.first, move.second);
                if (underThread != nullptr)
                    positionsUnderThreat[underThread] = std::make_pair(move.first, move.second);
            }
        }
    }
    std::vector<std::pair<int, int>> positions;
    for (const auto &p : positionsUnderThreat)
        positions.push_back(p.second);
    return positions;
}

bool Game::undo() {
    if (historyIndex < 0) return false;
    Move* m = history[historyIndex--];

    ChessPiece* moved = getPiece(m->to.first, m->to.second);
    setPiece(m->from.first, m->from.second, moved);
    setPiece(m->to.first, m->to.second, m->captured);
    turn = turn == white ? black : white;
    return true;
}

bool Game::redo() {
    if (historyIndex >= (int)history.size() - 1) return false;
    Move* m = history[++historyIndex];

    ChessPiece* moved = getPiece(m->from.first, m->from.second);
    setPiece(m->to.first, m->to.second, moved);
    setPiece(m->from.first, m->from.second, nullptr);
    turn = turn == white ? black : white;
    return true;
}

bw Game::getTurn() const {
    return turn;
}

Pawn *Game::getEnPassantPawn() const {
    return enPassantPawn;
}

void Game::setEnPassantPawn(Pawn *enPassantPawn) {
    Game::enPassantPawn = enPassantPawn;
}

void Game::setTurn(bw turn) {
    Game::turn = turn;
}
