#pragma once
#include "wizards.h"
#include "tvaUtil.h"
//#include "tvaDoc.h"
#include "qobject.h"
#include "qtablewidget.h"
#include "qtUtils.h"
#include "tvaMatrix.h"
#include "..\prj_tChart\chartSetup.h"

class QTableWidget;
class QCustomPlot;

namespace tva
{
	class tableWrapper:public QWidget
	{
		Q_OBJECT
	public:
		explicit tableWrapper(QTableWidget*w)
			:_wgt(w),_noData(false)
		{
			//doConnect();
		}

		bool NoData() const { return _noData; }
		void NoData(bool val) { _noData = val; }

		const QTableWidget* getWgt()const{return _wgt;}
		QTableWidget* getWgt(){return _wgt;}

		void startLoadDataToWidget(){doDisconnect();}
		void finishLoadDataToWidget(){doConnect();}

signals:
		void sigWidgetEdited();
		private slots:
			void slWidgetEdited()
			{
				//tva::util::showMsg("slWidgetEdited");
				emit sigWidgetEdited();
			}

	private:
		QTableWidget *_wgt;
		bool _noData;

		void doConnect()
		{
			bool flag = QObject::connect
				(_wgt, SIGNAL(itemChanged ( QTableWidgetItem * /*item*/ ))
				,this, SLOT( slWidgetEdited() )
				,Qt::UniqueConnection
				);

			if(!flag)
				tva::util::showMsg("error on connection to slot btnEigenCalc");
		}
		void doDisconnect()
		{
			bool flag = QObject::disconnect
				(_wgt, SIGNAL(itemChanged ( QTableWidgetItem * /*item*/ ))
				,this, SLOT( slWidgetEdited() )
				//,Qt::UniqueConnection
				);
			Q_UNUSED(flag);

			//if(!flag)
			//	tva::util::showMsg("error on disconnect to slot btnEigenCalc");
		}
	};

	static const Real percentLimitCorrection(0.15);

	namespace rawData
	{
		struct descrSingleShaft
		{
			static const int countItem = 5;
			double l_fe;
			double d;
			double E;
			double mu;
			double ro;

			descrSingleShaft():l_fe(0),d(0),E(0),mu(0),ro(0){};

			descrSingleShaft(const tva::wizz::dataEditShaft &data)
			{
				if (data.constLocalLength)
				{
					l_fe = data.length;
				}
				else
				{
					l_fe = data.length/(data.N*1.0);
				}

				d = data.d;
				E = data.E;
				mu = data.mu;
				ro = data.ro;
			}

			static descrSingleShaft getDefaultValue()
			{
				tva::wizz::dataEditShaft data;
				static descrSingleShaft descr(data);

				//
				return descr;
			}
		};

		struct descrSingleDisc
		{
			static const int countItem = 3;
			double z;
			double Iz;
			double d;
			//
			descrSingleDisc():z(0),Iz(0),d(0){};
		};

		struct descrComplexShaft
		{
			typedef std::pair<double, double> pairDouble;
			typedef std::vector<pairDouble> vecPairDouble;
			typedef tva::util::MayBe<vecPairDouble> opVecPairReal;

			descrComplexShaft(){}

			void setDataShaft(const std::vector<tva::rawData::descrSingleShaft>& newData)
			{
				_dataShaft = newData;
				makeZDataViaRawData();
			}

			void setDataDisc(const std::vector<tva::rawData::descrSingleDisc>& newData)
			{
				_dataDisc = newData;
				//makeZDataViaRawData();
			}

			const std::vector<tva::rawData::descrSingleShaft>& dataShaft()const{return _dataShaft;}
			std::vector<tva::rawData::descrSingleShaft>& dataShaft(){return _dataShaft;}
			const std::vector<tva::rawData::descrSingleDisc>& dataDisc()const{return _dataDisc;}
			std::vector<tva::rawData::descrSingleDisc>& dataDisc(){return _dataDisc;}

			const opVecPairReal& descrShaftAuxZCoord()const {return _descrShaftAuxZCoord;}
			opVecPairReal& descrShaftAuxZCoord() {return _descrShaftAuxZCoord;}

			const tva::pairVecReal& descrRealZCoord()const{return _descrRealZCoord;};
			tva::pairVecReal& descrRealZCoord(){return _descrRealZCoord;};

			void makeZDataViaRawData();

		private:
			std::vector<tva::rawData::descrSingleShaft> _dataShaft;
			std::vector<tva::rawData::descrSingleDisc> _dataDisc;
			mutable opVecPairReal _descrShaftAuxZCoord;
			mutable tva::pairVecReal _descrRealZCoord;//z,y
		};
		//
	}

	struct MK_rawData
	{
		double m;
		double Iz;
		//
		double c;

		MK_rawData(const double&m_, const double&Iz_, const double&c_)
			:m(m_),Iz(Iz_),c(c_)
		{}
	};

	void dataShaftToRawMIP_Data(const std::vector<tva::rawData::descrSingleShaft>& shaft, std::vector<tva::MK_rawData>& res);

	namespace preProc
	{
		struct sensParam
		{
			sensParam(bool needTo=false)
				:needTo_(needTo), typeIndexDev_(-1),indexVarForDev_(-1), percentDeviation_(0)
			{}

			sensParam(const int& typeIndexVar, const double& dev, const int& indexVarForDev)
				:needTo_(true), typeIndexDev_(typeIndexVar), indexVarForDev_(indexVarForDev),percentDeviation_(dev)
			{
			}

			bool needTo()const{return needTo_;}
			int typeIndexDev()const{return typeIndexDev_;}
			int indexVarForDev()const{return indexVarForDev_;}
			double percent()const{return percentDeviation_;}

		private:
			bool needTo_;
			int typeIndexDev_;
			int indexVarForDev_;
			double percentDeviation_;
		};

		inline
		double fCalc_Iz(const rawData::descrSingleShaft& shaft)
		{
			const auto& l_fe=shaft.l_fe;
			const auto&d=shaft.d;
			const auto&d2=d*d;
			const auto&d4=d2*d2;
			const auto& ro=shaft.ro;
			const auto Iz=M_PI*l_fe*ro*d4/32.0;
			//
			return std::move(Iz);
		}
		//
		inline
		double fCalc_Ip(const rawData::descrSingleShaft& shaft)
		{
			const auto&d=shaft.d;
			const auto&d2=d*d;
			const auto&d4=d2*d2;
			const auto& Ip=M_PI*d4/32.0;
			//
			return std::move(Ip);
		}
		//
		inline
		double fCalc_A(const rawData::descrSingleShaft& shaft)
		{
			const auto&d=shaft.d;
			const auto&d2=d*d;
			const auto S = M_PI*d2/4.0;
			//
			return std::move(S);
		}

		void funcDoMakeGlobMatr( const tva::preProc::sensParam& sensP, const std::vector<tva::rawData::descrSingleShaft>& descrs, tva::tMatr& M, tva::tMatr& K, bool& hasM, bool& hasK, double& dx );

		static const double PaToGPa(1.0e9);

		void rawSingleShaftToTable(const tva::rawData::descrSingleShaft& single, const int& raw, QTableWidget*wgt);
		void rawSingleDiscToTable(const tva::rawData::descrSingleDisc& single, const int& raw, QTableWidget*wgt);
		void TableRowToSingleShaft(const QTableWidget*wgt, const int& indexRow, tva::rawData::descrSingleShaft& single);
		void TableRowToSingleDisc(const QTableWidget*wgt, const int& indexRow, tva::rawData::descrSingleDisc& single);
		void rawShaftDataToWgt(const tva::rawData::descrComplexShaft& descr, tva::tableWrapper *wrTableShaft);
		void rawDiscDataToWgt(const tva::rawData::descrComplexShaft& descr, tva::tableWrapper *wrTableDisc);
		void WgtToRawShaftData(const tva::tableWrapper*wrTableShaft, tva::rawData::descrComplexShaft& descr);
		void WgtToRawDiscData(const tva::tableWrapper*wrTableDisc, tva::rawData::descrComplexShaft& descr);
		void plotShaft(const tva::chartSetup::opChartStyle& style, const tva::rawData::descrComplexShaft& descr, QCustomPlot*plot, bool needClear, bool leftAxis);
		void plotShaftToEigenVecs(const tva::rawData::descrComplexShaft& descr, QCustomPlot*plot, bool needClear, bool leftAxis);
		//
		namespace tuningPlot
		{
			void makeOnlyShaftPlotData(const tva::rawData::descrComplexShaft& descr, tva::opVecReal& z, tva::vecReal&y);
			//
			tva::chartSetup::lineStyle makeShaftLineStyle();
			tva::chartSetup::lineStyle makeDiscLineStyle();
			tva::chartSetup::chartStyle makeShaftChartStyle(const tva::vecReal& vX,const std::vector<tva::vecReal>& vY);
		}
	}
}