#pragma once

#include <vector>
#include "tvaUtil.h"
#include <functional>
#include "tvaMatrix.h"

namespace tva
{
	const int nulInt(0);
	const Real nulReal(0.0);
	const Real defErr(1e-5);

	namespace local
	{
		static const int sizeLocalMatrixK(2);
		void makeK(const Real&value, tva::tMatr& local);
		void makeM(const Real&value, tva::tMatr& local);
	}

	struct globMatrises
	{
		bool hasM;
		//

		bool hasK;
		//
		globMatrises():hasM(false),hasK(false){};

		const tva::tMatr& getM()const{return _M;}
		tva::tMatr& M(){return _M;}
		const tva::tMatr& getK()const{return _K;}
		tva::tMatr& K(){return _K;}
		//void check();
	private:
		tva::tMatr _M;
		tva::tMatr _K;
	};

	int calcGlobMatrSize(const int & discCount);

	int/*codeError*/ simpleSolveEigenProblem(const tva::tMatr& M, const tva::tMatr& K, std::vector<double>& eigenVals, tva::vecvecReal& eigenVecs);
	bool doSolveEigenProblem(const tva::tMatr& M, const tva::tMatr& K, tva::vecReal& Omega, tva::vecvecReal& normVecs);
	//
	void calcOmega(const std::vector<double>& eigenVals, std::vector<double>& Omega);
	int/*codeError*/ getNormalEigenVecs(const tva::vecvecReal& vecvecReal, const tva::tMatr& M, tva::vecvecReal& normalizedEigenVecs);

	typedef std::function<void( const Real&, tva::tMatr& )> functor_Real_Matr;
	//
	namespace stub
	{
		struct regularData
		{
			int n;//count of disc
			Real mipI;//moment of inertria
			Real strengh;//torsional strengh

			regularData():n(nulInt), mipI(nulReal), strengh(nulReal){}

			regularData(const int& n_, const Real& mipI_, const Real &strength_)
				:n(n_), mipI(mipI_), strengh(strength_){}
		};
		//
		void makeRegularVector(const Real& value, const int&size, std::vector<Real>& column);
		void vector2diagonal(const std::vector<Real>& vec, tva::tMatr& sm);
		void diagMatrToVector(const tva::tMatr& m, tva::vecReal& vec);

		void makeRegularMatrM(const Real&value,const int&size, tva::tMatr& M);
		void makeRegularMatrK(const Real&value,const int&size, tva::tMatr& K);
		void makeRegularMatrByVectorValue(tva::functor_Real_Matr &funcMakeLocal, const tva::vecReal& vec, tva::tMatr& K);

		void makeRegularMatrises(const regularData& data, globMatrises& gM);
		//
		void addLocMatrToGlobMatr(const tva::tMatr& locM, tva::tMatr& globM, const int& startIndex);
		void addLastLocMatrToGlobMatr(const tva::tMatr& locM, tva::tMatr& globM, const int& startIndex);
	}
	namespace byShaft
	{
	}

	namespace e_b
	{
		struct policy
		{
			static const int sizeLocalMatrix = 8;

			static const int step = sizeLocalMatrix/2;

			static const int start_U_position = 0;
			static const int start_Psi_position = 1;//du/dz
			static const int start_V_position = 2;
			static const int start_Theta_position = 3;//dv/dz
		};

		template<typename T>
		void make_M_E_B(const double& A, const double& L, const double& ro, tva::tMatr& out);

		template<>inline
		void make_M_E_B<policy>(const double& A, const double& L, const double& ro, tva::tMatr& out)
		{
			const auto& n = policy::sizeLocalMatrix;
			out.resize(n, n);
			out.fill(0.0);
			//
			const double cmnMult = ro*A*L/420.0;
			//
			const double _156 = 156.0*(cmnMult);
			const double _22L = 22.0*L*(cmnMult);
			const double _4LL = 4.0*L*L*(cmnMult);
			//
			//
			tva::tMatr leftUppercorner;
			const auto& nHalf = n/2;
			leftUppercorner.resize(nHalf, nHalf);
			leftUppercorner.fill(0.0);
			tva::tMatr rightUppercorner = leftUppercorner;
			//
			{
				auto&m = leftUppercorner;
				//
				m.getElem(0,0) = _156;m.getElem(0,1) = _22L;
				m.getElem(1,1) = _4LL;
				m.getElem(2,2) = _156;m.getElem(2,3) = -_22L;
				m.getElem(3,3) = _4LL;
			}
			out.fill(leftUppercorner);
			//
			auto rightDown = leftUppercorner;
			{
				auto& m = rightDown;
				m.getElem(0,1) *= -1;
				m.getElem(2,3) *= -1;
			}
			out.fill(rightDown, nHalf, nHalf);
			//
			{
				const double _54 = 54.0*(cmnMult);
				const double _13L = 13.0*L*(cmnMult);
				const double _3LL = 3.0*L*L*(cmnMult);
				//
				auto&m = rightUppercorner;
				//
				m.getElem(0,0) = _54;m.getElem(0,1) = -_13L;
				m.getElem(1,0) = _13L;m.getElem(1,1) = -_3LL;
				m.getElem(2,2) = _54;m.getElem(2,3) = _13L;
				m.getElem(3,2) = -_13L;	m.getElem(3,3) = -_3LL;
			}
			out.fill(rightUppercorner, 0, nHalf);
			//

			out.symm_UpperToDown();
		}
		//
		template<typename T>
		void make_K(const double& E, const double& I, const double& L, bool useTimoshCorrection, const double& mu,const double& A, tva::tMatr& out);

		template<>inline
			void make_K<policy>
			(const double& E, const double& I, const double& L, bool useTimoshCorrection, const double& mu,const double& A, tva::tMatr& out)
		{
			const auto& n = policy::sizeLocalMatrix;
			out.resize(n, n);
			out.fill(0.0);
			//

			const double a = (useTimoshCorrection)? (12*2*(1+mu)*I/(A*L*L)) : 0.0;
			const double cmnMult = E*I/((L*L*L)*(1+a));
			//
			const double _12 = 12.0*(cmnMult);
			const double _6L = 6.0*L*(cmnMult);
			const double _4LL = 4.0*L*L*(cmnMult);
			//
			tva::tMatr leftUppercorner;
			const auto& nHalf = n/2;
			leftUppercorner.resize(nHalf, nHalf);
			leftUppercorner.fill(0.0);
			//
			{
				auto&m = leftUppercorner;
				//
				m.getElem(0,0) = _12;m.getElem(0,1) = _6L;
				m.getElem(1,1) = _4LL+a;
				m.getElem(2,2) = _12;m.getElem(2,3) = -_6L;
				m.getElem(3,3) = _4LL+a;
			}
			tva::tMatr rightUppercorner = leftUppercorner;
			//
			out.fill(leftUppercorner);
			//
			auto rightDown = leftUppercorner;
			{
				auto& m = rightDown;
				m.getElem(0,1) *= -1;
				m.getElem(2,3) *= -1;
			}
			out.fill(rightDown, nHalf, nHalf);
			//
			{
				const double _2LL = 2.0*L*L*(cmnMult);
				//
				auto&m = rightUppercorner;
				//
				m.getElem(0,0) *= -1;
				m.getElem(1,0) = -m.getElem(0,1);m.getElem(1,1)=_2LL-a;
				m.getElem(2,2) *= -1;
				m.getElem(3,2) = -m.getElem(2,3);m.getElem(3,3)=_2LL-a;
			}
			out.fill(rightUppercorner, 0, nHalf);
			//

			out.symm_UpperToDown();
		}

		template<typename T>
		void make_M_ri(const double& I, const double& L, const double& ro, tva::tMatr& out);

		template<>
		inline
		void make_M_ri<policy>(const double& I, const double& L, const double& ro, tva::tMatr& out)
		{
			const auto& n = policy::sizeLocalMatrix;
			out.resize(n, n);
			out.fill(0.0);
			//
			const double cmnMult = ro*I/(30.0*L);
			//
			const double _36 = 36.0*(cmnMult);
			const double _3L = 3.0*L*(cmnMult);
			const double _4LL = 4.0*L*L*(cmnMult);
			//
			tva::tMatr leftUppercorner;
			const auto& nHalf = n/2;
			leftUppercorner.resize(nHalf, nHalf);
			leftUppercorner.fill(0.0);
			tva::tMatr rightUppercorner = leftUppercorner;
			//
			{
				auto&m = leftUppercorner;
				//
				m.getElem(0,0) = _36;m.getElem(0,1) = _3L;
				m.getElem(1,1) = _4LL;
				m.getElem(2,2) = _36;m.getElem(2,3) = -_3L;
				m.getElem(3,3) = _4LL;
			}
			out.fill(leftUppercorner);
			//
			auto rightDown = leftUppercorner;
			{
				auto& m = rightDown;
				m.getElem(0,1) *= -1;
				m.getElem(2,3) *= -1;
			}
			out.fill(rightDown, nHalf, nHalf);
			//
			{
				const double _1LL = 1.0*L*L*(cmnMult);
				//
				auto&m = rightUppercorner;
				//
				m.getElem(0,0) = -_36;m.getElem(0,1) = _3L;
				m.getElem(1,0) = -_3L;m.getElem(1,1) = -_1LL;
				m.getElem(2,2) = -_36;m.getElem(2,3) = _3L;
				m.getElem(3,2) = _3L;	m.getElem(3,3) = -_1LL;
			}
			out.fill(rightUppercorner, 0, nHalf);
			//

			out.symm_UpperToDown();
		}
		//

		template<typename T>
		//inline
		void calcIndexForInsert(const int& elemIndex, int& out)
		{
			const auto& n = T::sizeLocalMatrix;

			if (elemIndex == 0)
			{
				out = 0;
			}
			else
			{
				out = (elemIndex)*n/2;
				//out = (elemIndex)*(n/2);;//elemIndex*n-(n/2);//-1;
			}
		}

		template<typename T>
		//inline
		int shiftIndexForNextFE()
		{
			return T::sizeLocalMatrix/2;
		}

		template<typename T>inline
		void calcSizeGlobMatr(const int& countFE, int& size)
		{
			const auto& n = T::sizeLocalMatrix;

			size = n+(countFE-1)*n/2;
			//if (countFE>1)
			//{
			//	size = n*(countFE-1);;//(n/2) + (countFE-1)*n;
			//	return;
			//}
			//if (countFE == 1)
			//{
			//	size = n;
			//}
			//else
			//{
			//	size = -1;//error
			//}
		}

		template<typename T>
		void addLocMatrToGlobMatr(const tva::tMatr& locM,const int& elemIndex, tva::tMatr& globM );

		template<>
		inline
		void addLocMatrToGlobMatr<policy>(const tva::tMatr& locM, const int& elemIndex, tva::tMatr& globM)
		{
			int startIndex;
			calcIndexForInsert<policy>(elemIndex, startIndex);
			//
			globM.sumUp(locM, startIndex, startIndex);
			//
		}

		template<typename T>
		void makeGlobMatr_K
		(const tva::vecReal& vE, const tva::vecReal& vI, const tva::vecReal& vL, bool useTimoshCorrection, const tva::vecReal& vmu,const tva::vecReal& vA, tva::tMatr& out);

		template<>inline
		void makeGlobMatr_K<policy>
		(const tva::vecReal& vE, const tva::vecReal& vI, const tva::vecReal& vL, bool useTimoshCorrection, const tva::vecReal& vmu,const tva::vecReal& vA, tva::tMatr& out)
		{
			//
			int n = vE.size();
			int Size;
			calcSizeGlobMatr<policy>(n, Size);

			out.resize(Size, Size);
			out.fill(0.0);
			//
			tva::tMatr localFE;
			//
			for (int i=0; i<n; ++i)
			{
				make_K<policy>(vE[i], vI[i], vL[i], useTimoshCorrection, vmu[i], vA[i], localFE);
				//
				addLocMatrToGlobMatr<policy>(localFE, i, out);
			}
		}

		template<typename T>
		void makeGlobMatr_M(const tva::vecReal& vA, const tva::vecReal& vL, const tva::vecReal& vro, const tva::vecReal& vI, tva::tMatr& out);

		template<>inline
		void makeGlobMatr_M<policy>(const tva::vecReal& vA, const tva::vecReal& vL, const tva::vecReal& vro, const tva::vecReal& vI, tva::tMatr& out)
		{
			int n = vA.size();
			int Size;
			calcSizeGlobMatr<policy>(n, Size);

			out.resize(Size, Size);
			out.fill(0.0);
			//
			tva::tMatr localFE_EB;
			tva::tMatr localFE_RI;
			//
			for (int i=0; i<n; ++i)
			{
				make_M_E_B<policy>(vA[i], vL[i], vro[i], localFE_EB);
				make_M_ri<policy>(vI[i], vL[i], vro[i], localFE_RI);
				//				//
				addLocMatrToGlobMatr<policy>(localFE_EB, i, out);
				//addLocMatrToGlobMatr<policy>(localFE_RI, i, out);
			}
		}

		namespace post
		{
			inline
			void getRegularSubVector(const tva::vecReal& raw, const int& start, const int& step, tva::vecReal& out)
			{
				out.clear();
				int i=start;
				while(i<raw.size())
				{
					out.push_back(raw[i]);
					i +=step;
				}
			}

			template<typename T>
			void get_U (const tva::vecReal& raw, tva::vecReal& out);

			template<>inline
			void get_U<policy> (const tva::vecReal& raw, tva::vecReal& out)
			{
				getRegularSubVector(raw, policy::start_U_position, policy::step, out);
			}

			template<typename T>
			void get_Psi (const tva::vecReal& raw, tva::vecReal& out);//dU/dz

			template<>inline
			void get_Psi<policy> (const tva::vecReal& raw, tva::vecReal& out)
			{
				getRegularSubVector(raw, policy::start_Psi_position, policy::step, out);
			}

			template<typename T>
			void get_V (const tva::vecReal& raw, tva::vecReal& out);

			template<>inline
			void get_V<policy> (const tva::vecReal& raw, tva::vecReal& out)
			{
				getRegularSubVector(raw, policy::start_V_position, policy::step, out);
			}

			template<typename T>
			void get_Theta (const tva::vecReal& raw, tva::vecReal& out);//dU/dz

			template<>inline
			void get_Theta<policy> (const tva::vecReal& raw, tva::vecReal& out)
			{
				getRegularSubVector(raw, policy::start_Theta_position, policy::step, out);
			}
		}
	}
}