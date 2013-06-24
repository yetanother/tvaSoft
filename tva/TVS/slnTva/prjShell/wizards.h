#pragma once
#include "ui_wizShaft.h"
#include "ui_wizDisc.h"
#include "..\src\corelib\kernel\qobject.h"

#include "qwidget.h"
#include <tuple>

namespace tva
{
	namespace wizz
	{
		struct dataEditShaft
		{
			bool constLocalLength;
			double length;
			int N;
			double d;
			double E;
			double mu;
			double ro;
			//
			bool stateData;

			dataEditShaft()
				:constLocalLength(false)
				,length(1.0)
				,N(10)
				,d(0.01)
				,E(210e9)
				,mu(0.28)
				,ro(7800)
				,stateData(true)
			{
				//
			}
		};

		struct dataEditDisc
		{
			bool constLocalLength;
			double length;
			int N;
			double z;
			double Iz;
			double d;
			//
			bool stateData;

			dataEditDisc()
				:constLocalLength(false)
				,length(1.0)
				,N(10)
				,z(0.0)
				,Iz(1)
				,d(0.02)
				,stateData(true)
			{
				//
			}
		};

		class EditShaft : public QDialog, private Ui::wizShaft
		{
			Q_OBJECT
		public:
			explicit EditShaft(QWidget* parent=0);
			~EditShaft(){}

		public slots:
			int exec(dataEditShaft& data);

		private:
			void dataToWgt(const dataEditShaft& data);
			void WgtToData(dataEditShaft& data);
		};

		static EditShaft* getDlgEditShaft(QWidget* parent=0)
		{
			static EditShaft* p = new EditShaft(parent);
			return p;
		}

		class EditDisc : public QDialog, private Ui::wizDisc
		{
			Q_OBJECT
		public:
			explicit EditDisc(QWidget* parent=0);
			~EditDisc(){}

			public slots:
				int exec(dataEditDisc& data);

		private:
			void dataToWgt(const dataEditDisc& data);
			void WgtToData(dataEditDisc & data);
		};

		static EditDisc* getDlgEditDisc(QWidget* parent=0)
		{
			static EditDisc* p = new EditDisc(parent);
			return p;
		}
	}
}