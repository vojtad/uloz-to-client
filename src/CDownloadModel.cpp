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

#include "CDownloadModel.h"

#include <QDebug>
#include <QStringList>
#include <QNetworkReply>
#include <QPixmap>

QString progressString(const DownloadData & data)
{
	switch(data.state)
	{
	case STATE_NONE:
		return QObject::tr("None");
	case STATE_WAITING:
		return QObject::tr("Waiting");
	case STATE_CAPTCHA:
		return QObject::tr("Solving captcha");
	case STATE_RETRIEVING_URL:
		return QObject::tr("Retrieving URL");
	case STATE_FINISHED:
		return QObject::tr("Finished");
	case STATE_NET_ERROR:
		return QObject::tr("Network error");
	case STATE_ERROR:
		return QObject::tr("Error");
	case STATE_WILL_RETRY:
		return QObject::tr("Retrying");
	case STATE_DOWNLOADING:
		if(data.size > 0)
			return QString("%1 %").arg(data.downloaded * 100 / double(data.size), 0, 'f', 2);
		return QObject::tr("Downloading");
	case STATE_ABORTED:
		return QObject::tr("Aborted");
	default:
		return QObject::tr("Unknown state");
	}
}

QString errorString(const DownloadData & data)
{
	if(data.state == STATE_NET_ERROR)
		return QString("NetworkError: %1").arg(data.miscState);

	if(data.state == STATE_ERROR)
	{
		switch(data.miscState)
		{
		case ERROR_PARSE_SUBMIT_URL:
			return QObject::tr("Cannot parse submit URL");
		case ERROR_PARSE_CAPTCHA_ID:
			return QObject::tr("Cannot parse captcha ID");
		case ERROR_CANNOT_SOLVE_CAPTCHA:
			return QObject::tr("Cannot solve captcha");
		case ERROR_CANNOT_OPEN_FILE:
			return QObject::tr("Cannot open destination file");
		default:
			return QObject::tr("Unknown error");
		}
	}

	return QObject::tr("No error");
}

CDownloadModel::CDownloadModel(QObject * parent) :
		QAbstractItemModel(parent)
{
}

int CDownloadModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return m_data.count();
}

int CDownloadModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return NUM_COL;
}

QModelIndex CDownloadModel::index(int row, int column, const QModelIndex & parent) const
{
	if(parent.isValid() || row < 0 || row >= m_data.count())
		return QModelIndex();

	return createIndex(row, column);
}

QModelIndex CDownloadModel::parent(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return QModelIndex();
}

bool CDownloadModel::hasChildren(const QModelIndex & parent) const
{
	return !parent.isValid();
}

QVariant CDownloadModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role == Qt::DisplayRole)
	{
		switch(section)
		{
		case COL_NAME:
			return tr("Name");
		case COL_SPEED:
			return tr("Speed");
		case COL_SIZE:
			return tr("Size");
		case COL_PROGRESS:
			return tr("Progress");
		case COL_ERROR:
			return tr("Error");
		}
	}

	return QVariant();
}

QVariant CDownloadModel::data(const QModelIndex & index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		const DownloadData & data = m_data.at(index.row());
		switch(index.column())
		{
		case COL_NAME:
			if(!data.fileName.isEmpty())
				return data.fileName;
			return data.url.split("/", QString::SkipEmptyParts).last();
		case COL_SPEED:
			return CDownloadModel::formatSize(data.speed, "B/s");
		case COL_SIZE:
			return CDownloadModel::formatSize(data.size, "B");
		case COL_PROGRESS:
			return progressString(data);
		case COL_ERROR:
			return errorString(data);
		}
	}
	else if(role == Qt::DecorationRole)
	{
		const DownloadData & data = m_data.at(index.row());

		if(index.column() == 0)
		{
			switch(data.state)
			{
			//case STATE_NONE:
			case STATE_WAITING:
			case STATE_WILL_RETRY:
				return QPixmap(":/pixmaps/download-waiting.png").scaledToHeight(16, Qt::SmoothTransformation);
			case STATE_CAPTCHA:
			case STATE_RETRIEVING_URL:
			case STATE_DOWNLOADING:
				return QPixmap(":/pixmaps/start-download.png").scaledToHeight(16, Qt::SmoothTransformation);
			case STATE_NET_ERROR:
			case STATE_ERROR:
			case STATE_ABORTED:
				return QPixmap(":/pixmaps/download-aborted.png").scaledToHeight(16, Qt::SmoothTransformation);
			case STATE_FINISHED:
				return QPixmap(":/pixmaps/download-done.png").scaledToHeight(16, Qt::SmoothTransformation);
			}
		}
	}

	return QVariant();
}

bool CDownloadModel::updateDownload(const DownloadData & data)
{
	for(int i = 0; i < m_data.count(); ++i)
	{
		if(m_data.at(i).id == data.id)
		{
			m_data[i] = data;
			emit dataChanged(index(i, 0, QModelIndex()), index(i, NUM_COL, QModelIndex()));
			return true;
		}
	}

	return false;
}

void CDownloadModel::insertDownloads(const DownloadList & data)
{
	beginInsertColumns(QModelIndex(), m_data.count(), m_data.count() + data.size());
	m_data.append(data);
	endInsertRows();
}

void CDownloadModel::removeDownload(quint32 id)
{
	int row;
	DownloadList::iterator it;

	for(row = 0, it = m_data.begin(); it != m_data.end(); ++row, ++it)
	{
		if(it->id == id)
		{
			beginRemoveRows(QModelIndex(), row, row);
			m_data.erase(it);
			endRemoveRows();
			break;
		}
	}
}

const DownloadData & CDownloadModel::data(quint32 row) const
{
	return m_data.at(row);
}

QString CDownloadModel::formatSize(qint64 size, const QString & str)
{
	if(size < 1024)
		return QString("%1 %2").arg(size).arg(str);

	if(size < 1024 * 1024)
		return QString("%1 Ki%2").arg(size / 1024.0f, 0, 'f', 2).arg(str);

	if(size < 1024 * 1024 * 1024)
		return QString("%1 Mi%2").arg(size / (1024.0f * 1024.0f), 0, 'f', 2).arg(str);

	return QString("%1 Gi%2").arg(size / (1024.0f * 1024.0f * 1024.0f), 0, 'f', 2).arg(str);
}

DownloadStatus CDownloadModel::status() const
{
	DownloadStatus status;

	foreach(const DownloadData & d, m_data)
	{
		status.totalSpeed += d.speed;
		status.totalCount++;

		if(d.isWaiting())
			status.waitingCount++;
		else if(d.isActive())
			status.activeCount++;
		else if(d.isFailed())
			status.failedCount++;
	}

	return status;
}

void CDownloadModel::clear()
{
	beginResetModel();
	m_data.clear();
	endResetModel();
}

void DownloadData::serialize(QDataStream & stream) const
{
	stream << id << url << userAgent << fileName << quint8(state) << miscState
			<< downloaded << size << speed;
}

void DownloadData::unserialize(QDataStream & stream)
{
	quint8 st;

	stream >> id >> url >> userAgent >> fileName >> st >> miscState
			>> downloaded >> size >> speed;

	state = DownloadState(st);
}

bool DownloadData::isWaiting() const
{
	return state == STATE_WAITING;
}

bool DownloadData::isActive() const
{
	return state == STATE_CAPTCHA || state == STATE_RETRIEVING_URL || state == STATE_DOWNLOADING ||
			state == STATE_WILL_RETRY || state == STATE_ABORTING;
}

bool DownloadData::isFailed() const
{
	return state == STATE_ERROR || state == STATE_NET_ERROR;
}
