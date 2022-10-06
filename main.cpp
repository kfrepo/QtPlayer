#include "QtPlayer.h"
#include <QtWidgets/QApplication>
#include "COpenFileButton.h"
#include "CVideoWidgetTopWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QtPlayer w;
    w.show();

    //CTimeSlider p;
    //p.show();

    //CVolumeSliderDialog p2;
    //p2.show();

    //CVolumeButton p3;
    //p3.show();

    return a.exec();
}
