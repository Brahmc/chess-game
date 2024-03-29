//
// Created by toonc on 12/17/2021.
//

#ifndef CHESS_CHESS_GUI_H
#define CHESS_CHESS_GUI_H

#include "guicode/ChessWindow.h"
#include "ChessPiece.h"
#include <QMessageBox>
#include <QAction>
#include <optional>

class ChessGUI: public ChessWindow {
public:
    ChessGUI();

    void update() override;

private:
    Game* g;
    std::optional<std::pair<int, int>> selected;

    void clicked(int x, int y) override;
    void newGame() override;
    void open() override;
    void save() override;
    void undo() override;
    void redo() override;
    void visualizationChange() override;

/*  Overgeerfde functies van ChessWindow:

    void removeAllMarking();
    void clearBoard();
    void setItem(int i,int j,ChessPiece* p);
    void setPieceThreat(int i,int j,bool b);
    void removeAllPieceThreats();
    void setTileSelect(int i,int j,bool b);
    void setTileFocus(int i,int j,bool b);
    void setTileThreat(int i,int j,bool b);

    bool displayKills();
    bool displayMoves();
    bool displayThreats();
*/
    void drawPromotionSelection();

    bool handlePromotionSelected(int r, int k);

    void updateThreats();

    static QString pieceToString(ChessPiece *piece);

    void displayStatusMessage();

    void updateSelectedMoves();
};

#endif //CHESS_CHESS_GUI_H


