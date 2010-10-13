#include "CMainWindow.h"

#include <QHostAddress>

#include "CNewDownloadDialog.h"
#include "CSearchDialog.h"

void CMainWindow::on_actionConnect_to_daemon_triggered()
{
	m_downloadModel.clear();

	if(m_socket.state() == QAbstractSocket::UnconnectedState)
	{
		QHostAddress host(m_hostAddressEdit->text());
		quint16 port = m_hostPortSpinBox->value();

		m_socket.connectToHost(host, port);
	}
	else
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

			stream << qint64(0);
			stream << quint8(OPCODE_ADD);
			foreach(const QString & url, list)
			{
				stream << dialog.autoStart();
				stream << url;
			}

			stream.device()->seek(0);
			stream << qint64(buffer.size() - sizeof(qint64));

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

			stream << qint64(0);
			stream << quint8(OPCODE_REMOVE);
			foreach(const QModelIndex & index, list)
			{
				stream << quint32(m_downloadModel.data(index.row()).id);
			}

			stream.device()->seek(0);
			stream << qint64(buffer.size() - sizeof(qint64));

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

			stream << qint64(0);
			stream << quint8(OPCODE_START);
			foreach(const QModelIndex & index, list)
			{
				stream << quint32(m_downloadModel.data(index.row()).id);
			}

			stream.device()->seek(0);
			stream << qint64(buffer.size() - sizeof(qint64));

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

			stream << qint64(0);
			stream << quint8(OPCODE_STOP);
			foreach(const QModelIndex & index, list)
			{
				stream << quint32(m_downloadModel.data(index.row()).id);
			}

			stream.device()->seek(0);
			stream << qint64(buffer.size() - sizeof(qint64));

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

		stream << qint64(0);
		stream << quint8(OPCODE_ADD);
		foreach(const QModelIndex & index, sel)
		{
			if(index.column() == 0)
			{
				stream << true;
				stream << QString("http://uloz.to%1").arg(dialog.data(index.row()).url);
			}
		}

		stream.device()->seek(0);
		stream << qint64(buffer.size() - sizeof(qint64));

		if(m_socket.isWritable())
			m_socket.write(buffer);
	}
}

void CMainWindow::on_enableQueueCheckBox_stateChanged(int state)
{
	bool enable = state == Qt::Checked;

	m_ui.maxActiveDownloadsSpinBox->setEnabled(enable);

	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream << qint64(0);
	stream << quint8(OPCODE_QUEUE);
	stream << enable;
	stream << quint8(m_ui.maxActiveDownloadsSpinBox->value());
	stream.device()->seek(0);
	stream << qint64(buffer.size() - sizeof(qint64));

	if(m_socket.isWritable())
		m_socket.write(buffer);
}

void CMainWindow::on_maxActiveDownloadsSpinBox_valueChanged(int value)
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
	stream << qint64(0);
	stream << quint8(OPCODE_QUEUE);
	stream << m_ui.enableQueueCheckBox->isEnabled();
	stream << quint8(value);
	stream.device()->seek(0);
	stream << qint64(buffer.size() - sizeof(qint64));

	if(m_socket.isWritable())
		m_socket.write(buffer);
}
