#include "eigenCore.h"
#include "..\src\corelib\io\qdebug.h"
#include <iostream>
#include "qtUtils.h"
#include <algorithm>

//
void tva::stub::makeRegularMatrises( const regularData& data, globMatrises& gM )
{
	const auto& size = tva::calcGlobMatrSize(data.n);
	//
	{//make M
		tva::tMatr M;
		tva::stub::makeRegularMatrM( data.mipI, size, M );
		gM.M() = std::move(M);
		gM.hasM=true;
	}
	{//make K
		tva::tMatr K;
		tva::stub::makeRegularMatrK( data.strengh, size, K );
		gM.K() = std::move(K);
		gM.hasK=true;
	}
}

void tva::stub::makeRegularVector( const Real& value, const int&size, std::vector<Real>& column )
{
	column.resize(size);
	//
	for(auto it=column.begin();it!=column.end();++it)
	{
		(*it) = value;
	}
}

void tva::stub::makeRegularMatrM( const Real&value,const int&size, tva::tMatr& M )
{
	//
	std::vector<Real> vecM;
	tva::stub::makeRegularVector( value, size, vecM );
	tva::stub::vector2diagonal( vecM, M );
}

void tva::stub::vector2diagonal( const std::vector<Real>& vec, tva::tMatr& sm )
{
	const size_t &size= vec.size();
	//
	sm.resize(size,size);
	//
	sm.fill(tva::nulReal);
	//
	for (size_t it=0;it<size;++it)
	{
		sm.getElem(it,it) = vec[it];
	}
}

void tva::stub::makeRegularMatrK( const Real&value,const int&size, tva::tMatr& K )
{
	std::vector<Real> vecK;
	//symMatr res;
	tva::stub::makeRegularVector( value, size, vecK );
	//
	std::function<void( const Real&, tva::tMatr& )> funcMakeLocal = &tva::local::makeK;
	tva::stub::makeRegularMatrByVectorValue( funcMakeLocal,vecK, K );
}

void tva::stub::diagMatrToVector( const tva::tMatr& m, tva::vecReal& vec )
{
	vec.clear();
	for (int i=0; i<m.countCols(); ++i)
	{
		vec.push_back(m.getElem(i,i));
	}
}

void tva::stub::makeRegularMatrByVectorValue( tva::functor_Real_Matr &funcMakeLocal, const tva::vecReal& vec, tva::tMatr& K )
{
	const auto&size=vec.size();
	//
	if (size==1)
	{
		const int size(1);
		K.resize(size, size);
		K.fill(0.0);
		K.getElem(0,0) = vec[0];
	}else
	//if (size==2)
	//{
	//	funcMakeLocal(vec[0],K);
	//	return;
	//}else
	//	if(size>0)
		{
			const auto effCount(size);
			const int size=(tva::local::sizeLocalMatrixK)*(effCount+1)/2;
			//
			K.resize(size,size);
			K.fill(0.0);
			//
			//const int& s= tva::local::sizeLocalMatrixK;
			for(int it=0; it<effCount-1+1; ++it)
			{
				const double& value=vec[it];
				tva::tMatr kLocal;
				funcMakeLocal(value,kLocal);
				//
				const auto& shift=it;
				tva::stub::addLocMatrToGlobMatr( kLocal, K, shift);
			}
			return;
		}
		//else
		//	return;
			//throw;
}

void tva::stub::addLocMatrToGlobMatr( const tva::tMatr& locM, tva::tMatr& globM, const int& startIndex )
{
	const auto&n=locM.countCols();
	//
	for (auto i=0;i<n;++i)
	{
		for (auto j=0;j<n;++j)
		{
			int is=i+startIndex;
			int js=j+startIndex;
			//
			const Real&d= locM.getElem(i,j);
			//
			globM.getElem(is,js) = globM.getElem(is,js)+d;
		}
	}
}

void tva::stub::addLastLocMatrToGlobMatr( const tva::tMatr& locM, tva::tMatr& globM, const int& startIndex )
{
	const auto&n=locM.countCols();
	//
	for (auto i=0;i<n;++i)
	{
		for (auto j=i;j<n;++j)
		{
			int is=i+startIndex-1;
			int js=j+startIndex-1;
			//
			const Real&d= locM.getElem(i,j);
			//
			if ((i==j)&&(i==0))//||(i!=0))
			{
				//globM.element(is,js) = d;
			}else
			//if (j!=0)
			{
				//globM.element(is,js) = globM.element(is,js)+d;
				globM.getElem(is,js) = d;
			}
		}
	}
}

void tva::local::makeK( const Real&value, tva::tMatr& local )
{
	//make Klocal
	local.resize(sizeLocalMatrixK, sizeLocalMatrixK);
	local.getElem(0,0)=value;local.getElem(0,1)=-value;
	local.getElem(1,0)=-value;local.getElem(1,1)=value;
}

void tva::local::makeM( const Real&value, tva::tMatr& local )
{
	//make Mlocal
	local.resize(sizeLocalMatrixK, sizeLocalMatrixK);
	//
	local.getElem(0,0)=value;local.getElem(0,1)=-value*0.5;
	local.getElem(1,0)=local.getElem(0,1);local.getElem(1,1)=value;
}

int/*codeError*/ tva::simpleSolveEigenProblem( const tva::tMatr& M, const tva::tMatr& K, std::vector<double>& eigenVals, tva::vecvecReal& eigenVecs )
{
	try
	{
		tva::EigenSolver es(M,K);
		//
		auto& unsortedValues=es.eigenValues();
		auto& unsortedVectors = es.eigenVectors();//unsorted
		typedef std::pair<tva::Real,tva::Real> tPair;
		typedef std::vector<tPair> tVecPair;
		tVecPair vecPair;
		for (auto i=0;i<unsortedValues.size();++i)
		{
			//
			tPair p(unsortedValues[i],i);
			vecPair.push_back(p);
		}
		auto f=[](const tPair& v1, const tPair& v2){return v1.first<v2.first;};
		std::sort(vecPair.begin(),vecPair.end(), f);
		//
		eigenVals.clear();
		eigenVecs.clear();
		for (auto i=0; i<vecPair.size(); ++i)
		{
			eigenVals.push_back(vecPair[i].first);
			//
			eigenVecs.push_back(unsortedVectors[vecPair[i].second]);
		}
		//
		return 0;//ok
	}
	catch (...)
	{
		tva::util::showMsg("Error solveEigenProblem");
		return 1;//unknownError
	}
}

void tva::calcOmega( const std::vector<double>& eigenVals, std::vector<double>& Omega )
{
	//
	const auto& size(eigenVals.size());
	Omega.clear();
	//Omega.resize(size);
	//
	for(auto it=0; it<size; ++it)
	{
		const auto&raw = eigenVals[it];
		//
		const double corrValue= (fabs(raw)<tva::defErr)? 0.0 : sqrt(raw);
		Omega.push_back(corrValue);
		//if (fabs(raw)<tva::defErr)
		//{
		//	Omega[it] = 0.0;
		//}
		//else
		//{
		//	Omega[it] = sqrt(raw);
		//}
	}
	//
}

int/*codeError*/ tva::getNormalEigenVecs( const tva::vecvecReal& eigenVecs, const tva::tMatr& M, tva::vecvecReal& normalizedEigenVecs )
{
	const auto& size(M.countCols());
	//
	normalizedEigenVecs.clear();
	//
	for(auto it=0; it<size; ++it)
	{
		//eigenVecs.Column(it);
		 auto& v = eigenVecs[it];
		auto e=tva::getDoubleOuterMult(v, M, v);
		//v.t();
		tva::vecReal vNext;
		foreach(auto elem, v)
		{
			vNext.push_back(elem/sqrt(e));
		}
		//
		normalizedEigenVecs.push_back(vNext);
		//const
		//diagMatr d; d<<v.t() * M * v;
		//const double k= d.element(0);
		//normalizedEigenVecs.Column(it) = v/k;
	}
	return 0;
}

int tva::calcGlobMatrSize( const int & discCount )
{
	return discCount;
}

bool tva::doSolveEigenProblem( const tMatr& M, const tMatr& K, vecReal& Omega, vecvecReal& normVecs )
{
	tva::vecReal eigenVals;
	tva::vecvecReal eigenVecs;
	int codeError = tva::simpleSolveEigenProblem(M, K, eigenVals, eigenVecs);
	if (0 == codeError)
	{
		tva::calcOmega(eigenVals, Omega);
		tva::getNormalEigenVecs(eigenVecs, M, normVecs);
		return true;
	}
	//
	return false;
}