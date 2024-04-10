#include "QtPlayer.h"
#include <QtWidgets/QApplication>
#include "COpenFileButton.h"
#include "CVideoWidgetTopWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtPlayer w;
    w.show();

    return a.exec();
}
