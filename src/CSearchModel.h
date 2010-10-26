#ifndef CSEARCHMODEL_H
#define CSEARCHMODEL_H

#include <QAbstractItemModel>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

enum Media
{
	ALL = 0,
	MOVIES,
	IMAGE,
	MUSIC,
	PR0N
};

enum OrderBy
{
	RELEVANCY = 0, // nothing
	NEWEST, // "%40id"
	RATING, // "ratings"
	DOWNLOAD_COUNT, // "downloads"
	SIZE_DESC, // "size"
	SIZE_ASC // "size&podle=asc"
};

struct SearchInfo
{
	QString pattern;
	quint32 pageCount;
	Media media;
	OrderBy orderBy;
};

struct SearchData
{
	quint32 id;
	QString name;
	QString size;
};

typedef QList<SearchData> SearchList;

class CSearchModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		enum Columns
		{
			COL_NAME = 0,
			COL_SIZE,
			NUM_COL
		};

	public:
		CSearchModel(QObject * parent);

		int rowCount(const QModelIndex & parent) const;
		int columnCount(const QModelIndex & parent) const;

		QModelIndex index(int row, int column, const QModelIndex & parent) const;
		QModelIndex parent(const QModelIndex & index) const;
		bool hasChildren(const QModelIndex & parent) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		QVariant data(const QModelIndex & index, int role) const;

		void search(const SearchInfo & info);
		const SearchData & searchData(int row) const;

	private:
		QNetworkAccessManager m_manager;
		SearchList m_data;

	private slots:
		void searchComplete(QNetworkReply * reply);
};

#endif // CSEARCHMODEL_H
