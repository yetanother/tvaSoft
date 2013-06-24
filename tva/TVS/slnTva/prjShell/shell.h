#pragma once
#include "..\src\corelib\kernel\qobject.h"
#include "..\..\slnPreProc\tvaModel.h"

class MainWindow;


namespace tva
{
	class Logger;
	//
	class Shell: public QObject
	{
		Q_OBJECT
	private:
		MainWindow *mw_;
		Logger* logger_;
		tva::Model model;
	public:
		Shell():mw_(nullptr){}
		void setMainWnd(MainWindow*mw){mw_= mw;}
		void setLogger(Logger* logger){logger_=logger;}

		tva::Model& getModel(){return model;}


	public slots:
		void show();
		void addLineToLogger(const QString& str)const;
	};
}