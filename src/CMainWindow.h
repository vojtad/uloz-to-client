#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QTcpSocket>

#include "ui_CMainWindow.h"
#include "CDownloadModel.h"

enum ClientOpcodes
{
	OPCODE_ADD = 0, // data = url
	OPCODE_REMOVE, // data = id
	OPCODE_START, // data = id
	OPCODE_STOP, // data = id
	OPCODE_LIST
};

class CMainWindow : public QMainWindow
{
		Q_OBJECT
	public:
		CMainWindow(QWidget * parent = 0);
		~CMainWindow();

	protected:
		void changeEvent(QEvent *);

	private:
		void loadSettings();
		void saveSettings();

		void changeOnConnected(bool connected);
		void handleUpdate(QDataStream & stream);
		void handleRemove(QDataStream & stream);

	private:
		Ui::CMainWindow m_ui;
		QSettings m_settings;

		QLabel m_totalSpeedLabel;
		CDownloadModel m_downloadModel;
		QTcpSocket m_socket;

	private slots:
		void on_enableQueueCheckBox_stateChanged(int state);
  void on_actionStop_download_triggered();
		void on_actionStart_download_triggered();
		void on_actionSearch_triggered();
		void on_actionRemove_download_triggered();
		void on_actionAdd_download_triggered();
		void on_actionConnect_to_daemon_triggered();

		void socketConnected();
		void socketError(QAbstractSocket::SocketError);
		void socketDisconnected();
		void socketRead();
};

#endif // CMAINWINDOW_H
