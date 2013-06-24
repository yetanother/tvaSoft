#include "shell.h"
#include "mainwindow.h"
#include "logger.h"

void tva::Shell::show()
{
	if(mw_) mw_->show();
}

void tva::Shell::addLineToLogger( const QString& str ) const
{
	logger_->addLine(str);
}
