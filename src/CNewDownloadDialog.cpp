#include "CNewDownloadDialog.h"

CNewDownloadDialog::CNewDownloadDialog(QWidget * parent) :
	QDialog(parent)
{
	m_ui.setupUi(this);
}

QStringList CNewDownloadDialog::urlList() const
{
	return m_ui.urlListEdit->toPlainText().split(QRegExp("\\s+"), QString::SkipEmptyParts);
}

bool CNewDownloadDialog::autoStart() const
{
	return m_ui.autoStartCheckBox->isChecked();
}

void CNewDownloadDialog::changeEvent(QEvent * e)
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
