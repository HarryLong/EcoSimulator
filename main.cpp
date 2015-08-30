#include <QApplication>
#include "gui/main_window.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    qWarning() << "Launching Ecosimulator...";
    QApplication app(argc, argv);
    MainWindow w;
    w.resize(w.sizeHint());
    w.showMaximized();

    return app.exec();
}
