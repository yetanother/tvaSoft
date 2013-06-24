#pragma once
#include "eigenCore.h"
#include <set>
#include "qboxlayout.h"
#include "prj_tChart\chartSetup.h"

class QTableWidget;
class vecReal;
class Matrix;
class QCustomPlot;
class QLineEdit;

namespace tva
{
	class Model;
}

namespace tva
{
	typedef std::pair<QTableWidget*, QTableWidget*> pairTbl;
	typedef std::pair<QCustomPlot*, QCustomPlot*> pairPlot;
	typedef std::pair<QLineEdit*, QLineEdit*> pairEdit;
	typedef std::pair<QLineEdit*,QLineEdit*> pairEdit;
	typedef std::pair<pairEdit,pairEdit> pairpairEdit;
	typedef std::pair<pairTbl, pairTbl> pairpairTbl;
	//
	namespace postProc
	{
		namespace sub
		{
			void GlobMatrisesToWgt(const tva::globMatrises& gM, QTableWidget* twM, QTableWidget* twK);
			void vecOmegaToWidgetTable(const tva::opVecReal& vecOmega, QTableWidget *tw);
			void eigenVecsToTable(const tva::Model& model, QTableWidget *tw);
			//
			void plotOmega(const tva::Model& model, QCustomPlot* plot);
			void plotEigenVecs(const tva::Model& model, QCustomPlot* plot);
			void tableErrEigenProblem(const tva::Model& model, tva::pairTbl& wgtErr, tva::pairpairEdit& wdtMaxErr);
					//		//
			void doubleToLineEdit(const tva::opReal& d, QLineEdit* edit);
			//void pivotDataToWgt(const tva::proc::pivotElem&pivot, );
		}

		//inline
		void fInitPlot(QWidget* parent, std::shared_ptr<QCustomPlot>& plot);

		void MatrToWidgetTable(const tva::vecvecReal& mat, QTableWidget *tw);
		void MatrToWidgetTable(const tva::tMatr& matr, QTableWidget *tw);

		void ShowModelData (const tva::Model& model, tva::pairpairTbl& MK_EigData, tva::pairPlot& plotOmeVecs, tva::pairTbl& wgtErr, tva::pairpairEdit& wdtMaxErr );

		typedef tva::chartSetup::chartStyle chStyle;

		//inline
		chStyle fMakeTypicalStyle(const tva::opVecReal& vecX, const tva::opVecReal& vecY);
		//
		namespace modes
		{
			void selected(const std::set<int>& cont, QTableWidget* twOmega, QCustomPlot* plot);
		}
	}
}