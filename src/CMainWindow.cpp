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

#include "CMainWindow.h"

#include "CDownloadDelegate.h"

#include <QLineEdit>

CMainWindow::CMainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_hostAddressEdit(new QLineEdit(this)),
	m_hostPortSpinBox(new QSpinBox(this)),
	m_downloadModel(this)
{
	m_ui.setupUi(this);

	m_hostAddressEdit->setMinimumWidth(120);
	m_hostAddressEdit->setMaximumWidth(120);
	m_hostPortSpinBox->setMinimumWidth(70);
	m_hostPortSpinBox->setMaximumWidth(70);
	m_hostPortSpinBox->setMinimum(1);
	m_hostPortSpinBox->setMaximum(65535);

	m_ui.mainToolBar->insertWidget(m_ui.actionConnect_to_daemon, new QLabel("Host: ", this));
	m_ui.mainToolBar->insertWidget(m_ui.actionConnect_to_daemon, m_hostAddressEdit);
	m_ui.mainToolBar->insertWidget(m_ui.actionConnect_to_daemon, new QLabel(":", this));
	m_ui.mainToolBar->insertWidget(m_ui.actionConnect_to_daemon, m_hostPortSpinBox);

	m_ui.statusBar->addWidget(&m_statusLabels.totalCount);
	m_ui.statusBar->addWidget(&m_statusLabels.waitingCount);
	m_ui.statusBar->addWidget(&m_statusLabels.activeCount);
	m_ui.statusBar->addWidget(&m_statusLabels.failedCount);
	m_ui.statusBar->addWidget(&m_statusLabels.totalSpeed);

	loadSettings();

	m_ui.downloadView->setItemDelegate(new CDownloadDelegate(m_ui.downloadView));
	m_ui.downloadView->setModel(&m_downloadModel);

	connect(&m_socket, SIGNAL(connected()), this, SLOT(socketConnected()));
	connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
			this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(&m_socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(&m_socket, SIGNAL(readyRead()), this, SLOT(socketRead()));

	changeOnConnected(false);
}

CMainWindow::~CMainWindow()
{
	saveSettings();
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

void CMainWindow::loadSettings()
{
	m_hostAddressEdit->setText(m_settings.value("communication/daemonAddress", "127.0.0.1").toString());
	m_hostPortSpinBox->setValue(m_settings.value("communication/daemonPort", "1234").toInt());
}

void CMainWindow::saveSettings()
{
	m_settings.setValue("communication/daemonAddress", m_hostAddressEdit->text());
	m_settings.setValue("communication/daemonPort", m_hostPortSpinBox->value());
}

void CMainWindow::changeOnConnected(bool connected)
{
	m_hostAddressEdit->setEnabled(!connected);
	m_hostPortSpinBox->setEnabled(!connected);

	m_ui.actionConnect_to_daemon->setEnabled(!connected);
	m_ui.actionConnect_to_daemon->setText(connected ? tr("Disconnect from daemon") :
										  tr("Connect to daemon"));

	m_ui.actionAdd_download->setEnabled(connected);
	m_ui.actionRemove_download->setEnabled(connected);
	m_ui.actionStart_download->setEnabled(connected);
	m_ui.actionStop_download->setEnabled(connected);
	//m_ui.actionSearch->setEnabled(connected);
	m_ui.enableQueueCheckBox->setEnabled(connected);
	m_ui.maxActiveDownloadsSpinBox->setEnabled(connected);
	m_ui.downloadView->setEnabled(connected);
}

DownloadStatusLabels::DownloadStatusLabels(QWidget * parent) :
		totalSpeed(parent),
		totalCount(parent),
		waitingCount(parent),
		activeCount(parent),
		failedCount(parent)
{
}

void DownloadStatusLabels::update(const DownloadStatus & status)
{
	totalSpeed.setText(QObject::tr("Total download speed: %1").arg(CDownloadModel::formatSize(status.totalSpeed, "B/s")));
	totalCount.setText(QObject::tr("Total downloads: %1").arg(status.totalCount));
	waitingCount.setText(QObject::tr("Waiting: %1").arg(status.waitingCount));
	activeCount.setText(QObject::tr("Active: %1").arg(status.activeCount));
	failedCount.setText(QObject::tr("Failed: %1").arg(status.failedCount));
}
