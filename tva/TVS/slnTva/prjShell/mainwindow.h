#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
//#include "qcustomplot.h"

#include "ui_tvaMainWindow.h"
#include "..\prjTva\tvaDoc.h"
#include "..\src\corelib\kernel\qsignalmapper.h"

//namespace Ui {
//	class mainFom;
//}

namespace tva
{
	class Shell;
}

class MainWindow : public QMainWindow, private Ui::mainFom
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  QPlainTextEdit* getWidgetLogger()const{return plainTextEdit;}

  void setShell(tva::Shell *s){shell= s;}

private slots:
	tva::Doc *createMdiChild();
	//
	void newFile();
	//
	void updateWgts();
	void updateWindowMenu();

	void setActiveSubWindow(QWidget *window);

	//
  void titleDoubleClick();
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void addRandomGraph();
  void removeSelectedGraph();
  void removeAllGraphs();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void graphClicked(QCPAbstractPlottable *plottable);

private:
  //Ui::mainFom *ui;
	tva::Shell* shell;

	void initConnect();
	void initWidgets();
	void createActions();
	tva::Doc *activeMdiChild();
	QSignalMapper *windowMapper;
};

#endif // MAINWINDOW_H