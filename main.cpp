//
//  !!! Dit bestand moet je in principe NIET wijzigen !!!
//

#include  <QApplication>
#include  "ChessGUI.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessGUI w;
    w.show();

    return a.exec();
}
