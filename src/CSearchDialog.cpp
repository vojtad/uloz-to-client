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
	m_searchModel.search(m_ui.searchEdit->text());
}
