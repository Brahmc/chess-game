//
// Created by toonc on 12/17/2021.
//

#include "ChessGUI.h"
#include "guicode/message.h"
#include "guicode/fileIO.h"

// Constructor
ChessGUI::ChessGUI(): ChessWindow(nullptr) {
    ChessGUI::newGame();
}

// Update de inhoud van de grafische weergave van het schaakbord (scene)
// en maak het consistent met de game state in variabele g.
void ChessGUI::update() {
    clearBoard();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8 ; j++) {
            ChessPiece* piece = g->getPiece(i, j);
            if (piece != nullptr) {
                setItem(i, j, piece);
            }
        }
    }
}


// Deze functie wordt opgeroepen telkens er op het schaakbord
// geklikt wordt. x,y geeft de positie aan waar er geklikt
// werd; r is de 0-based rij, k de 0-based kolom
void ChessGUI::clicked(int r, int k) {
    if (g->isWaitingForPromotion()) {
        bool success = handlePromotionSelected(r, k);
        if (success) {
            displayStatusMessage();
        }
        return;
    }
    removeAllTileFocus();

    ChessPiece* clickedPiece = g->getPiece(r, k);
    if (selected.has_value() && (clickedPiece == nullptr || clickedPiece->getColor() != g->getTurn())) {
        ChessPiece* selectedPiece = g->getPiece(selected->first, selected->second);
        if (g->move(selectedPiece, r, k)) {
            update();
            if (g->isWaitingForPromotion()) {
                drawPromotionSelection();
            } else {
                updateThreads();
                displayStatusMessage();
            }
        }
        selected = std::nullopt;
        return;
    }

    if (clickedPiece == nullptr || clickedPiece->getColor() != g->getTurn()) return;
    selected = std::make_pair(r, k);

    setTileSelect(r, k,true);
    if (!displayMoves()) return;

    std::vector<std::pair<int, int>> moves = clickedPiece->getAllowedMoves(r, k, *g);
    for (auto move : moves) {
        setTileFocus(move.first, move.second, true);
    }

}

void ChessGUI::displayStatusMessage() {
    if (g->inCheck(g->getTurn())) {
        if (g->checkMate(g->getTurn())) {
            message("Checkmate! " + QString((g->getTurn() == white ? "Black" : "White")) + " wins!");
        } else {
            message("Check!");
        }
    } else if (g->staleMate(g->getTurn())) {
        message("Stalemate!");
    }
}

void ChessGUI::drawPromotionSelection() {
    std::pair<int,int> promotionPos = g->getPawnWaitingForPromotion().value();
    auto promotionPieces = g->getPromotionPieces();
    int dir = promotionPos.first == 0 ? 1 : -1;
    for (int i = 0; i < promotionPieces.size(); i++) {
        setItem(promotionPos.first + i * (dir), promotionPos.second, promotionPieces[i]);
        setTileFocus(promotionPos.first + i * (dir), promotionPos.second, true);
    }
}

bool ChessGUI::handlePromotionSelected(int r, int k) {
    std::pair<int,int> promotionPos = g->getPawnWaitingForPromotion().value();
    int dir = promotionPos.first == 0 ? 1 : -1;

    auto promotionPieces = g->getPromotionPieces();
    if (r > (promotionPos.first + (int)promotionPieces.size() * dir - 1) || k != promotionPos.second) return false;
    ChessPiece* piece = promotionPieces[promotionPos.first + r * dir];
    g->promotePawn(piece);

    removeAllMarking();
    update();
    return true;
}

void ChessGUI::updateThreads() {
    removeAllPieceThreats();
    std::vector<std::pair<int, int>> positions;

    if (displayThreats()) {
        auto threads = g->getPositionsUnderThreat(g->getTurn());
        positions.insert(positions.end(), threads.begin(), threads.end());
    }

    if (displayKills()) {
        auto kills = g->getPositionsUnderThreat(g->getTurn() == white ? black : white);
        positions.insert(positions.end(), kills.begin(), kills.end());
    }

    for (const auto &pos : positions) {
        setPieceThreat(pos.first, pos.second, true);
    }
}

void ChessGUI::newGame()
{
    delete g;
    g = new Game;
    g->setStartBord();
    removeAllMarking();
    update();
}


// TODO: save state of En passant pawn, castling
void ChessGUI::save() {
    QFile file;
    if (!openFileToWrite(file)) return;

    QDataStream out(&file);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8 ; j++) {
            ChessPiece* piece = g->getPiece(i, j);
            out << pieceToString(piece);
        }
    }
    out << QString(g->getTurn() == white ? "w" : "b");
}

QString ChessGUI::pieceToString(ChessPiece* piece) {
    if (piece == nullptr) return ".";
    QString pieceString = "";
    switch (piece->piece().type()) {
        case Piece::Pawn:
            pieceString += "P";
            break;
        case Piece::Rook:
            pieceString += "R";
            break;
        case Piece::Knight:
            pieceString += "H";
            break;
        case Piece::Bishop:
            pieceString += "B";
            break;
        case Piece::Queen:
            pieceString += "Q";
            break;
        case Piece::King:
            pieceString += "K";
            break;
        case Piece::None:
            return ".";
    }
    return pieceString + QString(piece->getColor() == white ? "w" : "b");
}

ChessPiece* stringToPiece(QString pieceString) {
    if (pieceString == ".") return nullptr;
    bw color = pieceString[1] == 'w' ? white : black;
    switch (pieceString[0].toLatin1()) {
        case 'P':
            return new Pawn(color);
        case 'R':
            return new Rook(color);
        case 'H':
            return new Knight(color);
        case 'B':
            return new Bishop(color);
        case 'Q':
            return new Queen(color);
        case 'K':
            return new King(color);
        default:
            throw QString("Error parsing file");
    }
}

void ChessGUI::open() {
    QFile file;
    Game* newGame = new Game;
    if (!openFileToRead(file)) return;

    try {
        QDataStream in(&file);
        for (int r=0;r<8;r++) {
            for (int k=0;k<8;k++) {
                QString piece;
                in >> piece;
                ChessPiece* p = stringToPiece(piece);
                newGame->setPiece(r, k , p);
                if (in.status()!=QDataStream::Ok) {
                    throw QString("Ongeldig formaat");
                }
            }
        }
        QString turn;
        in >> turn;
        newGame->setTurn(turn == "w" ? white : black);

        message("Game loaded");
        delete g;
        g = newGame;
        update();
        updateThreads();
    } catch (QString& Q) {
        message(Q);
    }
}


void ChessGUI::undo() {
    bool success = g->undo();
    message(success ? "Undo" : "Cannot undo any further");
    update();
    removeAllMarking();
    updateThreads();
}

void ChessGUI::redo() {
    bool success = g->redo();
    message(success ? "Redo" : "Cannot redo any further");
    update();
    removeAllMarking();
    updateThreads();
}


void ChessGUI::visualizationChange() {
    QString visstring = QString(displayMoves()?"T":"F")+(displayKills()?"T":"F")+(displayThreats()?"T":"F");
    updateThreads();
    message(QString("Nieuwe settings : ")+visstring);
}










