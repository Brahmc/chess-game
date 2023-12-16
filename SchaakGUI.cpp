//
// Created by toonc on 12/17/2021.
//

#include "SchaakGUI.h"
#include "guicode/message.h"
#include "guicode/fileIO.h"

// Constructor
SchaakGUI::SchaakGUI():ChessWindow(nullptr) {
    newGame();
}

// Update de inhoud van de grafische weergave van het schaakbord (scene)
// en maak het consistent met de game state in variabele g.
void SchaakGUI::update() {
    clearBoard();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8 ; j++) {
            ChessPiece* piece = g.getPiece(i, j);
            if (piece != nullptr) {
                setItem(i, j, piece);
            }
        }
    }
}


// Deze functie wordt opgeroepen telkens er op het schaakbord
// geklikt wordt. x,y geeft de positie aan waar er geklikt
// werd; r is de 0-based rij, k de 0-based kolom
void SchaakGUI::clicked(int r, int k) {
    if (g.isWaitingForPromotion()) {
        bool success = handlePromotionSelected(r, k);
        if (success && g.inCheck(g.getTurn())) {
            message("Check!");
        }
        return;
    }
    removeAllTileFocus();


    ChessPiece* clickedPiece = g.getPiece(r, k);
    if (selected.has_value() && (clickedPiece == nullptr || clickedPiece->getColor() != g.getTurn())) {
        ChessPiece* selectedPiece = g.getPiece(selected->first, selected->second);
        if (g.move(selectedPiece, r, k)) {
            update();
            if (g.isWaitingForPromotion()) {
                drawPromotionSelection();
            } else {
                updateThreads();
                if (g.inCheck(g.getTurn())) {
                    if (g.checkMate(g.getTurn())) {
                        message("Checkmate! " + QString((g.getTurn() == white ? "Black" : "White")) + " wins!");
                    } else {
                        message("Check!");
                    }
                } else if (g.staleMate(g.getTurn())) {
                    message("Stalemate!");
                }
            }
        }
        selected = std::nullopt;
        return;
    }

    if (clickedPiece == nullptr || clickedPiece->getColor() != g.getTurn()) return;
    selected = std::make_pair(r, k);

    setTileSelect(r, k,true);
    if (!displayMoves()) return;

    std::vector<std::pair<int, int>> moves = clickedPiece->getAllowedMoves(r, k, g);
    for (auto move : moves) {
        setTileFocus(move.first, move.second, true);
    }

}

void SchaakGUI::drawPromotionSelection() {
    std::pair<int,int> promotionPos = g.getWaitingForPromotion().value();
    auto promotionPieces = g.getPromotionPieces();
    int dir = promotionPos.first == 0 ? 1 : -1;
    for (int i = 0; i < promotionPieces.size(); i++) {
        setItem(promotionPos.first + i * (dir), promotionPos.second, promotionPieces[i]);
        setTileFocus(promotionPos.first + i * (dir), promotionPos.second, true);
    }
}

bool SchaakGUI::handlePromotionSelected(int r, int k) {
    std::pair<int,int> promotionPos = g.getWaitingForPromotion().value();
    int dir = promotionPos.first == 0 ? 1 : -1;

    auto promotionPieces = g.getPromotionPieces();
    if (r > (promotionPos.first + (int)promotionPieces.size() * dir - 1) || k != promotionPos.second) return false;
    ChessPiece* piece = promotionPieces[promotionPos.first + r * dir];
    g.promotePawn(piece);

    removeAllMarking();
    update();
    return true;
}

void SchaakGUI::updateThreads() {
    removeAllPieceThreats();
    std::vector<std::pair<int, int>> positions;

    if (displayThreats()) {
        auto threads = g.getPositionsUnderThreat(g.getTurn());
        positions.insert(positions.end(), threads.begin(), threads.end());
    }

    if (displayKills()) {
        auto kills = g.getPositionsUnderThreat(g.getTurn() == white ? black : white);
        positions.insert(positions.end(), kills.begin(), kills.end());
    }

    for (const auto &pos : positions) {
        setPieceThreat(pos.first, pos.second, true);
    }
}

void SchaakGUI::newGame()
{
    g = Game();
    g.setStartBord();
    removeAllMarking();
    update();
}


// TODO: save state of En passant pawn, castling
void SchaakGUI::save() {
    QFile file;
    if (openFileToWrite(file)) {
        QDataStream out(&file);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8 ; j++) {
                ChessPiece* piece = g.getPiece(i, j);
                QString ps = pieceToString(piece);
                out << ps;
            }
        }
        out << QString(g.getTurn() == white ? "w" : "b");
    }
}

QString SchaakGUI::pieceToString(ChessPiece* piece) {
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

void SchaakGUI::open() {
    QFile file;
    Game newGame = Game();
    if (openFileToRead(file)) {
        try {
            QDataStream in(&file);
            for (int r=0;r<8;r++) {
                for (int k=0;k<8;k++) {
                    QString piece;
                    in >> piece;
                    ChessPiece* p = stringToPiece(piece);
                    newGame.setPiece(r, k , p);
                    if (in.status()!=QDataStream::Ok) {
                        throw QString("Ongeldig formaat");
                    }
                }
            }
            QString turn;
            in >> turn;
            newGame.setTurn(turn == "w" ? white : black);

            message("Game loaded");
            g = newGame;
            update();
            updateThreads();
        } catch (QString& Q) {
            message(Q);
        }
    }
}


void SchaakGUI::undo() {
    message("UNDO");
}

void SchaakGUI::redo() {
    message("REDO");
}


void SchaakGUI::visualizationChange() {
    QString visstring = QString(displayMoves()?"T":"F")+(displayKills()?"T":"F")+(displayThreats()?"T":"F");
    updateThreads();
    message(QString("Nieuwe settings : ")+visstring);
}










