#include "tvaModel.h"

tva::eigenVecs tva::eigenVecs::makeNewEigenVecs()
{
	eigenVecs r;
	return r;
}

tva::eigenVecs::eigenVecs()
	:init(false)
{
	//qDebug();
}

void tva::eigenVecs::setData( const tva::vecvecReal& m )
{
	data= m;
	init = true;
}

bool tva::eigenVecs::isInit() const
{
	return init;
}

tva::Model::Model()// :normEigenVecs()
	//:EigenData.normEigenVecs(tva::eigenVecs::makeNewEigenVecs())
{
	//EigenData.normEigenVecs=tva::eigenVecs::makeNewEigenVecs();
}

void tva::Model::calcErrorEigenProblem( const tva::vecReal& Omega, tva::errEigenData& errData )
{
	auto fCalcErrMatr=[](const tva::tMatr& M, const tva::vecvecReal& vecs, tva::tMatr& res)
	{
		const int&n = M.countCols();
		{
			//auto& utMu = errData.globM.M();
			res.resize(n,n);
			for (int i=0;i<n;++i)
			{
				for (int j=i;j<n;++j)
				{
					const auto& am=vecs;
					//
					auto ui = am[i];
					auto uj = am[j];
					auto e=tva::getDoubleOuterMult(ui, M, uj);
					//
					res.getElem(i,j)=e;
					res.getElem(j,i)=e;
				}
			}
			//
		}
		//
	};
	//get M-norm
	auto& utMu = errData.globM.M();
	fCalcErrMatr(gM.getM(), EigenData.normEigenVecs.get(), utMu );
	errData.globM.hasM=true;
	//
	//get K-norm
	auto& utKu = errData.globM.K();
	fCalcErrMatr(gM.getK(), EigenData.normEigenVecs.get(), utKu );
	// extract the wi*wi
	for(auto it=0; it<utKu.countRows(); ++it)
	{
		utKu.getElem(it,it) -= Omega[it]*Omega[it];
	}
	//
	errData.globM.hasK=true;
}