#include "CMainWindow.h"

#include "CDownloadDelegate.h"

CMainWindow::CMainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_totalSpeedLabel(this),
	m_downloadModel(this)
{
	m_ui.setupUi(this);

	m_ui.statusBar->addWidget(&m_totalSpeedLabel);

	m_ui.downloadView->setItemDelegate(new CDownloadDelegate(m_ui.downloadView));
	m_ui.downloadView->setModel(&m_downloadModel);

	connect(&m_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
	connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(&m_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::changeEvent(QEvent * e)
{
    QMainWindow::changeEvent(e);
	switch (e->type())
	{
    case QEvent::LanguageChange:
		m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void CMainWindow::changeOnConnected(bool connected)
{
	m_ui.actionConnect_to_daemon->setEnabled(!connected);
	m_ui.actionConnect_to_daemon->setText(connected ? tr("Disconnect from daemon") :
										  tr("Connect to daemon"));
}
