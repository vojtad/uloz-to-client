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

#include "CSearchModel.h"

#include <QDebug>
#include <QStringList>
#include "CDownloadModel.h"

CSearchModel::CSearchModel(QObject * parent) :
	QAbstractItemModel(parent),
	m_manager(this)
{
	connect(&m_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(searchComplete(QNetworkReply *)));
}

int CSearchModel::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return m_data.count();
}

int CSearchModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return NUM_COL;
}

QModelIndex CSearchModel::index(int row, int column, const QModelIndex & parent) const
{
	if(parent.isValid() || row < 0 || row >= m_data.count())
		return QModelIndex();

	return createIndex(row, column);
}

QModelIndex CSearchModel::parent(const QModelIndex & index) const
{
	Q_UNUSED(index);

	return QModelIndex();
}

bool CSearchModel::hasChildren(const QModelIndex & parent) const
{
	return !parent.isValid();
}

QVariant CSearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(orientation);
	if(role == Qt::DisplayRole)
	{
		switch(section)
		{
		case COL_NAME:
			return tr("Name");
		case COL_SIZE:
			return tr("Size");
		}
	}

	return QVariant();
}

QVariant CSearchModel::data(const QModelIndex & index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		const SearchData & data = m_data.at(index.row());
		switch(index.column())
		{
		case COL_NAME:
			return data.name;
		case COL_SIZE:
			return data.size;//CDownloadModel::formatSize(data.size, "B");
		}
	}

	return QVariant();
}

void CSearchModel::search(const SearchInfo & info)
{
	beginResetModel();
	m_data.clear();
	endResetModel();

	QString getData;

	getData.append(QString("?do=ajaxSearch&q=%1").arg(info.pattern));

	switch(info.media)
	{
	case MOVIES:
		getData.append("&media=video"); break;
	case IMAGE:
		getData.append("&media=image"); break;
	case MUSIC:
		getData.append("&media=music"); break;
	case PR0N:
		getData.append("&disclaimer=1&media=pr0n"); break;
	case ALL:
	default:
		break;
	}

	switch(info.orderBy)
	{
	case NEWEST:
		getData.append("&type=%40id"); break;
	case RATING:
		getData.append("&type=ratings"); break;
	case DOWNLOAD_COUNT:
		getData.append("&type=downloads"); break;
	case SIZE_DESC:
		getData.append("&type=size"); break;
	case SIZE_ASC:
		getData.append("&type=size&podle=asc"); break;
	case RELEVANCY:
	default:
		break;
	}

	QNetworkRequest request;
	request.setRawHeader("X-Requested-With", "XMLHttpRequest");
	request.setRawHeader("Accept", "application/json, text/javascript, */*");

	for(quint32 i = 1; i <= info.pageCount; ++i)
	{
		QString url = "http://www.uloz.to/hledej/";
		url.append(getData);
		if(i > 1)
			url.append(QString("&pos=%1").arg(i));
		request.setUrl(QUrl(url));

		m_manager.get(request);
	}
}

const SearchData & CSearchModel::searchData(int row) const
{
	return m_data.at(row);
}

void CSearchModel::searchComplete(QNetworkReply * reply)
{
	if(reply->isFinished() && reply->error() == QNetworkReply::NoError)
	{
		QRegExp rx("<div class=\\\\\"innerthumb\\\\\">[^<]*<a href=\\\\\"([^\"]+)\\\\\"><img[^>]*\\\\/><\\\\/a>[^<]*<\\\\/div>[^<]*<\\\\/div>[^<]*<div class=\\\\\"info\\\\\">[^<]*<h4>[^<]*<a class=\\\\\"name\\\\\" href=\\\\\"[^\"]+\\\\\" title=\\\\\"([^\"]+)\\\\\">[^<]+<\\\\/a>[^<]*<\\\\/h4>[^<]*<span class=\\\\\"lft\\\\\">[^<]*<\\\\/span>[^<]*<span[^>]*>([^<]+)<\\\\/span>");
		QRegExp rxSize("^([0-9:]+) \\| (.+)$");
		QByteArray data(reply->readAll());
		SearchData search;
		SearchList list;

		for(int pos = rx.indexIn(data); pos != -1; pos = rx.indexIn(data, pos + 1))
		{
			search.link = rx.cap(1).replace("\\/", "/");
			search.name = rx.cap(2);
			search.size = rx.cap(3);
			if(rxSize.indexIn(search.size) != -1)
			{
				search.videoLength = rxSize.cap(1);
				search.size = rxSize.cap(2);
			}
			list.append(search);
		}

		beginInsertRows(QModelIndex(), m_data.count(), m_data.count() + list.count());
		m_data.append(list);
		endResetModel();
	}

	reply->deleteLater();
}
