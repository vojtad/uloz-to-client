#include <QtGui/QApplication>
#include "CMainWindow.h"

#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication::setApplicationName("uloz-to-client");
	QApplication::setOrganizationDomain("dev-z.cz");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    QApplication a(argc, argv);
    CMainWindow w;
    w.show();
    return a.exec();
}
