/*
 * uloz-to-client
 * Copyright (C) 2010 Vojta Drbohlav <vojta.d@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	OPCODE_LIST,
	OPCODE_QUEUE
};

struct DownloadStatusLabels
{
	DownloadStatusLabels(QWidget * parent = 0);

	QLabel totalSpeed;
	QLabel totalCount;
	QLabel waitingCount;
	QLabel activeCount;
	QLabel failedCount;

	void update(const DownloadStatus & status);
};

class QLineEdit;

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
		void handleQueue(QDataStream & stream);

	private:
		Ui::CMainWindow m_ui;
		QSettings m_settings;

		QLineEdit * m_hostAddressEdit;
		QSpinBox * m_hostPortSpinBox;
		DownloadStatusLabels m_statusLabels;

		CDownloadModel m_downloadModel;
		QTcpSocket m_socket;

	private slots:
		void on_maxActiveDownloadsSpinBox_valueChanged(int value);
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
