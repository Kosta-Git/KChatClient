#include "kchat.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    KChat w;
    w.show();

    return a.exec();
}
