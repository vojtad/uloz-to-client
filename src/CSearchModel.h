#ifndef CSEARCHMODEL_H
#define CSEARCHMODEL_H

#include <QAbstractItemModel>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>

struct SearchData
{
	QString name;
	QString url;
	QString date;
	qint64 size;
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
			COL_DATE,
			NUM_COL,
			COL_URL
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

		void search(const QString & pattern);
		const SearchData & searchData(int row) const;

	private:
		QNetworkAccessManager m_manager;
		SearchList m_data;

	private slots:
		void searchComplete(QNetworkReply * reply);
};

#endif // CSEARCHMODEL_H
