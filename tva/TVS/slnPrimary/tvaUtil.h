#pragma once

//class QString;
//class QFrame;
//class QWidget;
//class QTableWidget;
//class QCustomPlot;
//class QCPGraph;
//class Real;
//
//#include <vector>
//
//#include <algorithm>
//#include "qstring.h"
//#include "qcolor.h"
////#include "qcustomplot.h"
////#include "tvaMatrix.h"
//
//namespace tva
//{
//	typedef double Real;
//	typedef bool	ebool;
//	const ebool efalse(false);
//	const ebool etrue(true);
//	//int calcGlobMatrSize(const int & discCount);
//	typedef std::vector<Real> vecReal;
//	typedef std::vector<vecReal> vecvecReal;
//	typedef std::pair<vecReal, vecReal> pairVecReal;
//
//	namespace util
//	{
//		template<typename T>
//		class MayBe
//		{
//		protected:
//			ebool _setled;
//			T	 _value;
//		public:
//
//			MayBe():_setled(efalse){}
//
//			MayBe(const MayBe<T>& rv)
//			{
//				_setled = rv._setled;
//				_value = rv._value;
//			}
//
//			MayBe& operator=(const MayBe<T>& rv)
//			{
//				_setled = rv._setled;
//				_value = rv._value;
//				return *this;
//			}
//
//			MayBe(const T& t)
//			{
//				_setled = etrue;
//				_value = t;
//			}
//
//			MayBe& operator=(const T& t)
//			{
//				_setled = etrue;
//				_value = t;
//				return *this;
//			}
//
//			//operator ebool()const{return _setled;}
//			ebool isSettled()const{return _setled;}
//
//			const T& get()const
//			{
//				if (_setled)
//				{
//					return _value;
//				}
//				else
//				{
//					throw;
//				}
//			}
//
//			T& get()
//			{
//				if (_setled)
//				{
//					return _value;
//				}
//				else
//				{
//					throw;
//				}
//			}
//
//			void set(const T& rv)
//			{
//				_setled = etrue;
//				_value = rv;
//			}
//
//			void reset(){_setled = efalse;}
//
//			//T& value(){return _value;}
//		private:
//		};
//
//		//inline
//		//QString RowToString(const hasem::Matrix&m, const int& i)
//		//{
//		//	//const auto&n=m.Ncols();
//		//	const auto&n=m.col();
//		//	QString s;
//		//	for (auto j=0;j<n;++j)
//		//	{
//		//		//s.append(QString("\t%1").arg(m.element(i,j)));
//		//		s.append(QString("\t%1").arg(m(i,j)));
//		//	}
//		//	return s;
//		//}
//
//		//template<>
//		//inline
//		//QString toString<hasem::Matrix>(const hasem::Matrix&t)
//		//{
//		//	//const auto&n=t.Ncols();
//		//	const auto&n=t.col();
//		//	QStringList sl;
//		//	for (auto i=0;i<n;++i)
//		//	{
//		//		sl.append(tva::util::RowToString(t,i));
//		//	}
//		//	QString res = sl.join("\n");
//		//	//
//		//	return std::move(res);
//		//}
//
//		inline
//		tva::util::MayBe<double> toDouble(const QString& str)
//		{
//			tva::util::MayBe<double> res;
//			bool flag(false);
//			double d = str.toDouble(&flag);
//			if (flag)
//			{
//				res = d;
//			}
//			return res;
//		}
//
//		inline
//		tva::util::MayBe<int> toInt(const QString& str)
//		{
//			tva::util::MayBe<int> res;
//			bool flag(false);
//			int d = str.toInt(&flag);
//			if (flag)
//			{
//				res = d;
//			}
//			return res;
//		}
//	}
//
//	typedef util::MayBe<Real> opReal;
//	typedef util::MayBe<QString> opString;
//	//
//	typedef std::vector<opString> vecOpString;
//	typedef tva::util::MayBe<vecOpString> opVecOpString;
//
//	namespace str
//	{
//		static const QString noData("no data");
//	}
//
//	void noDataToTable(QTableWidget *tw);
//	void dataToTableItem(const QString& str, const int&r,const int&c, QTableWidget *tw);
//	tva::opReal TableItemToDouble(const QTableWidget *tw, const int&r,const int&c);
//
//	namespace util
//	{
//		struct minmax
//		{
//			opReal min;
//			opReal max;
//		};
//
//		opReal getMaxElement(const vecReal& vec);
//		opReal getMinElement(const vecReal& vec);
//		minmax getMinMaxElement(const tva::vecvecReal& matr);
//	}
//
//	typedef util::MayBe<vecReal> opVecReal;
//	//typedef util::MayBe<Matrix> opMatrix;
//	typedef util::MayBe<QColor> opColor;
//
//	tva::vecReal makeInterval(const int&startValue, const int&finishValue);
//
//	class WatcherTableEdit : public QObject
//	{
//		Q_OBJECT
//
//	signals:
//		void ItemEdited();
//
//	private slots:
//		void slEditItem();
//
//		void slLeaveItem();
//
//	public:
//		WatcherTableEdit(QTableWidget * tbl);
//
//		~WatcherTableEdit()
//		{
//			//disconnect()
//		}
//
//		void setActive(bool state);
//
//	private:
//		bool active;
//		QTableWidget * _tbl;
//		bool flagStartEdit;
//	};
//
//	namespace postProc
//	{
//		void vecToWidgetTable(const vecReal& mat, QTableWidget *tw);
//
//		struct policyChart
//		{
//			tva::chartSetup::opChartStyle style;
//			tva::chartSetup::opLineStyle lStyle;
//			//
//			bool needClear;
//			bool leftAxis;
//		};
//
//		QCPGraph* vec2Chart (const postProc::policyChart& policy, const tva::opVecReal& opVx, const tva::vecReal& vy, QCustomPlot *plot);
//		//
//		void columnus2Chart
//				(const tva::chartSetup::opChartStyle& style
//				, const tva::opVecReal& dataZ
//				, const tva::vecvecReal& mat
//				, QCustomPlot *plot
//				, const tva::opVecOpString& vecAdditionalLineText=tva::opVecOpString()
//				);
//	}
//	inline
//	const int cols(const vecvecReal& matr)//set of column!
//	{
//		return matr.size();
//	}
//	inline
//	const int rows(const vecvecReal& matr)//set of column!
//	{
//		tva::vecReal v;
//		for(size_t i=0; i<matr.size(); ++i)
//		{
//			v.push_back(matr[i].size());
//		}
//		const auto a= tva::util::getMaxElement(v);
//		if (a.isSettled())
//		{
//			return a.get();
//		}
//		else
//		{
//			return 0;//no cols atall
//		}
//	}
//}