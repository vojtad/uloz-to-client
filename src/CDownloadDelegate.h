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
