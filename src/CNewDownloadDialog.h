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
