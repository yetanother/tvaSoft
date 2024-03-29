#include "mainwindow.h"
#include "ui_tvaMainWindow.h"
#include "tvaUtil.h"
#include "..\..\slnProc\prjEigenCore\eigenCore.h"
#include "shell.h"
#include <algorithm>
#include "..\prjTva\tvaDoc.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), shell(nullptr)
	  //,
  //ui(new Ui::mainFom)
{
	//srand(QDateTime::currentDateTime().toTime_t());
	//
	setupUi(this);
	//
	windowMapper = new QSignalMapper(this);
	int f1=connect(windowMapper, SIGNAL(mapped(QWidget*)),
		this, SLOT(setActiveSubWindow(QWidget*)));
	if (f1!=1)
	{
		tva::util::showMsg("Error on connection windowMapper");
	}
	//
	initConnect();
	createActions();
	//
	//slMakeMatrises();
	//slMakeEigenData();
  
  //ui->customPlot->setInteractions(QCustomPlot::iRangeDrag | QCustomPlot::iRangeZoom | QCustomPlot::iSelectAxes |
  //                                QCustomPlot::iSelectLegend | QCustomPlot::iSelectPlottables | QCustomPlot::iSelectTitle);
  //ui->customPlot->setRangeDrag(Qt::Horizontal|Qt::Vertical);
  //ui->customPlot->setRangeZoom(Qt::Horizontal|Qt::Vertical);
  //ui->customPlot->xAxis->setRange(-8, 8);
  //ui->customPlot->yAxis->setRange(-5, 5);
  //ui->customPlot->setupFullAxesBox();
  //ui->customPlot->setTitle("Interaction Example");
  //ui->customPlot->xAxis->setLabel("x Axis");
  //ui->customPlot->yAxis->setLabel("y Axis");
  //ui->customPlot->legend->setVisible(true);
  //QFont legendFont = font();
  //legendFont.setPointSize(10);
  //ui->customPlot->legend->setFont(legendFont);
  //ui->customPlot->legend->setSelectedFont(legendFont);
  //ui->customPlot->legend->setSelectable(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  //
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //addRandomGraph();
  //
  //// connect slot that ties some axis selections together (especially opposite axes):
  //connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  //// connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  //connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  //connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  //
  //// make bottom and left axes transfer their ranges to top and right axes:
  //connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  //connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
  //
  //// connect some interaction slots:
  //connect(ui->customPlot, SIGNAL(titleDoubleClick(QMouseEvent*)), this, SLOT(titleDoubleClick()));
  //connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  //connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
  //
  //// connect slot that shows a message in the status bar when a graph is clicked:
  //connect(ui->customPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));
  //
  //// setup policy and connect slot for context menu popup:
  //ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
  //connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
}

MainWindow::~MainWindow()
{
  //delete ui;
}

void MainWindow::titleDoubleClick()
{
  // Set the plot title by double clicking on it
  
  bool ok;
  //QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, ui->customPlot->title(), &ok);
  //if (ok)
  //{
  //  ui->customPlot->setTitle(newTitle);
  //  ui->customPlot->replot();
  //}
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    //bool ok;
    //QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    //if (ok)
    //{
    //  axis->setLabel(newLabel);
    //  ui->customPlot->replot();
    //}
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    //QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    //bool ok;
    //QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    //if (ok)
    //{
    //  plItem->plottable()->setName(newName);
    //  ui->customPlot->replot();
    //}
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.
   
   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.
   
   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */
  
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  //if (ui->customPlot->xAxis->selected().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selected().testFlag(QCPAxis::spTickLabels) ||
  //    ui->customPlot->xAxis2->selected().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selected().testFlag(QCPAxis::spTickLabels))
  //{
  //  ui->customPlot->xAxis2->setSelected(QCPAxis::spAxis|QCPAxis::spTickLabels);
  //  ui->customPlot->xAxis->setSelected(QCPAxis::spAxis|QCPAxis::spTickLabels);
  //}
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  //if (ui->customPlot->yAxis->selected().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selected().testFlag(QCPAxis::spTickLabels) ||
  //    ui->customPlot->yAxis2->selected().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selected().testFlag(QCPAxis::spTickLabels))
  //{
  //  ui->customPlot->yAxis2->setSelected(QCPAxis::spAxis|QCPAxis::spTickLabels);
  //  ui->customPlot->yAxis->setSelected(QCPAxis::spAxis|QCPAxis::spTickLabels);
  //}
  
  // synchronize selection of graphs with selection of corresponding legend items:
  //for (int i=0; i<ui->customPlot->graphCount(); ++i)
  //{
  //  QCPGraph *graph = ui->customPlot->graph(i);
  //  QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
  //  if (item->selected() || graph->selected())
  //  {
  //    item->setSelected(true);
  //    graph->setSelected(true);
  //  }
  //}
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
  //if (ui->customPlot->xAxis->selected().testFlag(QCPAxis::spAxis))
  //  ui->customPlot->setRangeDrag(ui->customPlot->xAxis->orientation());
  //else if (ui->customPlot->yAxis->selected().testFlag(QCPAxis::spAxis))
  //  ui->customPlot->setRangeDrag(ui->customPlot->yAxis->orientation());
  //else
  //  ui->customPlot->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  //
  //if (ui->customPlot->xAxis->selected().testFlag(QCPAxis::spAxis))
  //  ui->customPlot->setRangeZoom(ui->customPlot->xAxis->orientation());
  //else if (ui->customPlot->yAxis->selected().testFlag(QCPAxis::spAxis))
  //  ui->customPlot->setRangeZoom(ui->customPlot->yAxis->orientation());
  //else
  //  ui->customPlot->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addRandomGraph()
{
  //int n = 100; // number of points in graph
  //double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
  //double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
  //double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
  //double yOffset = (rand()/(double)RAND_MAX - 0.5)*5;
  //double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
  //double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
  //double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
  //double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
  //QVector<double> x(n), y(n);
  //for (int i=0; i<n; i++)
  //{
  //  x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
  //  y[i] = (sin(x[i]*r1*5)*sin(cos(x[i]*r2)*r4*3)+r3*cos(sin(x[i])*r4*2))*yScale + yOffset;
  //}
  //
  //ui->customPlot->addGraph();
  //ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
  //ui->customPlot->graph()->setData(x, y);
  //ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
  //if (rand()%100 > 75)
  //  ui->customPlot->graph()->setScatterStyle((QCP::ScatterStyle)(rand()%9+1));
  //QPen graphPen;
  //graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  //graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
  //ui->customPlot->graph()->setPen(graphPen);
  //ui->customPlot->replot();
}

void MainWindow::removeSelectedGraph()
{
  //if (ui->customPlot->selectedGraphs().size() > 0)
  //{
  //  ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
  //  ui->customPlot->replot();
  //}
}

void MainWindow::removeAllGraphs()
{
  //ui->customPlot->clearGraphs();
  //ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  //if (ui->customPlot->legend->selectTestLegend(pos)) // context menu on legend requested
  //{
  //  menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)QCPLegend::psTopLeft);
  //  menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)QCPLegend::psTop);
  //  menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)QCPLegend::psTopRight);
  //  menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)QCPLegend::psBottomRight);
  //  menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)QCPLegend::psBottomLeft);
  //} else  // general context menu on graphs requested
  //{
  //  menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
  //  if (ui->customPlot->selectedGraphs().size() > 0)
  //    menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
  //  if (ui->customPlot->graphCount() > 0)
  //    menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  //}
  //
  //menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  //if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  //{
  //  bool ok;
  //  int dataInt = contextAction->data().toInt(&ok);
  //  if (ok)
  //  {
  //    ui->customPlot->legend->setPositionStyle((QCPLegend::PositionStyle)dataInt);
  //    ui->customPlot->replot();
  //  }
  //}
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable)
{
  //ui->statusBar->showMessage(QString("Clicked on graph '%1'.").arg(plottable->name()), 1000);
}

void MainWindow::initConnect()
{
	//
}

void MainWindow::initWidgets()
{
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
		this, SLOT(updateWgts()));
}

void MainWindow::updateWgts()
{
	//
}

void MainWindow::newFile()
{
	tva::Doc *child = createMdiChild();
	child->show();
	updateWindowMenu();
}

tva::Doc * MainWindow::createMdiChild()
{
	tva::Doc *child = new tva::Doc(*shell);
	mdiArea->addSubWindow(child);

	//connect(child, SIGNAL(copyAvailable(bool)),
	//	cutAct, SLOT(setEnabled(bool)));
	//connect(child, SIGNAL(copyAvailable(bool)),
	//	copyAct, SLOT(setEnabled(bool)));

	return child;
}

void MainWindow::createActions()
{
	connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	//
	updateWindowMenu();
	connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
}

void MainWindow::updateWindowMenu()
{
	windowMenu->clear();
	//windowMenu->addAction(closeAct);
	//windowMenu->addAction(closeAllAct);
	//windowMenu->addSeparator();
	//windowMenu->addAction(tileAct);
	//windowMenu->addAction(cascadeAct);
	//windowMenu->addSeparator();
	//windowMenu->addAction(nextAct);
	//windowMenu->addAction(previousAct);
	//windowMenu->addAction(separatorAct);

	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	//separatorAct->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i) {
		tva::Doc *child = qobject_cast<tva::Doc*>(windows.at(i)->widget());

		QString text;
		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		} else {
			text = tr("%1 %2").arg(i + 1)
				.arg(child->userFriendlyCurrentFile());
		}
		QAction *action  = windowMenu->addAction(text);
		action->setCheckable(true);
		action ->setChecked(child == activeMdiChild());
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}

tva::Doc * MainWindow::activeMdiChild()
{
	if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
		return qobject_cast<tva::Doc *>(activeSubWindow->widget());
	return 0;
}

void MainWindow::setActiveSubWindow( QWidget *window )
{
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
