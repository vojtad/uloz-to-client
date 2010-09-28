#ifndef CSEARCHDIALOG_H
#define CSEARCHDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>

#include "ui_CSearchDialog.h"
#include "CSearchModel.h"

class CSearchDialog : public QDialog
{
    Q_OBJECT
	public:
		CSearchDialog(QWidget *parent = 0);

		const SearchData & data(int row) const;
		QModelIndexList selected() const;

	protected:
		void changeEvent(QEvent *e);

	private:
		Ui::CSearchDialog m_ui;

		QSortFilterProxyModel m_filterModel;
		CSearchModel m_searchModel;

	private slots:
		void on_searchButton_clicked();
  void addDownloads();
};

#endif // CSEARCHDIALOG_H
