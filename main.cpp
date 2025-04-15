#include <QApplication>
#include <QResource>

#include "loading.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QString rcc_path = QCoreApplication::applicationDirPath() + "/../Resources/resource.rcc";
    QResource::registerResource(rcc_path);
    Loading w;
    w.show();
    return a.exec();
}
