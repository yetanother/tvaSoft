#pragma once
#include <qstring>
#include "qwidget.h"
#include "qmessagebox.h"

namespace tva
{
	namespace util
	{
		void showMsg(const QString& msg, QWidget *pwgt = nullptr);

		template<typename T>
		QString toString(const T&t)
		{
			//QString res = QString("%1").arg(t);
			return QString("%1").arg(t);
		}

		//
		inline
		void tva::util::showMsg( const QString& msg, QWidget *pwgt /*= nullptr*/ )
		{
			QMessageBox::warning(pwgt, QObject::tr("debug message"), msg, QMessageBox::Ok );
		}
	}
}
