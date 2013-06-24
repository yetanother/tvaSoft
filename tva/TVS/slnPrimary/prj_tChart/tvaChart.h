#pragma once

#include "primary.h"
#include "cmnExtInclude.h"

namespace tva
{
	typedef util::MayBe<QColor> opColor;

	//namespace chartSetup
	//{
	//	typedef std::pair<opReal,opReal> opPair;
	//	typedef tva::util::MayBe<opPair> limitAxis;
	//	//
	//	static const QColor defColor;// = QColor(0,0,255);
	//	static const QColor defColorDisc = QColor(0,85,0);

	//	struct lineStyle
	//	{
	//		static const QCP::ScatterStyle defScatter = QCP::ssCircle;
	//		static const QCPGraph::LineStyle defLineStyle = QCPGraph::lsLine;

	//		QCPGraph::LineStyle style;
	//		QCP::ScatterStyle	scatter;
	//		opColor color;

	//		tva::opString name;
	//		//
	//		lineStyle():style(defLineStyle), scatter(defScatter){};
	//	};

	//	typedef tva::util::MayBe<tva::chartSetup::lineStyle> opLineStyle;

	//	struct axisStyle
	//	{
	//		tva::chartSetup::limitAxis lim;
	//		QString title;
	//		opReal multiplicator;
	//	};
	//	typedef tva::util::MayBe<tva::chartSetup::axisStyle> opAxisStyle;

	//	struct chartStyle
	//	{
	//		QString title;
	//		//
	//		axisStyle axStyleX;
	//		axisStyle axStyleY;
	//	};
	//	typedef tva::util::MayBe<tva::chartSetup::chartStyle> opChartStyle;

	//	void setupAxisStyle(const tva::chartSetup::opAxisStyle& style, QCPAxis* axis);
	//	void setupAxisesStyle(const tva::chartSetup::opChartStyle& style, QCPAxis* xAxis, QCPAxis* yAxis);
	//}
}