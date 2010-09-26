#ifndef CDOWNLOADDELEGATE_H
#define CDOWNLOADDELEGATE_H

#include <QItemDelegate>

class CDownloadDelegate : public QItemDelegate
{
	public:
		CDownloadDelegate(QObject *parent = 0);

		QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
		void paint(QPainter * painter, const QStyleOptionViewItem & option,
				   const QModelIndex & index) const;
		QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option,
							   const QModelIndex & index) const;
		void setEditorData(QWidget * editor, const QModelIndex & index) const;
		void setModelData(QWidget * editor, QAbstractItemModel * model,
						  const QModelIndex & index) const;
		void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option,
								  const QModelIndex & index) const;
};

#endif // CDOWNLOADDELEGATE_H
