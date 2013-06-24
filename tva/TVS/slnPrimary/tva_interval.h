#pragma once
#include "primary.h"

namespace tva
{
	namespace util
	{
		struct minmax
		{
			opReal min;
			opReal max;
		};

		opReal getMaxElement(const vecReal& vec);
		opReal getMinElement(const vecReal& vec);
		minmax getMinMaxElement(const tva::vecvecReal& matr);
	}

	tva::vecReal makeInterval(const int&startValue, const int&finishValue);

	const int getColsCount(const tva::vecvecReal& matr);//set of column!
	const int getRowsCount(const tva::vecvecReal& matr);//set of column!
}