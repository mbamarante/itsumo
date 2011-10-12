#include <qapplication.h>
#include "mainwindowimpl.h"

int main (int argc, char **argv)
{
 QApplication app(argc, argv);
 MainWindowImpl* mw = new MainWindowImpl();

 app.setMainWidget(mw);

 mw->show();

 return app.exec();
}
