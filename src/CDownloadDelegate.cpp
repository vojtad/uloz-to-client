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

#include "CDownloadDelegate.h"

#include <QApplication>

#include "CDownloadModel.h"

CDownloadDelegate::CDownloadDelegate(QObject * parent) :
    QItemDelegate(parent)
{
}

QSize CDownloadDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QVariant data = index.data(Qt::DisplayRole);

	if(!data.isValid() || index.column() == CDownloadModel::COL_PROGRESS)
		return QSize(120, 20);

	return QSize(QItemDelegate::sizeHint(option, index).width(), 20);
}

void CDownloadDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	const CDownloadModel * model = static_cast<const CDownloadModel *>(index.model());

	if(index.row() < model->rowCount(QModelIndex()) && index.column() == CDownloadModel::COL_PROGRESS)
	{
		const DownloadData & data = model->data(index.row());
		QStyleOptionProgressBarV2 opts;

		opts.maximum = data.size == 0 ? 1 : data.size;
		opts.minimum = 0;
		opts.progress = data.downloaded;
		opts.rect = option.rect;
		opts.text = index.data(Qt::DisplayRole).toString();
		opts.textVisible = true;
		opts.state = QStyle::State_Enabled;

		QApplication::style()->drawControl(QStyle::CE_ProgressBar, &opts, painter);
	}
	else
		QItemDelegate::paint(painter, option, index);
}

QWidget * CDownloadDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QItemDelegate::createEditor(parent, option, index);
}

void CDownloadDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	return QItemDelegate::setEditorData(editor, index);
}

void CDownloadDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
	return QItemDelegate::setModelData(editor, model, index);
}

void CDownloadDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	return QItemDelegate::updateEditorGeometry(editor, option, index);
}
