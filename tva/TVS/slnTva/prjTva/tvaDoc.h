#pragma once
#include "ui_tvaDocWnd.h"
#include "shell.h"
//#include "qcustomplot.h"
#include "..\..\slnPostProc\postProc.h"

namespace tva
{
	class wrModeSelector : public QObject
	{
		Q_OBJECT
	public:
		wrModeSelector(QTableWidget* tbl, QCustomPlot * plot)
			:QObject(), tbl_(tbl), plot_(plot)
		{
			{
				bool flag = connect(
					tbl_, SIGNAL(itemSelectionChanged())
					, this, SLOT(slSelectionChanged())
					);
				if(!flag)
					tva::util::showMsg("error on connection to slot btnEigenCalc");
			}
		}
	protected:
	private:
		QTableWidget* tbl_;
		QCustomPlot * plot_;

	private slots:
		void slSelectionChanged()
		{
			const auto& selectedOj = tbl_->selectedItems();
			//
			std::set<int> selectedRows;
			for (auto it = selectedOj.begin(); it!=selectedOj.end(); ++it)
			{
				selectedRows.insert((*it)->row());
			}
			//unselect chart
			tva::postProc::modes::selected( selectedRows, tbl_, plot_);
		}
	};

	class Doc: public QWidget, private Ui::Form
	{
		Q_OBJECT
	public:
		explicit Doc(const tva::Shell& shell_, QWidget *parent = 0);
		~Doc(){}

		QString userFriendlyCurrentFile();

	public slots:
		void show();

signals:
		void rawShaftDataChanged();
		void rawDiscDataChanged();

	private slots:
		void slMakeEigenData();
		void slOmegaSelectionChanged();
		void slMakeMatrises();
		void slSensAnalisys();
		void slSolveStaticTask();
		void slSolveDynamicTask();

		void slWizardShaft();
		void slWizardDisc();
		void slUpdateRawShaftData();
		void slUpdateRawDiscData();
		void slUpdatePlotComplexShaft();
		void slTableShaftToModel();
		void slTableDiscToModel();
		void slShowDynStep(int value);
		//void slDynSliderReleased();

	private:
		void newFile();
		void initConnect();

		std::shared_ptr<wrModeSelector> msTorsional;
		std::shared_ptr<wrModeSelector> msBending;

		std::shared_ptr<QCustomPlot> plotOmega3;
		std::shared_ptr<QCustomPlot> plotVecs3;
		std::shared_ptr<QCustomPlot> plotShaft;
		std::shared_ptr<QCustomPlot> plotStaticTask;
		std::shared_ptr<QCustomPlot> plotRightEdge;
		std::shared_ptr<QCustomPlot> plotDynProfile;
		//
		std::shared_ptr<QCustomPlot> plotU_dbg;
		//
		QString curFile;
		QString strippedName(const QString &fullFileName);

		//tva::Shell shell;
		tva::Model model;
		const tva::Shell& shell;
		//
		 bool isUntitled;

		 std::shared_ptr<tva::tableWrapper> wrRawShaftData;
		 std::shared_ptr<tva::tableWrapper> wrRawDiscData;
	};
}