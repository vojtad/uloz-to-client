#include "CSearchModel.h"

#include <QDebug>
#include <QStringList>
#include "CDownloadModel.h"

CSearchModel::CSearchModel(QObject * parent) :
	QAbstractItemModel(parent),
	m_manager(this),
	m_reply(0)
{
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
		case COL_URL:
			return tr("Url");
		case COL_SIZE:
			return tr("Size");
		case COL_DATE:
			return tr("Date");
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
		case COL_URL:
			return data.url;
		case COL_SIZE:
			return CDownloadModel::formatSize(data.size, "B");
		case COL_DATE:
			return data.date;
		}
	}

	return QVariant();
}

void CSearchModel::search(const QString & pattern)
{
	QNetworkRequest request;
	request.setUrl(QUrl(QString("http://uloz.to/hledej/?q=%1").arg(pattern)));

	m_reply = m_manager.get(request);
	connect(m_reply, SIGNAL(finished()), this, SLOT(searchComplete()));
}

const SearchData & CSearchModel::searchData(int row) const
{
	return m_data.at(row);
}

void CSearchModel::searchComplete()
{
	if(m_reply->isFinished() && m_reply->error() == QNetworkReply::NoError)
	{
		QRegExp rxUrlName("<td align=\"left\" ?><a href=\"([^\"]+)\">([^<]+)</a></td>");
		QRegExp rxSize("<td align=\"right\" ?><span style=\"display:none\">[ ]*([0-9]+)</span>");
		QRegExp rxDate("<td align=\"right\" ?>([^<]+)</td>");
		SearchData data;
		SearchList list;

		while(!m_reply->atEnd())
		{
			QByteArray raw(m_reply->readLine());
			QString line(QString::fromUtf8(raw.data(), raw.size()));

			if(rxUrlName.indexIn(line) != -1)
			{
				data.name = rxUrlName.cap(2);
				data.url = rxUrlName.cap(1);
			}
			else if(rxSize.indexIn(line) != -1)
			{
				data.size = rxSize.cap(1).toInt();
			}
			else if(rxDate.indexIn(line) != -1)
			{
				data.date = rxDate.cap(1).replace("&nbsp;", " ");
				list.push_back(data);
			}
		}

		beginResetModel();
		m_data = list;
		endResetModel();
	}
	else
	{
		m_reply->deleteLater();
		m_reply = 0;
	}
}
