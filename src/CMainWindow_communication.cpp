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
	while(m_socket.bytesAvailable() > qint64(sizeof(qint64)))
	{
		QDataStream stream(&m_socket);
		qint64 size;

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
				case OPCODE_QUEUE:
					handleQueue(bStream); break;
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

	m_statusLabels.update(m_downloadModel.status());
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

void CMainWindow::handleQueue(QDataStream & stream)
{
	bool enable;
	quint8 max;

	stream >> enable;
	stream >> max;

	m_ui.enableQueueCheckBox->setChecked(enable);
	m_ui.maxActiveDownloadsSpinBox->setValue(max);
}
