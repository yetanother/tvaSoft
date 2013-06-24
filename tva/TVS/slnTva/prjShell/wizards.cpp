#include "wizards.h"
#include "tvaUtil.h"
#include "qtUtils.h"
#include "preProc.h"

tva::wizz::EditShaft::EditShaft( QWidget* parent )
	:QDialog(parent)
{
	setupUi(this);
}

int tva::wizz::EditShaft::exec( dataEditShaft& data )
{
	//dataToWgt
	dataToWgt( data );
	//
	int res = QDialog::exec();
	if (res == QDialog::Accepted)
	{//WgtToData
		WgtToData(data);
	}
	//
	return res;
}

void tva::wizz::EditShaft::dataToWgt( const dataEditShaft& data )
{
	//
	rb_l_const->setChecked(data.constLocalLength);
	rb_L_const->setChecked(!data.constLocalLength);
	//
	editLength->setText(tva::util::toString(data.length));
	editCount->setText(tva::util::toString(data.N));
	editDiameter->setText(tva::util::toString(data.d));
	editE->setText(tva::util::toString(data.E/tva::preProc::PaToGPa));
	edit_mu->setText(tva::util::toString(data.mu));
	edit_ro->setText(tva::util::toString(data.ro));
}

void tva::wizz::EditShaft::WgtToData( dataEditShaft& data )
{
	QStringList sl;
	bool flagError(false);
	//
	data.constLocalLength = rb_l_const->isChecked();

	{
		const auto& opLength = tva::util::toDouble(editLength->text());
		if (opLength.isSettled())
		{
			data.length = opLength.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Length");
		}
	}
	//
	{
		const auto& opN = tva::util::toInt(editCount->text());
		if (opN.isSettled())
		{
			data.N = opN.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Count");
		}
	}
	//
	{
		const auto& opDiam = tva::util::toDouble(editDiameter->text());
		if (opDiam.isSettled())
		{
			data.d = opDiam.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Diameter");
		}
	}
	//
	{
		const auto& opE = tva::util::toDouble(editE->text());
		if (opE.isSettled())
		{
			data.E = opE.get()*tva::preProc::PaToGPa;
		}
		else
		{
			flagError = true;
			sl.append("Error on Young's modulus");
		}
	}
	//
	{
		const auto& opMu = tva::util::toDouble(edit_mu->text());
		if (opMu.isSettled())
		{
			data.mu = opMu.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Puasson Coeff");
		}
	}
	//
	{
		const auto& opRo = tva::util::toDouble(edit_ro->text());
		if (opRo.isSettled())
		{
			data.ro = opRo.get();
		}
		else
		{
			sl.append("Error on Density");
		}
	}

	if (flagError)
	{
		QString str = sl.join("\n");
		tva::util::showMsg(str);
		data.stateData = false;
	}
}

tva::wizz::EditDisc::EditDisc( QWidget* parent/*=0*/ )
	:QDialog(parent)
{
	setupUi(this);
}

int tva::wizz::EditDisc::exec( dataEditDisc& data )
{
	//
	dataToWgt( data );
	//
	int res = QDialog::exec();
	if (res == QDialog::Accepted)
	{//WgtToData
		WgtToData(data);
	}
	//
	return res;
}

void tva::wizz::EditDisc::dataToWgt( const dataEditDisc& data )
{
	//
	rb_l_const->setChecked(data.constLocalLength);
	rb_L_const->setChecked(!data.constLocalLength);
	//
	editLength->setText(tva::util::toString(data.length));
	editCount->setText(tva::util::toString(data.N));
	editDiameter->setText(tva::util::toString(data.d));
	editZ0->setText(tva::util::toString(data.z));
	editDiameter->setText(tva::util::toString(data.d));
}

void tva::wizz::EditDisc::WgtToData( dataEditDisc & data )
{
	QStringList sl;
	bool flagError(false);
	//
	data.constLocalLength = rb_l_const->isChecked();

	{
		const auto& opLength = tva::util::toDouble(editLength->text());
		if (opLength.isSettled())
		{
			data.length = opLength.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Length");
		}
	}
	//
	{
		const auto& opN = tva::util::toInt(editCount->text());
		if (opN.isSettled())
		{
			data.N = opN.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Count");
		}
	}
	//
	{
		const auto& opZ = tva::util::toDouble(editZ0->text());
		if (opZ.isSettled())
		{
			data.z = opZ.get();//*tva::preProc::PaToGPa;
		}
		else
		{
			flagError = true;
			sl.append("Error on Z coord");
		}
	}
	//
	{
		const auto& opIz = tva::util::toDouble(editIz->text());
		if (opIz.isSettled())
		{
			data.Iz = opIz.get();//*tva::preProc::PaToGPa;
		}
		else
		{
			flagError = true;
			sl.append("Error on Iz");
		}
	}
	//
	{
		const auto& opDiam = tva::util::toDouble(editDiameter->text());
		if (opDiam.isSettled())
		{
			data.d = opDiam.get();
		}
		else
		{
			flagError = true;
			sl.append("Error on Diameter");
		}
	}

	if (flagError)
	{
		QString str = sl.join("\n");
		tva::util::showMsg(str);
		data.stateData = false;
	}
}