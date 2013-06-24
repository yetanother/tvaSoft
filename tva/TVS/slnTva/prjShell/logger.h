#pragma once
#include "..\src\corelib\kernel\qobjectdefs.h"
#include "..\src\gui\widgets\qplaintextedit.h"

class MainWindow;

namespace tva
{
	class Logger: public QObject
	{
		Q_OBJECT
	private:
		QPlainTextEdit* w_;
	public:
		Logger(QPlainTextEdit* w):w_(w){}
	public slots:
		void clear(){w_->clear();}
		void addLine(const QString& str){w_->appendPlainText(str);}
		void toPlainText(QString& str){str= w_->toPlainText();};
	};
}
