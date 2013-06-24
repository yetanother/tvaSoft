#include "chartSetup.h"
#include "..\src\gui\itemviews\qtablewidget.h"

void tva::dataToTableItem( const QString& str, const int&r,const int&c, QTableWidget *tw )
{
	QTableWidgetItem * Item = new QTableWidgetItem;
	//
	Item->setText(str);
	Item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	tw->setItem(r,c,Item);
}

QCPGraph* tva::postProc::vec2Chart( const postProc::policyChart& policy, const tva::opVecReal& opVx, const tva::vecReal& vy, QCustomPlot *plot )
{
	if (policy.needClear)
	{
		/*int n=*/ plot->clearGraphs();
	}
	size_t size = vy.size();
	tva::vecReal vx;
	// generate data:
	if (opVx.isSettled())
	{
		vx = opVx.get();
	}
	else
	{
		for (size_t i=0; i<size; ++i)
		{
			vx.push_back(i);//index
		}
	}

	//const auto& mx = vx_vy.first;
	//const auto& my = vx_vy.second;
	QVector<double> x, y;
	for (size_t i=0; i<size; ++i)
	{
		x.push_back(vx[i]);//index
		y.push_back(vy[i]);//value
	}

	// create graph and assign data to it:
	QCPGraph * graph;
	if (policy.leftAxis)
	{
		graph	= plot->addGraph();
	}
	else
	{
		graph = plot->addGraph(plot->xAxis, plot->yAxis2);
		plot->yAxis2->setVisible(true);
	}

	//auto * graph = plot->addPlottable();

	{//set graph
		QPen graphPen;

		if (policy.lStyle.isSettled())
		{
			graph->setLineStyle(policy.lStyle.get().style);
			graph->setScatterStyle(policy.lStyle.get().scatter);

			if (policy.lStyle.get().name.isSettled())
			{
				graph->setName(policy.lStyle.get().name.get());
			}

			if (policy.lStyle.get().color.isSettled())
			{
				graphPen.setColor(policy.lStyle.get().color.get());
			}
			else
			{
				graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
			}

			//graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
		}
		else
		{
			tva::chartSetup::lineStyle ls;
			graph->setLineStyle(ls.style);
		}

		graph->setPen(graphPen);
	}

	graph->setData(x, y);

	QCPAxis* xAxis= plot->xAxis;
	QCPAxis* yAxis;// = plot->yAxis;
	if (policy.leftAxis)
	{
		yAxis = plot->yAxis;
	}
	else
	{
		yAxis = plot->yAxis2;
	}

	if (policy.style.isSettled())
	{
		plot->setTitle(policy.style.get().title);
	}

	tva::chartSetup::setupAxisesStyle(policy.style, xAxis, yAxis);
	//
	plot->replot();
	return graph;
}

void tva::noDataToTable( QTableWidget *tw )
{
	tw->clearContents();
	//tw->setColumnCount(1);
	tw->setRowCount(1);
	//
	dataToTableItem(tva::str::noData, 0,0, tw);
}

void tva::postProc::columnus2Chart
	( const tva::chartSetup::opChartStyle& style
	, const tva::opVecReal& dataZ
	, const tva::vecvecReal& mat
	, QCustomPlot *plot
	, const tva::opVecOpString& vecAdditionalLineText/*=tva::opVecOpString()*/
	)
{
	plot->clearGraphs();
	//plot->legend->setVisible(true);

	const auto& nCol = tva::cols(mat);

	tva::chartSetup::lineStyle lStyle;
	vecReal x;
	const auto nRow =  tva::rows(mat);
	if (dataZ.isSettled())
	{
		x=dataZ.get();
	}
	else
	{
		for(auto j=0; j<nRow; ++j)
		{
			x.push_back(j);
		}
	}

	for(auto i=0; i<nCol; ++i)
	{
		//
		vecReal y;
		{//filfull y
			//const auto nRow = mat.Ncols();
			for(auto j=0; j<nRow; ++j)
			{
				//x.push_back(j);
				//y.push_back(mat[j][i]);
				y.push_back(mat[i][j]);
			}
		}
		//if (i==0)
		//tva::postProc::axisStyle axStyle;
		{
			static const QString strMode("m");
			QString name= QString("%1%2").arg(strMode).arg(i+1);;
			if (vecAdditionalLineText.isSettled())
			{
				const auto&vec=vecAdditionalLineText.get();
				const auto&ai=vec[i];
				if (ai.isSettled())
				{
					name.append(ai.get());
				}
			}

			//const QString name
			const QCP::ScatterStyle markStyle = QCP::ScatterStyle(i%9+1);
			lStyle.scatter = markStyle;
			lStyle.name = name;
			//
			tva::chartSetup::opChartStyle corrStyle;
			if (i==0)
			{
				corrStyle = style;
			}
			//
			postProc::policyChart policy;
			policy.style = corrStyle;
			policy.lStyle = lStyle;
			policy.needClear = false;
			policy.leftAxis = true;

			tva::postProc::vec2Chart( policy, x,y , plot);
		}
	}
}

tva::opReal tva::TableItemToDouble( const QTableWidget *tw, const int&r,const int&c )
{
	tva::opReal value;
	//value.reset();
	const auto& str = tw->item(r,c)->text();
	value = tva::util::toDouble(str);
	//
	return value;
}

void tva::chartSetup::setupAxisesStyle( const opChartStyle& style, QCPAxis* xAxis, QCPAxis* yAxis )
{
	if (style.isSettled())
	{
		tva::chartSetup::setupAxisStyle( style.get().axStyleX, xAxis);
		tva::chartSetup::setupAxisStyle( style.get().axStyleY, yAxis);
	}
}

void tva::chartSetup::setupAxisStyle( const tva::chartSetup::opAxisStyle& style, QCPAxis* axis )
{
	if (style.isSettled())
	{
		axis->setLabel(style.get().title);

		// set axes ranges, so we see all data:
		const auto &opX = style.get().lim;
		if (opX.isSettled())
		{
			const auto& min = opX.get().first;
			const auto& max = opX.get().second;
			if (min.isSettled() && max.isSettled())
			{
				double mult(1.0);
				if (style.get().multiplicator.isSettled())
				{
					mult=style.get().multiplicator.get();
				}
				axis->setRange( min.get()*mult, max.get()*mult);
				//xAxis->setAutoTicks(false);
			}
		}
	}
}