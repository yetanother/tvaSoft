#include "preProc.h"
#include "tvaUtil.h"
#include "..\src\gui\itemviews\qtablewidget.h"
#include "..\src\corelib\tools\qshareddata.h"
//#include "tvaDoc.h"
#include "eigenCore.h"
#include "..\src\corelib\global\qglobal.h"

void tva::preProc::rawShaftDataToWgt( const tva::rawData::descrComplexShaft& descr, tva::tableWrapper *wrTableShaft )
{
	{//shaft
		wrTableShaft->startLoadDataToWidget();
		auto* w=wrTableShaft->getWgt();
		w->clearContents();

		const auto& n = descr.dataShaft().size();
		if (n==0)
		{
			tva::noDataToTable(w);
			wrTableShaft->NoData(true);
		}
		else
		{
			wrTableShaft->NoData(false);
			w->setColumnCount(tva::rawData::descrSingleShaft::countItem);
			w->setRowCount(n);
			for(size_t i=0; i<n; ++i)
			{
				const auto& singleShaft = descr.dataShaft()[i];
				tva::preProc::rawSingleShaftToTable(singleShaft, i, w);
			}
			//
		}
		wrTableShaft->finishLoadDataToWidget();
	}
	//
}

void tva::preProc::rawDiscDataToWgt( const tva::rawData::descrComplexShaft& descr, tva::tableWrapper *wrTableDisc )
{
	{//disc
		wrTableDisc->startLoadDataToWidget();
		auto* w=wrTableDisc->getWgt();
		w->clearContents();

		const auto& n = descr.dataDisc().size();
		if (n==0)
		{
			tva::noDataToTable(w);
			wrTableDisc->NoData(true);
		}
		else
		{
			wrTableDisc->NoData(false);
			w->setColumnCount(tva::rawData::descrSingleDisc::countItem);
			w->setRowCount(n);
			for(size_t i=0; i<n; ++i)
			{
				const auto& singleDisc = descr.dataDisc()[i];
				tva::preProc::rawSingleDiscToTable(singleDisc, i, w);
			}
			//
		}
		wrTableDisc->finishLoadDataToWidget();
	}
}

void tva::preProc::rawSingleShaftToTable( const tva::rawData::descrSingleShaft& single, const int& raw, QTableWidget*wgt )
{
	//
	tva::dataToTableItem( tva::util::toString(single.l_fe), raw, 0, wgt );
	tva::dataToTableItem( tva::util::toString(single.d), raw, 1, wgt );
	tva::dataToTableItem( tva::util::toString(single.E/tva::preProc::PaToGPa), raw, 2, wgt );
	tva::dataToTableItem( tva::util::toString(single.mu), raw, 3, wgt );
	tva::dataToTableItem( tva::util::toString(single.ro), raw, 4, wgt );
	//
}

void tva::preProc::plotShaft( const tva::chartSetup::opChartStyle& style, const tva::rawData::descrComplexShaft& descr, QCustomPlot*plot, bool needClear, bool leftAxis )
{
	//make data for plot shaft
	tva::vecReal ySh;
	tva::opVecReal z;
	if (descr.descrShaftAuxZCoord().isSettled())
	{
		tva::preProc::tuningPlot::makeOnlyShaftPlotData( descr,z, ySh );
	}
	//make data for plot disc
	tva::vecReal zD, yD;
	for(auto i=0;i<descr.dataDisc().size();++i)
	{
		zD.push_back(descr.dataDisc()[i].z);
		yD.push_back(descr.dataDisc()[i].d/2.0);
	}
	if (z.isSettled())
	{
		foreach(auto v, zD)
		{
			z.get().push_back(v);
		}
	}
	else
	{
		z = zD;
	}
	//foreach(auto v, yD)
	//
	tva::chartSetup::opChartStyle loc_style;

	{//make style
		{
			std::vector<tva::vecReal> vecVec;
			vecVec.push_back(yD);
			vecVec.push_back(ySh);
			if (z.isSettled())
			{
				loc_style=tva::preProc::tuningPlot::makeShaftChartStyle(z.get(),vecVec);
			}

			if (loc_style.isSettled() && style.isSettled())
			{
				loc_style.get().axStyleY.multiplicator=style.get().axStyleY.multiplicator;
			}
		}
	}
	bool shaftPlotted(false);
	{//shaft
		if (z.isSettled())
		{
			const auto& lStyle=tva::preProc::tuningPlot::makeShaftLineStyle();

			postProc::policyChart policy;
			policy.style = loc_style;
			policy.lStyle = lStyle;
			policy.needClear = needClear;
			policy.leftAxis = leftAxis;

			tva::postProc::vec2Chart( policy, z.get(), ySh, plot);
			shaftPlotted = true;
		}
	}
	{//disc
		auto discLineStyle=tva::preProc::tuningPlot::makeDiscLineStyle();
		bool shouldClear = !shaftPlotted;
		//
		postProc::policyChart policy;
		policy.style = loc_style;
		policy.lStyle = discLineStyle;
		policy.needClear = shouldClear;
		policy.leftAxis = leftAxis;
		//
		tva::postProc::vec2Chart( policy, zD, yD, plot);
	}
}

void tva::preProc::plotShaftToEigenVecs( const tva::rawData::descrComplexShaft& descr, QCustomPlot*plot, bool needClear, bool leftAxis )
{
	tva::chartSetup::chartStyle style;
	//
	{//get yAxis property
		const auto* xA=plot->yAxis;
		const auto&min=xA->range().minRange;
		const auto&max=xA->range().maxRange;
		const auto& interval=fabs(max-min);
		//
		const auto& maxD = tva::util::getMaxElement(descr.descrRealZCoord().second);
		if (maxD.isSettled())
		{
			const double& mult=interval/(maxD.get()/2.0);
			//
			//qDebug()<<"mult="<<mult<<endl;
			style.axStyleY.multiplicator=20;//mult;
		}
	}
	tva::preProc::plotShaft( style, descr, plot, needClear, leftAxis );
}

void tva::preProc::WgtToRawShaftData( const tva::tableWrapper*wrTableShaft, tva::rawData::descrComplexShaft& descr )
{
	std::vector<tva::rawData::descrSingleShaft> res;
	//
	if (wrTableShaft->NoData())
	{
		descr.dataShaft().clear();
		return;
	}
	//
	auto*wgtShaft=wrTableShaft->getWgt();
	const auto& n = wgtShaft->rowCount();
	if (n==0)
	{
		return;
	}
	else
	{
		for(int i=0; i<n; ++i)
		{
			tva::rawData::descrSingleShaft single;
			tva::preProc::TableRowToSingleShaft(wgtShaft, i, single);
			res.push_back(single);
		}
		//
		descr.setDataShaft(res);
	}
}

void tva::preProc::WgtToRawDiscData( const tva::tableWrapper*wrTableDisc, tva::rawData::descrComplexShaft& descr )
{
	//tva::util::showMsg("WgtToRawDiscData");
	std::vector<tva::rawData::descrSingleDisc> res;
	//
	if (wrTableDisc->NoData())
	{
		descr.dataDisc().clear();
		return;
	}
	//
	auto*wgtDisc=wrTableDisc->getWgt();
	const auto& n = wgtDisc->rowCount();
	if (n==0)
	{
		return;
	}
	else
	{
		for(size_t i=0; i<n; ++i)
		{
			tva::rawData::descrSingleDisc single;
			tva::preProc::TableRowToSingleDisc(wgtDisc, i, single);
			res.push_back(single);
		}
		//
		descr.setDataDisc(res);
	}
}

void tva::preProc::TableRowToSingleShaft( const QTableWidget*wgt, const int& indexRow, tva::rawData::descrSingleShaft& single )
{
	//check correct index row
	const auto& l = tva::TableItemToDouble(wgt, indexRow, 0);
	if (l.isSettled())
	{
		single.l_fe = l.get();
	}
	//
	const auto& d = tva::TableItemToDouble(wgt, indexRow, 1);
	if (d.isSettled())
	{
		single.d = d.get();
	}
	//
	const auto& E = tva::TableItemToDouble(wgt, indexRow, 2);
	if (E.isSettled())
	{
		single.E = E.get()*tva::preProc::PaToGPa;
	}
	const auto& mu = tva::TableItemToDouble(wgt, indexRow, 3);
	if (mu.isSettled())
	{
		single.mu = mu.get();
	}
	//
	const auto& ro = tva::TableItemToDouble(wgt, indexRow, 4);
	if (ro.isSettled())
	{
		single.ro = ro.get();
	}
	//
}

void tva::preProc::TableRowToSingleDisc( const QTableWidget*wgt, const int& indexRow, tva::rawData::descrSingleDisc& single )
{
	//check correct index row
	const auto& z = tva::TableItemToDouble(wgt, indexRow, 0);
	if (z.isSettled())
	{
		single.z = z.get();
	}
	//
	const auto& Iz = tva::TableItemToDouble(wgt, indexRow, 1);
	if (Iz.isSettled())
	{
		single.Iz = Iz.get();
	}
	//
	const auto& d = tva::TableItemToDouble(wgt, indexRow, 2);
	if (d.isSettled())
	{
		single.d = d.get();//*tva::preProc::PaToGPa;
	}

	//
}

void tva::preProc::rawSingleDiscToTable( const tva::rawData::descrSingleDisc& single, const int& raw, QTableWidget*wgt )
{
	//
	tva::dataToTableItem( tva::util::toString(single.z), raw, 0, wgt );
	tva::dataToTableItem( tva::util::toString(single.Iz), raw, 1, wgt );
	tva::dataToTableItem( tva::util::toString(single.d), raw, 2, wgt );
	//
}

void tva::preProc::funcDoMakeGlobMatr( const tva::preProc::sensParam& sensP, const std::vector<tva::rawData::descrSingleShaft>& descrs_, tva::tMatr& M, tva::tMatr& K, bool& hasM, bool& hasK, double& dx )
{
	//auto descrForCorrection = descrs;
	//
	auto valDescr = descrs_;
	// correction if need to
	if (sensP.needTo())
	{
		auto& param = valDescr[sensP.indexVarForDev()].l_fe;//stub
		dx = param/100.0;//stub
		param += dx;
	}
	//
	std::vector<MK_rawData> vecRawMIP;
	tva::dataShaftToRawMIP_Data( valDescr, vecRawMIP);
	//
	//make vecForMatrM
	tva::vecReal vecI;
	tva::vecReal vecC;
	//
	auto size=vecRawMIP.size();
	if (size>0)
	{
		foreach(auto& it, vecRawMIP)
		{
			vecI.push_back(it.Iz);
			vecC.push_back(it.c);
		}
	}
	else
	{
		Q_ASSERT_X(size>0,"error", "count shaft=0");
		//for (auto i=0; i<2; ++i)
		//{
		//	vecI.push_back(vecRawMIP[0].Iz);
		//	vecC.push_back(vecRawMIP[0].c);
		//}
	}

	//

	{//make GlobMAtr due shaft
		auto makeGlobMatrDueShaft=[](tva::functor_Real_Matr &funcMakeLocalMatr,
			const tva::vecReal vecRaw,
			tva::tMatr& globMatr, bool &hasSet)
		{
			tva::tMatr pretend;
			tva::stub::makeRegularMatrByVectorValue(funcMakeLocalMatr, vecRaw, pretend);
			globMatr=pretend;
			hasSet=true;
		};
		//
		tva::functor_Real_Matr funcMakeMLocal = &tva::local::makeM;
		makeGlobMatrDueShaft(funcMakeMLocal, vecI, M, hasM);
		//
		tva::functor_Real_Matr funcMakeKLocal = &tva::local::makeK;
		makeGlobMatrDueShaft(funcMakeKLocal, vecC, K, hasK);
	}

	//{// add disc
	//	tva::vecReal vecIndexForPutDisc;
	//	foreach(auto disc, model.rawShaft().dataDisc())
	//	{
	//		const auto&z= disc.z;
	//		const auto&Iz=disc.Iz;
	//	}
	//}
}

void tva::rawData::descrComplexShaft::makeZDataViaRawData()
{
	if (descrShaftAuxZCoord().isSettled())
	{
		descrShaftAuxZCoord().get().clear();
	}
	//if (descrRealZCoord().isSettled())
	{
		//descrRealZCoord().get().clear();
		descrRealZCoord().first.clear();
		descrRealZCoord().second.clear();
	}
	//
	const size_t& n = _dataShaft.size();
	double buf(0.0);
	double buf_next(buf);
	vecPairDouble res;
	//
	tva::pairVecReal pretenderZReal;//z,y-Real
	pretenderZReal.first.push_back(0);
	pretenderZReal.second.push_back(_dataShaft[0].d);
	//
	for(size_t i=0; i<n-1; ++i)
	{
		buf_next = buf+_dataShaft[i].l_fe;
		pretenderZReal.first.push_back(buf_next);
		pretenderZReal.second.push_back(_dataShaft[i].d);
		pairDouble pair(buf, buf_next);
		res.push_back(pair);
		//
		std::swap(buf, buf_next);
	}
	pairDouble pair(buf, buf+_dataShaft[n-1].l_fe);
	pretenderZReal.first.push_back(pair.second);
	pretenderZReal.second.push_back(_dataShaft[n-1].d);
	res.push_back(pair);
	//
	if (n>0)
	{
		descrShaftAuxZCoord() = res;
		descrRealZCoord() = pretenderZReal;
	}
}

tva::chartSetup::lineStyle tva::preProc::tuningPlot::makeShaftLineStyle()
{
	tva::chartSetup::lineStyle lStyle;
	//
	lStyle.scatter = QCP::/*ScatterStyle::*/ssNone;
	lStyle.color = tva::chartSetup::defColor;
	//
	return lStyle;
}

tva::chartSetup::lineStyle tva::preProc::tuningPlot::makeDiscLineStyle()
{
	tva::chartSetup::lineStyle lStyle;
	//
	lStyle.style = QCPGraph::lsImpulse;
	lStyle.scatter = QCP::/*ScatterStyle::*/ssCircle;
	lStyle.color = tva::chartSetup::defColorDisc;
	//
	return lStyle;
}

tva::chartSetup::chartStyle tva::preProc::tuningPlot::makeShaftChartStyle( const tva::vecReal& vX,const std::vector<tva::vecReal>& vY )
{
	tva::chartSetup::chartStyle style;
	//
	tva::chartSetup::axisStyle asX;
	tva::chartSetup::axisStyle asY;
	asX.title = "z (m)";
	asY.title = "r (m)";
	{
		tva::chartSetup::opPair limY;
		//limY.first = 0;//tva::util::getMinElement(y);
		tva::vecReal vecMin;
		for(auto i=0;i<vY.size();++i)
		{
			const auto&a=tva::util::getMinElement(vY[i]);
			if (a.isSettled())
			{
				vecMin.push_back(a.get());
			}
		}
		limY.first = tva::util::getMinElement(vecMin);;
		//
		tva::vecReal vecMax;
		for(auto i=0;i<vY.size();++i)
		{
			const auto&a=tva::util::getMaxElement(vY[i]);
			if (a.isSettled())
			{
				vecMax.push_back(a.get());
			}
		}
		//
		limY.second = tva::util::getMaxElement(vecMax);
		if (limY.second.isSettled())
		{
			limY.second = limY.second.get()*(1+tva::percentLimitCorrection);
		}

		tva::chartSetup::opPair limX;
		limX.first = tva::util::getMinElement(vX);
		limX.second = tva::util::getMaxElement(vX);
		opReal Length;
		if (limX.first.isSettled() && limX.second.isSettled())
		{
			Length = limX.second.get()-limX.first.get();
		}
		if (limX.second.isSettled())
		{
			if (Length.isSettled())
			{
				limX.first = limX.first.get()-Length.get()*tva::percentLimitCorrection;
				limX.second = limX.second.get()+Length.get()*tva::percentLimitCorrection;
			}
			else
			{
				limX.second = limX.second.get()*(1+tva::percentLimitCorrection);
			}
			//limX.first = -0.5;
		}

		asY.lim = limY;
		asX.lim = limX;
	}
	style.axStyleX = asX;
	style.axStyleY = asY;
	//
	return style;
}

void tva::preProc::tuningPlot::makeOnlyShaftPlotData( const tva::rawData::descrComplexShaft& descr, tva::opVecReal& z, tva::vecReal&y )
{
	if (!descr.descrShaftAuxZCoord().isSettled())
	{
		return;
	}
	{//shaft
		const auto& a = descr.descrShaftAuxZCoord().get();
		//
		static const double percentValue(0.1);
		tva::vecReal vzAux;

		const auto& an=descr.dataShaft().size();
		for (size_t i=0; i<an ; ++i)
		{
			//  трапеция
			const auto& delta = descr.dataShaft()[i].l_fe*percentValue/100.0;
			//
			vzAux.push_back(a[i].first);
			vzAux.push_back(a[i].first+delta);
			vzAux.push_back(a[i].second-delta);
			vzAux.push_back(a[i].second);
			//
			const auto& r = descr.dataShaft()[i].d/2.0;
			y.push_back(0);
			y.push_back(r);
			y.push_back(r);
			y.push_back(0);
		}
		z=vzAux;
	}
}

void tva::dataShaftToRawMIP_Data( const std::vector<rawData::descrSingleShaft>& shaft, std::vector<tva::MK_rawData>& res )
{
	res.clear();
	//
	const auto&n = shaft.size();
	for (int i=0;i<n; ++i)
	{
		const auto& singleShaft=shaft[i];
		//
		const auto S = tva::preProc::fCalc_A(singleShaft);//M_PI*d2/4.0;
		const auto& Iz=tva::preProc::fCalc_Iz(singleShaft);//M_PI*l_fe*ro*d4/32.0;
		//
		const auto& l_fe=singleShaft.l_fe/1.0;
		const auto&d=singleShaft.d;
		const auto&d2=d*d;
		const auto&d4=d2*d2;
		const auto& ro=singleShaft.ro;
		//
		const auto& m = S*l_fe*ro;
		//
		const auto& Ip=tva::preProc::fCalc_Ip(singleShaft);
		const auto& E=singleShaft.E;
		const auto& mu=singleShaft.mu;
		const auto& G=E/(2*(1.0+mu));
		const auto&c=G*Ip/l_fe;
		//
		res.push_back(MK_rawData(m,Iz,c));
	}
}