#include "CMainWindow.h"

#include <QHostAddress>

#include "CNewDownloadDialog.h"
#include "CSearchDialog.h"

void CMainWindow::on_actionConnect_to_daemon_triggered()
{
	if(m_socket.state() == QAbstractSocket::UnconnectedState)
	{
		QHostAddress host(m_settings.value("communication/daemonAddress", "127.0.0.1").toString());
		quint16 port = m_settings.value("communication/daemonPort", 1234).toInt();

		m_socket.connectToHost(host, port);
	}
	else if(m_socket.state() == QAbstractSocket::ConnectedState ||
			m_socket.state() == QAbstractSocket::ConnectingState)
	{
		m_socket.close();
	}
}

void CMainWindow::on_actionAdd_download_triggered()
{
	CNewDownloadDialog dialog(this);

	if(dialog.exec() == QDialog::Accepted && m_socket.isWritable())
	{
		QStringList list = dialog.urlList();

		if(!list.isEmpty())
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);

			stream << quint32(0);
			stream << quint8(OPCODE_ADD);
			foreach(const QString & url, list)
			{
				stream << dialog.autoStart();
				stream << url;
			}

			stream.device()->seek(0);
			stream << quint32(buffer.size() - sizeof(quint32));

			m_socket.write(buffer);
		}
	}
}

void CMainWindow::on_actionRemove_download_triggered()
{
	if(m_socket.isWritable())
	{
		QModelIndexList list = m_ui.downloadView->selectionModel()->selectedRows(0);
		if(!list.isEmpty())
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);

			stream << quint32(0);
			stream << quint8(OPCODE_REMOVE);
			foreach(const QModelIndex & index, list)
			{
				stream << quint32(m_downloadModel.data(index.row()).id);
			}

			stream.device()->seek(0);
			stream << quint32(buffer.size() - sizeof(quint32));

			m_socket.write(buffer);
		}
	}
}

void CMainWindow::on_actionStart_download_triggered()
{
	if(m_socket.isWritable())
	{
		QModelIndexList list = m_ui.downloadView->selectionModel()->selectedRows(0);
		if(!list.isEmpty())
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);

			stream << quint32(0);
			stream << quint8(OPCODE_START);
			foreach(const QModelIndex & index, list)
			{
				stream << quint32(m_downloadModel.data(index.row()).id);
			}

			stream.device()->seek(0);
			stream << quint32(buffer.size() - sizeof(quint32));

			m_socket.write(buffer);
		}
	}
}

void CMainWindow::on_actionStop_download_triggered()
{
	if(m_socket.isWritable())
	{
		QModelIndexList list = m_ui.downloadView->selectionModel()->selectedRows(0);
		if(!list.isEmpty())
		{
			QByteArray buffer;
			QDataStream stream(&buffer, QIODevice::WriteOnly);

			stream << quint32(0);
			stream << quint8(OPCODE_STOP);
			foreach(const QModelIndex & index, list)
			{
				stream << quint32(m_downloadModel.data(index.row()).id);
			}

			stream.device()->seek(0);
			stream << quint32(buffer.size() - sizeof(quint32));

			m_socket.write(buffer);
		}
	}
}

void CMainWindow::on_actionSearch_triggered()
{
	CSearchDialog dialog(this);

	if(dialog.exec() == QDialog::Accepted)
	{
		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);
		QModelIndexList sel = dialog.selected();

		stream << quint32(0);
		stream << quint8(OPCODE_ADD);
		foreach(const QModelIndex & index, sel)
		{
			stream << true;
			stream << QString("http://uloz.to%1").arg(dialog.searchModel().searchData(index).url);
		}

		stream.device()->seek(0);
		stream << quint32(buffer.size() - sizeof(quint32));

		if(m_socket.isWritable())
			m_socket.write(buffer);
	}
}