#ifndef CDOWNLOADMODEL_H
#define CDOWNLOADMODEL_H

#include <QAbstractItemModel>

enum DownloadState
{
	STATE_NONE = 0,
	STATE_WAITING,
	STATE_CAPTCHA,
	STATE_RETRIEVING_URL,
	STATE_DOWNLOADING,
	STATE_FINISHED,
	STATE_NET_ERROR,
	STATE_ERROR,
	STATE_WILL_RETRY,
	STATE_ABORTED,
	STATE_ABORTING
};

enum Errors
{
	ERROR_PARSE_SUBMIT_URL = 0,
	ERROR_PARSE_CAPTCHA_ID,
	ERROR_CANNOT_SOLVE_CAPTCHA,
	ERROR_CANNOT_OPEN_FILE
};

struct DownloadData
{
	DownloadData()
	{
		id = 0;
		state = STATE_NONE;
		miscState = 0;
		downloaded = 0;
		size = 0;
		speed = 0;
	}

	DownloadData(const DownloadData & data) :
		id(data.id),
		url(data.url),
		userAgent(data.userAgent),
		fileName(data.fileName),
		state(data.state),
		miscState(data.miscState),
		downloaded(data.downloaded),
		size(data.size),
		speed(data.speed)
	{
	}

	quint32 id;
	QString url;
	QString userAgent;

	QString fileName;
	DownloadState state:8;
	quint16 miscState;
	qint64 downloaded;
	qint64 size;
	quint32 speed;

	void serialize(QDataStream & stream) const;
	void unserialize(QDataStream & stream);

	bool isActive() const;
	bool isWaiting() const;
	bool isFailed() const;
};

struct DownloadStatus
{
	DownloadStatus() :
		totalSpeed(0),
		totalCount(0),
		waitingCount(0),
		failedCount(0),
		activeCount(0)
	{
	}

	DownloadStatus(const DownloadStatus & status) :
		totalSpeed(status.totalSpeed),
		totalCount(status.totalCount),
		waitingCount(status.waitingCount),
		failedCount(status.failedCount),
		activeCount(status.activeCount)
	{
	}

	qint64 totalSpeed;
	quint32 totalCount;
	quint32 waitingCount;
	quint32 failedCount;
	quint32 activeCount;
};

typedef QList<DownloadData> DownloadList;

class CDownloadModel : public QAbstractItemModel
{
	public:
		enum Columns
		{
			COL_NAME = 0,
			COL_SPEED,
			COL_SIZE,
			COL_PROGRESS,
			COL_ERROR,
			NUM_COL
		};

	public:
		CDownloadModel(QObject * parent);

		int rowCount(const QModelIndex & parent) const;
		int columnCount(const QModelIndex & parent) const;

		QModelIndex index(int row, int column, const QModelIndex & parent) const;
		QModelIndex parent(const QModelIndex & index) const;
		bool hasChildren(const QModelIndex & parent) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		QVariant data(const QModelIndex & index, int role) const;

		bool updateDownload(const DownloadData & data);
		void insertDownloads(const DownloadList & data);
		void removeDownload(quint32 id);

		const DownloadData & data(quint32 id) const;
		static QString formatSize(qint64 size, const QString & str);
		DownloadStatus status() const;

		void clear();

	private:
		DownloadList m_data;
};

#endif // CDOWNLOADMODEL_H
