#include "tvaDoc.h"
#include "..\..\slnProc\prjEigenCore\eigenCore.h"
#include "..\src\corelib\io\qfileinfo.h"
#include "..\src\corelib\kernel\qmath.h"
#include "..\..\slnPostProc\postProc.h"
#include <set>
#include "wizards.h"
#include "tvaUtil.h"

tva::Doc::Doc( const tva::Shell& shell_, QWidget *parent /*= 0*/ )
	:QWidget(parent), shell(shell_),plotVecs3(nullptr),plotOmega3(nullptr)
	,plotShaft(nullptr), plotStaticTask(nullptr), plotRightEdge(nullptr), plotDynProfile(nullptr)
{
	setupUi(this);
	//
	wrRawShaftData = std::shared_ptr<tva::tableWrapper>(new tva::tableWrapper(twShaft));
	wrRawDiscData = std::shared_ptr<tva::tableWrapper>(new tva::tableWrapper(twDisc));
	//
	initConnect();
	//
	tva::postProc::fInitPlot(frameOmega, plotOmega3);
	tva::postProc::fInitPlot(frameVecs, plotVecs3);
	tva::postProc::fInitPlot(framePlotShaft, plotShaft);
	tva::postProc::fInitPlot(frame_5, plotStaticTask);
	tva::postProc::fInitPlot(groupBox, plotRightEdge);
	tva::postProc::fInitPlot(groupBox_7, plotDynProfile);
	//
	tva::postProc::fInitPlot(fU, plotU_dbg);

	msTorsional = std::make_shared<wrModeSelector>(twOmega, plotVecs3.get());
	msBending = std::make_shared<wrModeSelector>(t25, plotU_dbg.get());

	newFile();
}

void tva::Doc::slMakeMatrises()
{
	const int currentTab=0;//tabWidgetDataShaft->currentIndex();
	if (currentTab==0)
	{
		//tva::util::showMsg("shaft Data");
		//
		{
			bool hasM(false);
			bool hasK(false);
			const auto& dataForMakeRawMIP = model.rawShaft().dataShaft();
			//funcDoMakeGlobMatr(tva::preProc::sensParam(),dataForMakeRawMIP, model.gM.M(), model.gM.K(),hasM, hasK);
			double stubDx(-1.0);
			tva::preProc::funcDoMakeGlobMatr(tva::preProc::sensParam(),dataForMakeRawMIP, model.gM.M(), model.gM.K(),hasM, hasK, stubDx);
			model.gM.hasM = hasM;
			model.gM.hasK = hasK;
		}
		//
		model.rawShaft().makeZDataViaRawData();
	}
	//}
	else //get regular simple-raw data source
	{
		tva::util::showMsg("stub RegularData");
		//const auto n = sbCountDisc->value();
		//const auto mipI = sbMomemtInertia->value();
		//const auto strengh = sbStrengh->value();
		////
		//tva::globMatrises &gM = model.gM;
		//tva::stub::regularData regData(n, mipI, strengh);
		//tva::stub::makeRegularMatrises( regData, gM );
		////
		//shell.addLineToLogger("calc M and K");
	}
}

void tva::Doc::initConnect()
{
	{
		bool flag = connect(btnEigenCalc, SIGNAL(released ()), this, SLOT(slMakeEigenData()));
		if(!flag)
			tva::util::showMsg("error on connection to slot btnEigenCalc");
	}

	{
		bool flag = connect(
			btnWizShaft, SIGNAL(released())
			, this, SLOT(slWizardShaft())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot btnWizShaft");
	}
	{
		bool flag = connect(
			btnWizDisc, SIGNAL(released())
			, this, SLOT(slWizardDisc())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot btnWizDisc");
	}
	{
		bool flag = connect(
			this, SIGNAL(rawShaftDataChanged())
			, this, SLOT(slUpdateRawShaftData())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slUpdateRawShaftData");
	}
	{
		bool flag = connect(
			this, SIGNAL(rawDiscDataChanged())
			, this, SLOT(slUpdateRawDiscData())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slUpdateRawShaftData");
	}

	{
		bool flag = connect(
			wrRawShaftData.get(), SIGNAL(sigWidgetEdited())
			, this, SLOT(slTableShaftToModel())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slTableShaftToModel");
	}
	{
		bool flag = connect(
			wrRawDiscData.get(), SIGNAL(sigWidgetEdited())
			, this, SLOT(slTableDiscToModel())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slTableDiscToModel");
	}
	//
	{
		bool flag = connect(
			btnSensCalc, SIGNAL(released())
			, this, SLOT(slSensAnalisys())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slSensAnalisys");
	}
	{
		bool flag = connect(
			btnStaticTask, SIGNAL(released())
			, this, SLOT(slSolveStaticTask())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slSensAnalisys");
	}
	{
		bool flag = connect(
			btnDynTask, SIGNAL(released())
			, this, SLOT(slSolveDynamicTask())
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slSolveDynamicTask");
	}
	{
		bool flag = connect(
			dynSlider, SIGNAL(valueChanged ( int ))
			, this, SLOT(slShowDynStep(int))
			);
		if(!flag)
			tva::util::showMsg("error on connection to slot slShowDynStep");
	}
}

void tva::Doc::slMakeEigenData()
{
	{//Euler-Bernulli
		//
		//tva::tMatr out;
		bool useTimoshCorrection=false;
		tva::vecReal vE, vI, vl, vMu, vA, v_ro;

		const auto& dataForMakeRawMIP = model.rawShaft().dataShaft();
		const int n=dataForMakeRawMIP.size();

		for (int i=0; i<n; ++i)
		{
			const auto& prov = dataForMakeRawMIP[i];
			const auto& E = prov.E;
			const auto& I = tva::preProc::fCalc_Iz(prov);
			const auto& l = prov.l_fe;
			const auto& mu = prov.mu;
			const auto& A = tva::preProc::fCalc_A(prov);
			const auto& ro = prov.ro;

			 vE.push_back(E);
			 vI.push_back(I);
			 vl.push_back(l);
			 vMu.push_back(mu);
			 vA.push_back(A);
			 v_ro.push_back(ro);
		}

		tva::tMatr Kb, Mb;
		tva::e_b::makeGlobMatr_K<tva::e_b::policy>(vE, vI, vl, false, vMu, vA, Kb);
		tva::e_b::makeGlobMatr_M<tva::e_b::policy>(vA, vl, v_ro, vI, Mb);
		//
		tva::vecReal Omega;
		tva::vecvecReal normVecs;
		bool res = doSolveEigenProblem(Mb, Kb, Omega, normVecs);
		tva::postProc::sub::vecOmegaToWidgetTable(Omega, t25);
		tva::postProc::MatrToWidgetTable(Kb, tb26);
		{
			//get u, v
			tva::vecReal vecU, vecV;//, vZ;
			tva::vecvecReal vvU, vvV;
			for (int i=0; i<normVecs.size(); ++i)
			{
				tva::e_b::post::get_U<tva::e_b::policy>(normVecs[i], vecU);
				vvU.push_back(vecU);
				//
				tva::e_b::post::get_V<tva::e_b::policy>(normVecs[i], vecV);
				vvV.push_back(vecV);
				//
				//vZ.push_back(i);
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
			//
			//tva::opVecReal opZ;
			tva::chartSetup::chartStyle chSt = preProc::tuningPlot::makeShaftChartStyle(vZ.get(), vvU);
			tva::postProc::columnus2Chart(chSt, vZ, vvU, plotU_dbg.get());
			tva::postProc::MatrToWidgetTable(tva::tMatr(vvU), tb26);
			//tva::postProc::columnus2Chart(tva::chartSetup::chartStyle(), opZ, vvV, fV);
		}
		//
	}

	slMakeMatrises();
	//
	if (!model.gM.hasM)
	{
		tva::util::showMsg("matr M not ready");
		return;
	}
	if (!model.gM.hasK)
	{
		tva::util::showMsg("matr K not ready");
		return;
	}
	{//calc
		tva::vecReal Omega;
		tva::vecvecReal normVecs;
		bool res= tva::doSolveEigenProblem(model.gM.getM(), model.gM.getK(), Omega, normVecs);
		if (res)
		{
			const auto& n=Omega.size();
			model.EigenData.Omega = Omega;
			model.EigenData.vecIndex= tva::makeInterval(0,n);
			//
			model.EigenData.normEigenVecs.setData(normVecs);
			//
			model.calcErrorEigenProblem( model.EigenData.Omega.get(), model.errEigenProlemM);
			//
			shell.addLineToLogger("solve eigenValueProblem");
		}
	}
	//}
	tva::pairEdit pE_M(errMaxDiagByM, errMaxOutDiagByM);
	tva::pairEdit pE_K(errMaxDiagByK, errMaxOutDiagByK);
	//
	tva::postProc::ShowModelData
		(
		model
		, tva::pairpairTbl(tva::pairTbl(twM, twK),tva::pairTbl(twOmega, twEigenVecs))
		, tva::pairPlot(plotOmega3.get(), plotVecs3.get())
		, tva::pairTbl(twErrM, twErrK)
		, tva::pairpairEdit(pE_M, pE_K)
		);
	//
	tva::chartSetup::chartStyle style;

	tva::preProc::plotShaftToEigenVecs( model.rawShaft(), plotVecs3.get(), false, false);
	slUpdatePlotComplexShaft();
}

void tva::Doc::show()
{
	//slMakeEigenData();
	//
	return QWidget::show();
}

QString tva::Doc::userFriendlyCurrentFile()
{
	return strippedName(curFile);
}

QString tva::Doc::strippedName( const QString &fullFileName )
{
	return QFileInfo(fullFileName).fileName();
}

void tva::Doc::newFile()
{
	static int sequenceNumber = 1;

	isUntitled = true;
	curFile = tr("document%1.txt").arg(sequenceNumber++);
	setWindowTitle(curFile + "[*]");

	//slUpdateRawShaftData();
}

void tva::Doc::slOmegaSelectionChanged()
{
	const auto& selectedOj = twOmega->selectedItems();
	//
	std::set<int> selectedRows;
	for (auto it = selectedOj.begin(); it!=selectedOj.end(); ++it)
	{
		selectedRows.insert((*it)->row());
	}
	//unselect chart
	tva::postProc::modes::selected( selectedRows, twOmega, plotVecs3.get());
}

void tva::Doc::slWizardShaft()
{
	tva::wizz::EditShaft* dlg = tva::wizz::getDlgEditShaft(this);
	tva::wizz::dataEditShaft data;
	int res = dlg->exec(data);
	if (res==QDialog::Accepted)
	{
		//make deal
		if (data.stateData)
		{
			//filfull rawdata into model
			tva::rawData::descrSingleShaft descr(data);
			//
			std::vector<tva::rawData::descrSingleShaft> a;
			for (auto i=0;i<data.N; ++i)
			{
				a.push_back(descr);
			}
			//
			model.rawShaft().setDataShaft(a);
			emit rawShaftDataChanged();
			//
		}
	}
}

void tva::Doc::slUpdateRawShaftData()
{
	tva::preProc::rawShaftDataToWgt(model.rawShaft(), wrRawShaftData.get());
	slUpdatePlotComplexShaft();
}

void tva::Doc::slUpdateRawDiscData()
{
	tva::preProc::rawDiscDataToWgt(model.rawShaft(), wrRawDiscData.get());
	slUpdatePlotComplexShaft();
}

void tva::Doc::slUpdatePlotComplexShaft()
{
	tva::preProc::plotShaft( tva::chartSetup::opChartStyle(),model.rawShaft(), plotShaft.get(), true,true );
	//
	{
		tva::chartSetup::opLineStyle lStyle;
		tva::pairVecReal vx_vy;
		tva::vecReal z,y;
		const auto& aPair=model.rawShaft().descrRealZCoord();
		z=aPair.first;
		y=aPair.second;
		//
		tva::chartSetup::chartStyle style;
		//
		tva::chartSetup::axisStyle asX;
		tva::chartSetup::axisStyle asY;
		{
			{
				tva::chartSetup::opPair limY;
				limY.first = 0;//tva::util::getMinElement(y);
				//
				//
				limY.second = tva::util::getMaxElement(y);
				if (limY.second.isSettled())
				{
					const auto length=limY.second.get()-limY.first.get();
					limY.first.get() -= length*tva::percentLimitCorrection;
					limY.second = length*(1+tva::percentLimitCorrection);
				}
				asY.lim = limY;
				//asX.lim = limX;
			}
			style.axStyleY = asY;
		}
		//tva::postProc::vec2Chart(style, lStyle, std::make_pair(z,y), plotVecs3, false, false);
		//tva::preProc::plotShaft( model.rawShaft(), plotVecs3, _false, false );
	}
}

void tva::Doc::slTableShaftToModel()
{
	tva::rawData::descrComplexShaft descr;
	tva::preProc::WgtToRawShaftData(wrRawShaftData.get(), descr);
	model.rawShaft().setDataShaft( descr.dataShaft() );
	//
	slUpdatePlotComplexShaft();
}

void tva::Doc::slWizardDisc()
{
	auto* dlg = tva::wizz::getDlgEditDisc(this);
	tva::wizz::dataEditDisc data;
	int res = dlg->exec(data);
	if (res==QDialog::Accepted)
	{
		//make deal
		if (data.stateData)
		{
			//filfull rawdata into model
			const auto&n=data.N;
			double step(0.0);
			if (data.constLocalLength)
			{
				step=data.length;
			}
			else
			{
				step=data.length/double(n);
			}
			//
			std::vector<tva::rawData::descrSingleDisc> a;
			//
			for(auto i=0;i<n;++i)
			{
				double zi=data.z+i*step;
				//
				tva::rawData::descrSingleDisc descr;
				descr.z=zi;
				descr.Iz=data.Iz;
				descr.d=data.d;
				//
				a.push_back(descr);
			}
			//
			model.rawShaft().setDataDisc(a);
			emit rawDiscDataChanged();
			//
		}
	}
}

void tva::Doc::slTableDiscToModel()
{
	tva::rawData::descrComplexShaft descr;
	tva::preProc::WgtToRawDiscData(wrRawDiscData.get(), descr);
	model.rawShaft().setDataDisc( descr.dataDisc() );
	//
	slUpdatePlotComplexShaft();
}

void tva::Doc::slSensAnalisys()
{
	//tva::util::showMsg("slSensAnalisys");
	//get rawSourceData;
	tva::tMatr M,K;
	{
		bool hasM(false);
		bool hasK(false);
		const auto& dataForMakeRawMIP = model.rawShaft().dataShaft();
		double stubDx(-1.0);
		tva::preProc::funcDoMakeGlobMatr(tva::preProc::sensParam(),dataForMakeRawMIP, M, K,hasM, hasK, stubDx);
		//
		Q_ASSERT_X(hasM && hasK, "error", "calculation eigenStartPosition");
	}
	//
	tva::vecReal Omega;
	tva::vecvecReal normVecs;
	{
		bool res= tva::doSolveEigenProblem(M, K, Omega, normVecs);
	}
	{// calc matrix deviation
		int countDevParam=model.rawShaft().dataShaft().size();//stub
		const auto& dataForMakeRawMIP = model.rawShaft().dataShaft();
		const int stubTypeIndex=1;
		const double deviationPercent(1.0);
		//const double dx=0.1;//stub
		//
		tva::tMatr tmpMatr;
		int m=Omega.size();
		tmpMatr.resize(m, countDevParam);
		//
		for (int j=0; j<countDevParam; ++j)
		{
			// calc current devParam //calc globMatr
			tva::tMatr M2,K2;
			bool hasM(false);
			bool hasK(false);
			double dx(0);
			tva::preProc::funcDoMakeGlobMatr
				(
					tva::preProc::sensParam(stubTypeIndex, deviationPercent, j),
					dataForMakeRawMIP,
					M2, K2,
					hasM, hasK,
					dx
				);
			//calc derivative
			auto funcCalcDerivativeMatrix=[](const tva::tMatr& M0, const tva::tMatr& M1, const double& dx, tva::tMatr& derivM)
			{
				derivM = (M1-M0);
				derivM *= 1.0/dx;
				//
			};
			//
			tva::tMatr derM, derK;
			funcCalcDerivativeMatrix(M,M2, dx, derM);
			funcCalcDerivativeMatrix(K,K2, dx, derK);
			//
			//calc Sens

			for(auto i=0; i<m; ++i)
			{
				double lambda=Omega[i]*Omega[i];
				tva::tMatr derMlocal = derM;
				derMlocal *= lambda;
				tva::tMatr dev=derK-derMlocal;
			//	//
				auto dLambda_i_dxj=tva::getDoubleOuterMult(normVecs[i], dev, normVecs[i]);
				auto normMult=tva::getDoubleOuterMult(normVecs[i], M, normVecs[i]);
				tmpMatr.getElem(i,j)=dLambda_i_dxj/normMult;
			}
		}
		//debug
		tva::postProc::MatrToWidgetTable(tmpMatr, tableWidget_22);
	}
}

void tva::Doc::slSolveStaticTask()
{
	//tva::util::showMsg("slSolveStaticTask");
	slMakeMatrises();
	//
	//if (!model.gM.hasM)
	//{
	//	tva::util::showMsg("matr M not ready");
	//	return;
	//}
	if (!model.gM.hasK)
	{
		tva::util::showMsg("matr K not ready");
		return;
	}
	auto K2=model.gM.getK();
	{//correction K
		static const double multStuck=100.0;
		K2.getElem(0,0) *= multStuck;
		//K2.getElem(0,1) *= multStuck;
		//K2.getElem(1,0) *= multStuck;
	}
	//make F_outer
	tva::tMatr F_outer;
	F_outer.resize(K2.countRows(),1);
	{
		static const double F_r=-1.0;
		F_outer.fill(0.0);
		F_outer.getElem(K2.countRows()-1,0) = F_r;
	}
	//solve
	auto b=K2.getSolve(F_outer);
	//
	tva::postProc::MatrToWidgetTable(b, tableWidget_23);
	//extract vector
	std::vector<double> stdX,stdB;
	for(int i=0; i<b.countRows(); ++i)
	{
		stdX.push_back(i);
		stdB.push_back(b.getElem(i,0));
	}
	//
	//auto p = std::make_pair(stdX, stdB);
	{//plot staticTask
		typedef tva::chartSetup::chartStyle chStyle;
		//
		chStyle style;
		//
		tva::chartSetup::axisStyle asX;
		tva::chartSetup::axisStyle asY;
		asX.title = "index";
		asY.title = "staticDef";
		{
			tva::chartSetup::opPair limY;
			const auto& providerData = stdB;
			limY.first =tva::util::getMinElement(stdB);
			limY.second = tva::util::getMaxElement(stdB);
			if (limY.second.isSettled())
			{
				//limY.second = limY.second.get()*(1+percentLimitCorrection);
			}

			tva::chartSetup::opPair limX;
			limX.first = 0;//tva::util::getMinElement(model.Omega);
			limX.second = stdX.size()+1;

			asY.lim = limY;
			asX.lim = limX;
		}
		style.axStyleX = asX;
		style.axStyleY = asY;
		//
		//tva::chartSetup::opChartStyle loc_style;
		//
		tva::chartSetup::lineStyle lStyle;
		lStyle.color = QColor(0,85,0);
		//
		bool needClear(true);
		bool leftAxis(true);
		postProc::policyChart policy;
		policy.style = style;
		policy.lStyle = lStyle;
		policy.needClear = needClear;
		policy.leftAxis = leftAxis;
		//
		tva::postProc::vec2Chart( policy, stdX, stdB, plotStaticTask.get() );
	}
}

void tva::Doc::slSolveDynamicTask()
{
	slMakeMatrises();
	//
	{
		//get data from form
		double alfa = this->wg_alfa->value();
		double beta=this->wg_beta->value();

		double dt=this->wg_dt->value();
		double t0=this->wg_t0->value();
		double tk=this->wg_tk->value();
		//
		// correct matr;
		auto K2=model.gM.getK();
		auto M2=model.gM.getM();
		//
		{//correction K,M
			/*static*/ const double multStuck=1000.0;
			//
			auto funcMartCorrection=[&multStuck](tva::tMatr& M)
			{
				M.getElem(0,0) *= multStuck;
				//M.getElem(0,1) *= multStuck;
				//M.getElem(1,0) *= multStuck;
			};
			//
			funcMartCorrection(K2);
			funcMartCorrection(M2);
			//tva::postProc::MatrToWidgetTable(K2, tableWidget_25);
		}

		//make matrC
		auto C = proto::makeC(M2, K2, alfa, beta);
		// init model dyn data
		model.dynData.initData_.M = M2;
		model.dynData.initData_.C = C;
		model.dynData.initData_.K = K2;
		//
		model.dynData.initData_.t0 = t0;
		model.dynData.initData_.dt = dt;
		model.dynData.initData_.tk = tk;
		//make startCondition
		auto& n=model.dynData.initData_.K.countCols();
		tva::vecReal fi0; fi0.resize(n);
		std::fill(fi0.begin(), fi0.end(), 0.0);
		auto fit0=fi0;
		//
		model.dynData.initData_.y0 = fi0;
		model.dynData.initData_.yt0 = fit0;
	}

	const int Nstep = (model.dynData.initData_.tk-model.dynData.initData_.t0)/model.dynData.initData_.dt+1;

	//
	{
		tva::vecvecReal& vecZ = model.dynData.result_.vy;
		tva::vecvecReal& vecZt = model.dynData.result_.vyt;;
		tva::vecvecReal& vecZt2 = model.dynData.result_.vy2t;
		tva::vecReal &vec_t = model.dynData.result_.vTime;
		//
		proto::calc
			(
			model.dynData.initData_.M,
			model.dynData.initData_.C,
			model.dynData.initData_.K,
			model.dynData.initData_.y0,
			model.dynData.initData_.yt0,
			model.dynData.initData_.t0,
			model.dynData.initData_.dt,
			Nstep,
			//proto::fCalcRightSide,
			//nullptr,
			vecZ, vecZt, vecZt2,
			vec_t
			);
	}

	auto fShowRightEdge=[](const tva::Model& model, QCustomPlot* plotRightEdge)
	{
		// make single right edge
		std::vector<double> stdX,stdB;
		const auto& vec_t = model.dynData.result_.vTime;
		auto stepCount=vec_t.size();
		const auto &vecZ = model.dynData.result_.vy;

		auto indexRightEdge = vecZ[0].size()-1;
		for(int i=0; i<stepCount; ++i)
		{
			stdX.push_back(vec_t[i]);
			stdB.push_back(vecZ[i][indexRightEdge]);
		}
		//
		//auto p = std::make_pair(stdX, stdB);
		auto style=tva::postProc::fMakeTypicalStyle(stdX, stdB);
		style.axStyleX.title = "index";
		style.axStyleY.title = "right edge";
		//
		const auto& lStyle=tva::preProc::tuningPlot::makeShaftLineStyle();
		//
		postProc::policyChart policy;
		policy.style = style;
		policy.lStyle = lStyle;
		policy.needClear = true;
		policy.leftAxis = true;
		tva::postProc::vec2Chart( policy, stdX, stdB, plotRightEdge);
		//
		//tva::postProc::MatrToWidgetTable(bK, tableWidget_25);
		//
	};

	fShowRightEdge(model, plotRightEdge.get());
}

void tva::Doc::slShowDynStep( int value )
{
	util::showMsg("slShowDynStep", this);
}