//
// Created by toonc on 12/17/2021.
//

#include "SchaakGUI.h"
#include "guicode/message.h"
#include "guicode/fileIO.h"

// Constructor
SchaakGUI::SchaakGUI():ChessWindow(nullptr) {
    newGame();
    update();
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
        std::pair<int,int> promotionPos = g.getWaitingForPromotion().value();
        int dir = promotionPos.first == 0 ? 1 : -1;

        auto promotionPieces = g.getPromotionPieces();
        if (r > (promotionPos.first + (int)promotionPieces.size() * dir - 1) || k != promotionPos.second) return;
        ChessPiece* piece = promotionPieces[promotionPos.first + r * dir];
        g.promotePawn(piece);

        removeAllMarking();
        update();
        return;
    }
    removeAllMarking();

    if (selected.has_value()) {
        ChessPiece* selectedPiece = g.getPiece(selected->first, selected->second);
        if (g.move(selectedPiece, r, k)) {
            update();
            if (g.isWaitingForPromotion()) {
                drawPromotionSelection();
            }
        }
        selected = std::nullopt;
        return;
    };

    ChessPiece* piece = g.getPiece(r, k);
    if (piece == nullptr || piece->getColor() != g.getTurn()) return;
    setTileSelect(r, k,true);
    std::vector<std::pair<int, int>> moves = piece->getAllowedMoves(r, k, g);
    for (auto move : moves) {
        setTileFocus(move.first, move.second, true);
    }

    selected = std::make_pair(r, k);
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

void SchaakGUI::newGame()
{
    g.setStartBord();
    update();
}


void SchaakGUI::save() {
    QFile file;
    if (openFileToWrite(file)) {
        QDataStream out(&file);
        out << QString("Rb") << QString("Hb") << QString("Bb") << QString("Qb") << QString("Kb") << QString("Bb") << QString("Hb") << QString("Rb");
        for  (int i=0;i<8;i++) {
            out << QString("Pb");
        }
        for  (int r=3;r<7;r++) {
            for (int k=0;k<8;k++) {
                out << QString(".");
            }
        }
        for  (int i=0;i<8;i++) {
            out << QString("Pw");
        }
        out << QString("Rw") << QString("Hw") << QString("Bw") << QString("Qw") << QString("Kw") << QString("Bw") << QString("Hw") << QString("Rw");
    }
}

void SchaakGUI::open() {
    QFile file;
    if (openFileToRead(file)) {
        try {
            QDataStream in(&file);
            QString debugstring;
            for (int r=0;r<8;r++) {
                for (int k=0;k<8;k++) {
                    QString piece;
                    in >> piece;
                    debugstring += "\t" + piece;
                    if (in.status()!=QDataStream::Ok) {
                        throw QString("Ongeldig formaat");
                    }
                }
                debugstring += "\n";
            }
            message(debugstring);
        } catch (QString& Q) {
            message(Q);
        }
    }
    update();
}


void SchaakGUI::undo() {
    message("UNDO");
}

void SchaakGUI::redo() {
    message("REDO");
}


void SchaakGUI::visualizationChange() {
    QString visstring = QString(displayMoves()?"T":"F")+(displayKills()?"T":"F")+(displayThreats()?"T":"F");
    message(QString("Nieuwe settings : ")+visstring);
}










