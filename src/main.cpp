#include <QtGui>
#include <QApplication>
#include <stdlib.h>

#include "tetriswindow.h"

#include "control.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TetrisWindow window;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    window.show();

    return app.exec();
}
