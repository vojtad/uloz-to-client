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

#include "CSearchDialog.h"

#include <QSortFilterProxyModel>

CSearchDialog::CSearchDialog(QWidget * parent) :
	QDialog(parent),
	m_filterModel(this),
	m_searchModel(this)
{
	m_ui.setupUi(this);

	m_filterModel.setSourceModel(&m_searchModel);
	m_ui.searchView->setModel(&m_filterModel);
	connect(this, SIGNAL(accepted()), this, SLOT(addDownloads()));
}

const SearchData & CSearchDialog::data(int row) const
{
	return m_searchModel.searchData(row);
}

QModelIndexList CSearchDialog::selected() const
{
	const QItemSelection & selection = m_ui.searchView->selectionModel()->selection();
	return m_filterModel.mapSelectionToSource(selection).indexes();
}

void CSearchDialog::changeEvent(QEvent * e)
{
    QDialog::changeEvent(e);
	switch (e->type())
	{
    case QEvent::LanguageChange:
		m_ui.retranslateUi(this);
        break;
    default:
        break;
    }
}

void CSearchDialog::addDownloads()
{

}

void CSearchDialog::on_searchButton_clicked()
{
	SearchInfo info;
	info.pattern = m_ui.searchEdit->text();
	info.pageCount = m_ui.pageCountSpinBox->value();
	info.media = (Media)m_ui.mediaComboBox->currentIndex();
	info.orderBy = (OrderBy)m_ui.orderByComboBox->currentIndex();
	m_searchModel.search(info);
}
