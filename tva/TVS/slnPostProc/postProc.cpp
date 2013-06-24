#include "postProc.h"
//#include "tvaUtil.h"
#include "tvaModel.h"

tva::postProc::chStyle tva::postProc::fMakeTypicalStyle(const tva::opVecReal& vecX, const tva::opVecReal& vecY)
{
	chStyle style;

	tva::chartSetup::axisStyle asX;
	tva::chartSetup::axisStyle asY;
	{
		tva::chartSetup::opPair limY;
		if (vecY.isSettled())
		{
			const auto& providerData = vecY.get();
			limY.first =tva::util::getMinElement(providerData);
			limY.second = tva::util::getMaxElement(providerData);
			if (limY.second.isSettled())
			{
				//limY.second = limY.second.get()*(1+percentLimitCorrection);
			}
		}

		tva::chartSetup::opPair limX;
		if (vecX.isSettled())
		{
			const auto& providerData = vecX.get();
			limX.first = tva::util::getMinElement(providerData);
			auto a=tva::util::getMaxElement(providerData);
			if (a.isSettled())
			{
				//limX.second =a.get()*1.1;
			}
			limX.second = a;
		}
		//
		asY.lim = limY;
		asX.lim = limX;
	}
	style.axStyleX = asX;
	style.axStyleY = asY;
	//
	return style;
}

void tva::postProc::fInitPlot(QWidget* parent, std::shared_ptr<QCustomPlot>& plot)
{
	if (!plot)
	{
		//auto*l=parent->layout();
		//if (l) l->deleteLater();
		//
		plot = std::make_shared<QCustomPlot>();//(/*frame*/);
		QVBoxLayout *verticalLayout = new QVBoxLayout(parent);
		verticalLayout->addWidget(plot.get());
		parent->setLayout(verticalLayout);
	}
};

void tva::postProc::sub::GlobMatrisesToWgt( const tva::globMatrises& gM, QTableWidget* twM, QTableWidget* twK )
{
	if (gM.hasM)
	{
		tva::postProc::MatrToWidgetTable( gM.getM(), twM );
	}

	if (gM.hasK)
	{
		tva::postProc::MatrToWidgetTable( gM.getK(), twK );
	}
}

void tva::postProc::sub::vecOmegaToWidgetTable( const tva::opVecReal& vecOmega, QTableWidget *tw )
{
	static const Real wTof(1.0/(2*M_PI));

	tva::vecvecReal res;
	if (vecOmega.isSettled())
	{
		res.push_back(vecOmega.get());
		const auto& n =  vecOmega.get().size();
		tva::vecReal v;
		for (auto it=0; it<n; ++it)
		{
			v.push_back(vecOmega.get()[it]*wTof);
		}
		res.push_back(v);
		//
		MatrToWidgetTable(res, tw);
	}
	else
	{
		tva::noDataToTable(tw);
	}
}

void tva::postProc::sub::eigenVecsToTable( const tva::Model& model, QTableWidget *tw )
{
	if (model.EigenData.normEigenVecs.isInit())
	{
		tva::postProc::MatrToWidgetTable( model.EigenData.normEigenVecs.get(), tw );
	}
	else
	{
		tva::noDataToTable(tw);
	}
}

void tva::postProc::sub::plotOmega( const tva::Model& model, QCustomPlot* plot )
{
	typedef tva::chartSetup::chartStyle chStyle;
	//
	chStyle style;
	//
	tva::chartSetup::axisStyle asX;
	tva::chartSetup::axisStyle asY;
	asX.title = "index";
	asY.title = "omega";
	{
		tva::chartSetup::opPair limY;
		limY.first = 0;//tva::util::getMinElement(model.Omega);
		limY.second = tva::util::getMaxElement(model.EigenData.Omega.get());
		if (limY.second.isSettled())
		{
			limY.second = limY.second.get()*(1+percentLimitCorrection);
		}

		tva::chartSetup::opPair limX;
		limX.first = 0;//tva::util::getMinElement(model.Omega);
		limX.second = model.EigenData.Omega.get().size()+1;

		asY.lim = limY;
		asX.lim = limX;
	}
	style.axStyleX = asX;
	style.axStyleY = asY;

	//const QCP::ScatterStyle mark = QCP::ScatterStyle::ssCircle;

	if (model.EigenData.Omega.isSettled())
	{
		tva::chartSetup::lineStyle lStyle;
		lStyle.color = QColor(0,85,0);
		//
		//auto p = std::make_pair(, );

		postProc::policyChart policy;
		policy.style = style;
		policy.lStyle = lStyle;
		policy.needClear = true;
		policy.leftAxis = true;
		tva::postProc::vec2Chart( policy, model.EigenData.vecIndex, model.EigenData.Omega.get() , plot);
	}
}

void tva::postProc::sub::plotEigenVecs( const tva::Model& model, QCustomPlot* plot )
{
	typedef tva::chartSetup::axisStyle axStyle;
	if(model.EigenData.normEigenVecs.isInit())
	{
		tva::chartSetup::chartStyle chartSt;
		tva::util::MayBe<axStyle> styleVecs;
		axStyle asX;
		axStyle asY;
		{
			tva::chartSetup::opPair limY;
			const auto& vecs = model.EigenData.normEigenVecs.get();
			const auto minmax=tva::util::getMinMaxElement(vecs);
			//
			limY.first = minmax.min;
			limY.second = minmax.max;
			//
			if (limY.first.isSettled())
			{
				const auto& base=limY.first.get();
				limY.first = base-fabs(base)*(tva::percentLimitCorrection);
			}
			//
			if (limY.second.isSettled())
			{
				const auto& base=limY.second.get();
				limY.second = base+fabs(base)*(tva::percentLimitCorrection);
				//limY.second = limY.second.get()*(1+tva::percentLimitCorrection);
			}

			tva::chartSetup::opPair limX;
			static const int magicInc(1);
			const auto& source=model.rawShaft().descrShaftAuxZCoord();
			if (source.isSettled())
			{
				limX.first = source.get()[0].first;
				limX.second = source.get()[source.get().size()-1].second;
			}
			else
			{
				limX.first = 0;//tva::util::getMinElement(model.Omega);
				limX.second = model.EigenData.Omega.get().size()+magicInc;
			}

			asY.lim = limY;
			asX.lim = limX;
		}
		chartSt.axStyleX = asX;
		chartSt.axStyleY = asY;
		//make additional info for eigenVecsPlot
		tva::opVecOpString addStrings;
		if (model.EigenData.Omega.isSettled())
		{
			const auto&n=model.EigenData.Omega.get().size();
			std::vector<tva::opString> vec;
			for (auto i=0;i<n;++i)
			{
				const double f=model.EigenData.Omega.get()[i]/(2.0*M_PI);
				const QString str=QString(" (%1 Hz)").arg(f, 0, 'f', 3);
				vec.push_back(str);
			}
			addStrings=vec;
		}

		tva::opVecReal vZ;
		{
			tva::vecReal vY;
			const auto& a = model.rawShaft().descrRealZCoord();
			//if (a.isSettled())
			{
				vZ=a.first;
			}
		}
		const auto& vY=model.EigenData.normEigenVecs.get();

		tva::postProc::columnus2Chart( chartSt, vZ, vY, plot, addStrings);
	}
}

void tva::postProc::sub::tableErrEigenProblem( const tva::Model& model, tva::pairTbl& wgtErr, tva::pairpairEdit& wdtMaxErr )
{
	if (model.errEigenProlemM.globM.hasM)
	{
		const auto& M=model.errEigenProlemM.globM.getM();
		tva::postProc::MatrToWidgetTable( M, wgtErr.first);
		//
		tva::proc::pivotElem pivot;
		tva::proc::calcMaxDiagOutDiagElem( M, pivot );
		//
		tva::postProc::sub::doubleToLineEdit( pivot.maxDiag, wdtMaxErr.first.first);
		tva::postProc::sub::doubleToLineEdit( pivot.maxOutDiag, wdtMaxErr.first.second);
	}

	if (model.errEigenProlemM.globM.hasK)
	{
		const auto& K = model.errEigenProlemM.globM.getK();
		tva::postProc::MatrToWidgetTable( K, wgtErr.second);

		tva::proc::pivotElem pivot;
		tva::proc::calcMaxDiagOutDiagElem( K, pivot );

		tva::postProc::sub::doubleToLineEdit( pivot.maxDiag, wdtMaxErr.second.first);
		tva::postProc::sub::doubleToLineEdit( pivot.maxOutDiag, wdtMaxErr.second.second);
	}
}

void tva::postProc::sub::doubleToLineEdit( const const tva::opReal& d, QLineEdit* edit )
{
	if (d.isSettled())
	{
		edit->setText(tva::util::toString(d.get()));
	}
	else
	{
		edit->setText(tva::str::noData);
	}
}

void tva::postProc::ShowModelData( const tva::Model& model, tva::pairpairTbl& MK_EigData, tva::pairPlot& plotOmeVecs, tva::pairTbl& wgtErr, tva::pairpairEdit& wdtMaxErr )
{
	tva::postProc::sub::GlobMatrisesToWgt( model.gM, MK_EigData.first.first, MK_EigData.first.second);
	tva::postProc::sub::vecOmegaToWidgetTable( model.EigenData.Omega, MK_EigData.second.first);
	tva::postProc::sub::eigenVecsToTable( model, MK_EigData.second.second );
	//
	//
	tva::postProc::sub::tableErrEigenProblem( model, wgtErr, wdtMaxErr );
	//
	tva::postProc::sub::plotOmega(model, plotOmeVecs.first);
	tva::postProc::sub::plotEigenVecs( model, plotOmeVecs.second);
}

void tva::postProc::MatrToWidgetTable( const vecvecReal& mat, QTableWidget *tw )
{
	//
	tw->clearContents();
	const int colCount = tva::cols(mat);
	const int rowCount = tva::rows(mat);
	//
	tw->setColumnCount(colCount);
	tw->setRowCount(rowCount);
	//
	for (int i=0; i<rowCount; ++i)
	{
		for(int j=0; j<colCount; ++j)
		{
			const auto& val = mat[j][i];
			tva::dataToTableItem( QString("%1").arg(val), i, j, tw );
		}
	}
}

void tva::postProc::MatrToWidgetTable( const tva::tMatr& matr, QTableWidget *tw )
{
	//
	tw->clearContents();
	const int colCount =matr.countCols();
	const int rowCount = matr.countRows();
	//
	tw->setColumnCount(colCount);
	tw->setRowCount(rowCount);
	//
	for (int i=0; i<rowCount; ++i)
	{
		for(int j=0; j<colCount; ++j)
		{
			const auto& val = matr.getElem(i, j);
			tva::dataToTableItem( QString("%1").arg(val), i, j, tw );
		}
	}
}

void tva::postProc::modes::selected( const std::set<int>& cont, QTableWidget* twOmega, QCustomPlot* plot )
{
	//
	//twOmega->clearSelection();
	//auto *g = plot->graph();
	{//unselect plot
		for (int i=0; i<plot->graphCount(); ++i)
		{
			QCPGraph *graph = plot->graph(i);
			QCPPlottableLegendItem *item = plot->legend->itemWithPlottable(graph);
			//if (item->selected() || graph->selected())
			{
				item->setSelected(false);
				graph->setSelected(false);
			}
		}
	}
	//select table

	//select plot
	{//unselect plot
		for(auto it=cont.begin(); it!=cont.end(); ++it)
		//for (int i=0; i<plotVecs->graphCount(); ++i)
		{
			const int& i = *it;
			if (i<plot->graphCount())
			{
				QCPGraph *graph = plot->graph(i);
				QCPPlottableLegendItem *item = plot->legend->itemWithPlottable(graph);
				//if (item->selected() || graph->selected())
				{
					item->setSelected(true);
					graph->setSelected(true);
				}
			}
		}
	}
	plot->replot();
}