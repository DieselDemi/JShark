#include <QApplication>
#include <QPushButton>
#include "MainWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    jshark::forms::MainWidget main;
    main.show();

    return QApplication::exec();
}
