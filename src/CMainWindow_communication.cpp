#include "CMainWindow.h"

void CMainWindow::socketConnected()
{
	changeOnConnected(true);
}

void CMainWindow::socketError(QAbstractSocket::SocketError)
{

}

void CMainWindow::socketDisconnected()
{
	changeOnConnected(false);
}

void CMainWindow::socketRead()
{
	while(m_socket.bytesAvailable() > 4)
	{
		QDataStream stream(&m_socket);
		quint32 size;

		stream >> size;
		if(m_socket.bytesAvailable() >= size)
		{
			QByteArray buffer(m_socket.read(size));
			QDataStream bStream(buffer);
			quint8 opcode;

			bStream >> opcode;
			switch(opcode)
			{
				case OPCODE_LIST:
					handleUpdate(bStream); break;
				case OPCODE_REMOVE:
					handleRemove(bStream); break;
				default:
					qWarning() << "Unhandled packet:" << quint32(opcode); break;
			}
		}
	}
}

void CMainWindow::handleUpdate(QDataStream & stream)
{
	QList<DownloadData> downloadList;
	DownloadData d;

	while(!stream.atEnd())
	{
		d.unserialize(stream);
		if(!m_downloadModel.updateDownload(d))
			downloadList.push_back(d);
	}

	if(!downloadList.isEmpty())
	{
		m_downloadModel.insertDownloads(downloadList);
	}

	m_totalSpeedLabel.setText(QString("Total download speed: %1").arg(CDownloadModel::formatSize(m_downloadModel.totalSpeed(), "B/s")));
}

void CMainWindow::handleRemove(QDataStream & stream)
{
	quint32 id;
	while(!stream.atEnd())
	{
		stream >> id;
		m_downloadModel.removeDownload(id);
	}
}
