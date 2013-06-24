#pragma once
#include "tvaChart.h"

class QString;
class QFrame;
class QWidget;
class QTableWidget;
class QCustomPlot;
class QCPGraph;
class Real;

#include <vector>
#include <algorithm>
//#include "tvaUtil.h"
#include "tva_interval.h"

namespace tva
{
	void noDataToTable(QTableWidget *tw);
	void dataToTableItem(const QString& str, const int&r,const int&c, QTableWidget *tw);
	tva::opReal TableItemToDouble(const QTableWidget *tw, const int&r,const int&c);


	tva::vecReal makeInterval(const int&startValue, const int&finishValue);

	namespace chartSetup
	{
		typedef std::pair<opReal,opReal> opPair;
		typedef tva::util::MayBe<opPair> limitAxis;
		//
		static const QColor defColor = QColor(0,0,255);
		static const QColor defColorDisc = QColor(0,85,0);

		struct lineStyle
		{
			static const QCP::ScatterStyle defScatter = QCP::ssCircle;
			static const QCPGraph::LineStyle defLineStyle = QCPGraph::lsLine;

			QCPGraph::LineStyle style;
			QCP::ScatterStyle	scatter;
			opColor color;

			tva::opString name;
			//
			lineStyle():style(defLineStyle), scatter(defScatter){};
		};

		typedef tva::util::MayBe<tva::chartSetup::lineStyle> opLineStyle;

		struct axisStyle
		{
			tva::chartSetup::limitAxis lim;
			QString title;
			opReal multiplicator;
		};
		typedef tva::util::MayBe<tva::chartSetup::axisStyle> opAxisStyle;

		struct chartStyle
		{
			QString title;
			//
			axisStyle axStyleX;
			axisStyle axStyleY;
		};
		typedef tva::util::MayBe<tva::chartSetup::chartStyle> opChartStyle;

		void setupAxisStyle(const tva::chartSetup::opAxisStyle& style, QCPAxis* axis);
		void setupAxisesStyle(const tva::chartSetup::opChartStyle& style, QCPAxis* xAxis, QCPAxis* yAxis);
	}

	class WatcherTableEdit : public QObject
	{
		Q_OBJECT

signals:
		void ItemEdited();

		private slots:
			void slEditItem();

			void slLeaveItem();

	public:
		WatcherTableEdit(QTableWidget * tbl);

		~WatcherTableEdit()
		{
			//disconnect()
		}

		void setActive(bool state);

	private:
		bool active;
		QTableWidget * _tbl;
		bool flagStartEdit;
	};

	namespace postProc
	{
		void vecToWidgetTable(const vecReal& mat, QTableWidget *tw);

		struct policyChart
		{
			tva::chartSetup::opChartStyle style;
			tva::chartSetup::opLineStyle lStyle;
			//
			bool needClear;
			bool leftAxis;
		};

		QCPGraph* vec2Chart (const postProc::policyChart& policy, const tva::opVecReal& opVx, const tva::vecReal& vy, QCustomPlot *plot);
		//
		void columnus2Chart
			(const tva::chartSetup::opChartStyle& style
			, const tva::opVecReal& dataZ
			, const tva::vecvecReal& mat
			, QCustomPlot *plot
			, const tva::opVecOpString& vecAdditionalLineText=tva::opVecOpString()
			);
	}
	inline
		const int cols(const vecvecReal& matr)//set of column!
	{
		return matr.size();
	}
	inline
		const int rows(const vecvecReal& matr)//set of column!
	{
		tva::vecReal v;
		for(size_t i=0; i<matr.size(); ++i)
		{
			v.push_back(matr[i].size());
		}
		const auto a= tva::util::getMaxElement(v);
		if (a.isSettled())
		{
			return a.get();
		}
		else
		{
			return 0;//no cols atall
		}
	}
}