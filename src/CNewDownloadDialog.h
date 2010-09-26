#ifndef CNEWDOWNLOADDIALOG_H
#define CNEWDOWNLOADDIALOG_H

#include <QDialog>
#include <QStringList>

#include "ui_CNewDownloadDialog.h"

class CNewDownloadDialog : public QDialog
{
    Q_OBJECT

	public:
		CNewDownloadDialog(QWidget * parent = 0);

		QStringList urlList() const;
		bool autoStart() const;

	protected:
		void changeEvent(QEvent *e);

	private:
		Ui::CNewDownloadDialog m_ui;
};

#endif // CNEWDOWNLOADDIALOG_H
