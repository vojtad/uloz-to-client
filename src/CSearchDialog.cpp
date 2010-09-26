#include "CSearchDialog.h"

CSearchDialog::CSearchDialog(QWidget * parent) :
	QDialog(parent),
	m_searchModel(this)
{
	m_ui.setupUi(this);

	m_ui.searchView->setModel(&m_searchModel);
	connect(this, SIGNAL(accepted()), this, SLOT(addDownloads()));
}

const CSearchModel & CSearchDialog::searchModel() const
{
	return m_searchModel;
}

QModelIndexList CSearchDialog::selected() const
{
	return m_ui.searchView->selectionModel()->selectedRows(0);
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
