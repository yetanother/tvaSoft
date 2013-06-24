#pragma once
#include "..\slnProc\prjEigenCore\eigenCore.h"
#include "..\src\corelib\global\qglobal.h"
#include "wizards.h"
#include "preProc.h"
#include "tvaUtil.h"

namespace tva
{
	class eigenVecs
	{
	private:
		tva::vecvecReal data;
		bool init;

		explicit eigenVecs();
		public:
			static eigenVecs makeNewEigenVecs();
			//
			void setData(const tva::vecvecReal& m);

		tva::vecvecReal& get(){return data;}
		const tva::vecvecReal& get()const{return data;}

		bool isInit()const;
	};

	struct errEigenData
	{
		tva::globMatrises globM;
		tva::proc::pivotElem pivotM;
		tva::proc::pivotElem pivotK;
	};

	struct eigenData
	{
		tva::vecReal vecIndex;
		tva::opVecReal Omega;// собственные частоты
		eigenVecs normEigenVecs;

		eigenData():normEigenVecs(tva::eigenVecs::makeNewEigenVecs()){}
	};

	inline
double rk4 ( double t0, double u0, double dt, double f ( double t, double u ) )
//source from http://people.sc.fsu.edu/~jburkardt/cpp_src/rk4/rk4.cpp
//****************************************************************************80
//
//  Purpose:
//
//    RK4 takes one Runge-Kutta step.
//
//  Discussion:
//
//    It is assumed that an initial value problem, of the form
//
//      du/dt = f ( t, u )
//      u(t0) = u0
//
//    is being solved.
//
//    If the user can supply current values of t, u, a stepsize dt, and a
//    function to evaluate the derivative, this function can compute the
//    fourth-order Runge Kutta estimate to the solution at time t+dt.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    13 July 2011
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, double T0, the current time.
//
//    Input, double U0, the solution estimate at the current time.
//
//    Input, double DT, the time step.
//
//    Input, double F ( double T, double U ), a function which evaluates
//    the derivative, or right hand side of the problem.
//
//    Output, double RK4, the fourth-order Runge-Kutta solution estimate
//    at time T0+DT.
//
{
  double f1;
  double f2;
  double f3;
  double f4;
  double u1;
//
//  Get four sample values of the derivative.
//
  f1 = f ( t0,          u0 );
  f2 = f ( t0 + dt / 2, u0 + dt * f1 / 2 );
  f3 = f ( t0 + dt / 2, u0 + dt * f2 / 2 );
  f4 = f ( t0 + dt,     u0 + dt * f3 );
//
//  Combine them to estimate the solution U1 at time T1 = T0 + DT.
//
  u1 = u0 + dt * ( f1 + 2.0 * f2 + 2.0 * f3 + f4 ) / 6.0;

  return u1;
}

namespace proto
{
	typedef void(*rawPointer)(const double& , tva::tMatr& ) ;
	typedef std::function<void(const double&, tva::tMatr&)> tFunc;

	inline
		void fCalcRightSide(const double& t, tva::tMatr& rs)
	{
		//static const double F_r=1.0;
		//F_outer.fill(0.0);
		rs.getElem(rs.countRows()-1,0) = 1.0;
		//
	};

	inline
	tva::tMatr fCalc_yt2(const tva::tMatr& M, const tva::tMatr& C, const tva::tMatr& K, const tva::tMatr&F,
		const tva::tMatr&y, const tva::tMatr&yt)
	{
		tva::tMatr yt2;
		auto dbg1=M.toString();
		//
		auto b=F-(C*yt+K*y);
		yt2 = M.getSolve(b);
		//
		auto dbg12=M.toString();

		auto dbg2=C.toString();
		auto dbg3=K.toString();
		auto dbg4=F.toString();
		auto dbg5=b.toString();
		auto dbg6=yt2.toString();
		//
		return yt2;
	};

	inline
	void calc
		(
			const tva::tMatr& M, const tva::tMatr& C, const tva::tMatr& K,
			const tva::vecReal& vy0, const tva::vecReal& vyt0,
			const double& t0, const double& dt, const int& Nstep,
			//rawPointer  fRightSide,
			//tFunc fRightSide,
			tva::vecvecReal &vy, tva::vecvecReal &vyt, tva::vecvecReal &vyt2,
			tva::vecReal &v_time
		)
	{
		vy.clear();vyt.clear();vyt2.clear();
		//
		tva::tMatr y0(vy0);
		tva::tMatr yt0(vyt0);
		//
		tva::tMatr F;
		{
			auto n = M.countRows();
			F.resize(n,1);
			F.fill(0.0);
			fCalcRightSide(t0, F);
		}
		//
		tva::tMatr yt2_0=fCalc_yt2(M, C, K, F, y0, yt0);
		//
		//tva::tMatr Meff=M*(1.0/(dt*dt))+C*(1.0/(2*dt));
		//
		auto dbg12=M.toString();
		auto dbg22=C.toString();
		//
		auto y_curr=y0;
		auto yt_curr=yt0;
		auto yt2_curr=yt2_0;
		//
		auto t_curr=t0;
		//
		auto y_m1=y0-yt0*dt+yt2_0*(dt*dt/2.0);
		auto y_prev = y_m1;
		//
		//
		for(int i=0; i<Nstep; ++i)
		{
			{
				vy.push_back(y_curr.toVec());
				vyt.push_back(yt_curr.toVec());
				vyt2.push_back(yt2_curr.toVec());
				v_time.push_back(t_curr);
			}
			//
			auto y_next=yt_curr*2*dt+y_prev;
			auto yt_next= yt2_curr*2*dt+yt_curr;
			t_curr+= dt;
			//swap
			{
				y_prev = y_curr;
				y_curr = y_next;
				yt_curr = yt_next;
				//
				fCalcRightSide(t0, F);
				yt2_curr=fCalc_yt2(M, C, K, F, y_curr, yt_curr);
			}
		}

		//tva::util::showMsg("finish calc");
	}

	inline
	tva::tMatr makeC(const tva::tMatr& M, const tva::tMatr& K, const double& alfa, const double& beta)
	{
		tva::tMatr C;
		{
			//
			auto a1=M;//model.gM.getM();
			a1*=alfa;
			auto a2=K;//model.gM.getK();
			a2*=beta;
			C=a1+a2;
		}
		//
		return C;
	}

	struct dynCalcData
	{
		struct initData
		{
			double t0, tk, dt;
			tva::tMatr M,C,K;
			tva::vecReal y0, yt0;
		};

		struct result
		{
			tva::vecvecReal vy, vyt, vy2t, vForce;
			tva::vecReal vTime;
		};
		//============

		initData initData_;
		result result_;
	};
}

	class Model
	{
	public:
		tva::globMatrises gM;
		tva::errEigenData errEigenProlemM;
		eigenData EigenData;
		proto::dynCalcData dynData;
		//
		const tva::rawData::descrComplexShaft& rawShaft()const{return _rawShaft;};
		tva::rawData::descrComplexShaft& rawShaft(){return _rawShaft;};

		void calcErrorEigenProblem(const tva::vecReal& Omega, tva::errEigenData& errData);

		Model();

	private:
		tva::rawData::descrComplexShaft _rawShaft;
	};
}