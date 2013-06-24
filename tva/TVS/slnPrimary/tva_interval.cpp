#include "tva_interval.h"

const int tva::getColsCount(const  tva::vecvecReal& matr)//set of column!
{
	return matr.size();
}

const int  tva::getRowsCount(const  tva::vecvecReal& matr)//set of column!
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
		return 0;//no cols at all
	}
}

tva::opReal tva::util::getMaxElement( const vecReal& vec )
{
	opReal res;
	auto it = std::max_element(vec.begin(),vec.end());
	if (it!=vec.end())
	{
		res = *it;
	}
	return res;
}

tva::opReal tva::util::getMinElement( const vecReal& vec )
{
	opReal res;
	auto it = std::min_element(vec.begin(),vec.end());
	if (it!=vec.end())
	{
		res = *it;
	}
	return res;
}

tva::util::minmax tva::util::getMinMaxElement( const tva::vecvecReal& matr )
{
	tva::util::minmax res;
	tva::vecReal vMin, vMax;
	for (auto i=0; i<tva::getColsCount(matr); ++i)
	{
		opReal min=tva::util::getMinElement(matr[i]);
		opReal max=tva::util::getMaxElement(matr[i]);
		//
		if (min.isSettled())
		{
			vMin.push_back(min.get());
		}
		//
		if (max.isSettled())
		{
			vMax.push_back(max.get());
		}
	}
	res.min=tva::util::getMinElement(vMin);
	res.max=tva::util::getMaxElement(vMax);
	//
	return res;
}

tva::vecReal tva::makeInterval( const int&startValue, const int&finishValue )
{
	//
	tva::vecReal res;
	for (auto i=startValue; i<finishValue;++i)
	{
		res.push_back(i);
	}
	//
	return std::move(res);
}