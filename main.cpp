#include <QApplication>

#include "loading.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Loading w;
    w.show();
    return a.exec();
}
