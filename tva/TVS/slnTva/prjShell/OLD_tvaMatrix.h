// proxy for matrix classes
#pragma once
#include "tvaUtil.h"
//
#include <..\..\..\..\_addLibs\Eigen\source/Eigen/Core>
#include <..\..\..\..\_addLibs\Eigen\source/Eigen/LU>
#include <..\..\..\..\_addLibs\Eigen\source/Eigen/Eigenvalues>
#include <..\..\..\..\_addLibs\Eigen\source/Eigen\src\Eigenvalues\EigenSolver.h>
#include <..\..\..\..\_addLibs\Eigen\source/Eigen/src/Core/Transpose.h>
#include <..\..\..\..\_addLibs\Eigen\source\Eigen\src\LU\FullPivLU.h>
#include <algorithm>
#include <list>
#include <string>

namespace tva
{
	template<int T>
	class AbstractMatrix
	{
	public:
	};

	namespace codeTypeMatrix
	{
		enum
		{
			eigen=220
		};
	}

	class tMatr : protected AbstractMatrix<codeTypeMatrix::eigen>
	{
		friend class EigenSolver;
	public:
		typedef Eigen::Matrix<double,-1,-1,0,-1,-1> tPrivateData;

		typedef Eigen::Transpose<tPrivateData> tTransposeMatr;

		explicit tMatr(){}

		//explicit tMatr(const tMatr& rv):data_(rv.data_){}

		explicit tMatr(const int& sizeRow, const int& sizeCol)
			:data_(sizeRow, sizeCol)
		{
		}

		explicit tMatr(const tva::vecReal& vec)
		{
			const auto& n=vec.size();
			data_ = tPrivateData(n,1);
			size_t i(0);
			for(; i<n; ++i)
			{
				data_(i,0) = vec[i];
			}
		}

		explicit tMatr(const tva::vecvecReal& vecvec)
		{
			const auto& m=vecvec.size();
			if (m==0)
			{
				data_ = tPrivateData(0,0);
				return;
			}
			//
			const auto& n=vecvec[0].size();
			resize(n,m);
			//data_ = tPrivateData(n,m);
			for (int j=0; j<m; ++j)
			{
				const auto& v=vecvec[j];
				for(int i=0; i<n; ++i)
				{
					data_(i,j) = v[i];
				}
			}
		}

		tMatr operator*(const tMatr& rv)const
		{
			tMatr res;
			res.data_ = data_ * rv.data_;
			//
			return std::move(res);
		}

		tMatr& operator*=(const double& mult)
		{
			for (int i=0; i<countRows(); ++i)
			{
				for (int j=0; j<countCols(); ++j)
				{
					getElem(i, j) *= mult;
				}
			}
			return *this;
		}

		tMatr operator*(const double& mult)const
		{
			tMatr res;
			res.resize(this->countRows(),this->countCols());
			for (int i=0; i<countRows(); ++i)
			{
				for (int j=0; j<countCols(); ++j)
				{
					res.getElem(i, j) = this->getElem(i,j)*mult;
				}
			}
			return std::move(res);
		}

		tMatr operator-(const tMatr& rv)const
		{
			tMatr res;
			res.data_ = data_ - rv.data_;
			//
			return std::move(res);
		}
		tMatr operator+(const tMatr& rv)const
		{
			tMatr res;
			res.data_ = data_ + rv.data_;
			//
			return std::move(res);
		}

		tMatr t()const//transpose
		{
			tMatr res;
			res.data_ = data_.transpose();
			//
			return std::move(res);
		}

		//
		void resize(const int& nR, const int& nC)
		{
			data_.resize(nR,nC);
			//_data.t
		}

		void fill(const double& val)
		{
			for(auto i=0;i<data_.rows();++i)
			{
				for (auto j=0;j<data_.cols();++j)
				{
					data_(i,j)=val;
				}
			}
		}

		void fill(const tva::tMatr& m)
		{
			for(auto i=0;i<m.data_.rows();++i)
			{
				for (auto j=0;j<m.data_.cols();++j)
				{
					data_(i,j)=m.getElem(i,j);
				}
			}
		}

		void fill(const tva::tMatr& m, const int& rowStart, const int& colStart)
		{
			for(auto i=0;i<m.data_.rows();++i)
			{
				for (auto j=0;j<m.data_.cols();++j)
				{
					data_(i+rowStart,j+colStart)=m.getElem(i,j);
				}
			}
		}

		void sumUp(const tva::tMatr& m, const int& rowStart, const int& colStart)
		{
			for(auto i=0;i<m.data_.rows();++i)
			{
				for (auto j=0;j<m.data_.cols();++j)
				{
					data_(i+rowStart,j+colStart)+=m.getElem(i,j);
				}
			}
		}

		const double& getElem (const int& r,const int&c)const
		{
			return data_(r,c);
		}
		double& getElem (const int& r,const int&c)
		{
			return data_(r,c);
		}

		const int countCols()const{return data_.cols();}
		const int countRows()const{return data_.rows();}

		tva::tMatr getSolve(const tva::tMatr& b)const
		{
			tva::tMatr res;
			res.data_ = data_.fullPivLu().solve(b.data_);
			//
			return std::move(res);
		}

		tva::vecReal toVec()const
		{
			//Q_ASSERT_X(countCols()==1, "error", "matrix isnt vector");
			//
			tva::vecReal res;
			for (int i=0;i<countRows();++i)
			{
				res.push_back(getElem(i,0));
			}
			return std::move(res);
		}

		void symm_UpperToDown()
		{
			for(auto i=0;i<data_.rows();++i)
			{
				for (auto j=i;j<data_.cols();++j)
				{
					data_(j,i)=data_(i,j);
				}
			}
		}

		std::list<std::string> toString()const
		{
			std::list<std::string> res;
			for (int i=0;i<countRows();++i)
			{
				std::string sub;
				for (int j=0;j<countCols();++j)
				{
					auto a = QString(" %1").arg(getElem(i,j));
					sub.append(a.toStdString());//.push_back(getElem(i,0));
				}
				res.push_back(sub);
				//res.append("\n");
			}
			return std::move(res);
		}

		const tPrivateData& data()const{return data_;}
		tPrivateData& data(){return data_;}

	protected:
	private:
		tPrivateData data_;
	};

	inline
	double getDoubleOuterMult(const tva::vecReal& vec1, const tva::tMatr& matr, const tva::vecReal& vec2)
	{
		double res(-1);
		//
		auto vu1 = tva::tMatr(vec1);
		auto vut= vu1.t();
		auto vu2 = tva::tMatr(vec2);
		//
		auto m1= vut*matr;
		auto mult= m1*vu2;
		//
		res = mult.getElem(0,0);
		//
		return std::move(res);
	}

	inline
	bool checkSymmetry(const tva::tMatr& m)
	{
		const auto& r = m.countRows();
		const auto& c = m.countCols();
		//
		if (r != c)
		{
			 return false;
		}
		//
		for (int i=0; i<r; ++i)
		{
			for (int j=0; j<c; ++j)
			{
				if(i!=j)
				{
					if (m.getElem(i,j) != m.getElem(j,i))
					{
						return false;
					}
				}
			}
		}
		//
		return true;
	}

	class EigenSolver
	{
	public:
		explicit EigenSolver(const tva::tMatr& M, const tva::tMatr& K)
		{
			const auto MK=M.data_.inverse()*K.data_;
			//
			Eigen::EigenSolver<tMatr::tPrivateData> es(MK);
			//
			const auto eigValues = es.eigenvalues();
			_vals.clear();
			for (auto i=0; i<eigValues.rows(); ++i)
			{
				_vals.push_back(eigValues(i,0).real());//TODO: add complex value
			}
			//cout<<"eigenVectors"<<endl;cout<<es.eigenvectors()<<endl;
			//
			const auto v = es.eigenvectors();
			//normalyze
			const auto d = v.transpose() * M.data_ * v;
			double mult=1.0;//d(0,0).real();//TODO: add complex value
			//
			_vecs.clear();
			for (auto j=0;j<v.cols();j++)
			{
				tva::vecReal normV;
				for(auto i=0; i<v.rows();++i)
				{
					normV.push_back(v(i,j).real()/mult);
				}
				_vecs.push_back(normV);
			}
			//
		}

		const tva::vecReal& eigenValues()const{return _vals;};
		const tva::vecvecReal& eigenVectors()const{return _vecs;};

	private:
		tva::vecReal _vals;
		tva::vecvecReal _vecs;
	};

	typedef util::MayBe<tMatr> opMatrix;
	//
	namespace proc
	{
		void getDiagonal(const tva::tMatr& sM, tva::vecReal& vec);
		void getOutDiagonal(const tva::tMatr& sM, tva::vecReal& vec);

		struct pivotElem
		{
			tva::opReal maxDiag;
			tva::opReal maxOutDiag;
		};
		void calcMaxDiagOutDiagElem(const tva::tMatr& sM, tva::proc::pivotElem& pivot);
	}
}

//
inline
void tva::proc::calcMaxDiagOutDiagElem( const tva::tMatr& sM, tva::proc::pivotElem& pivot )
{
	{
		tva::vecReal vecDiag;
		tva::proc::getDiagonal(sM, vecDiag);
		const auto& maxDiag = tva::util::getMaxElement(vecDiag);
		pivot.maxDiag = maxDiag;
	}
	//
	{
		tva::vecReal vecOutDiag;
		tva::proc::getOutDiagonal(sM, vecOutDiag);
		const auto& maxOutDiag = tva::util::getMaxElement(vecOutDiag);
		pivot.maxOutDiag = maxOutDiag;
	}
}

inline
void tva::proc::getDiagonal( const tva::tMatr& sM, tva::vecReal& vec )
{
	vec.clear();
	//const auto& n = sM.Ncols();
	const auto& n = sM.countCols();
	for (auto it=0; it<n; ++it)
	{
		vec.push_back(fabs(sM.getElem(it,it)));
	}
	//
	return;
}

inline
void tva::proc::getOutDiagonal( const tva::tMatr& sM, tva::vecReal& vec )
{
	vec.clear();
	const auto& n = sM.countCols();
	for (auto i=0; i<n; ++i)
	{
		for(auto j=i+1; j<n; ++j)
		{
			vec.push_back(fabs(sM.getElem(i,j)));
		}
	}
	//
	return;
}