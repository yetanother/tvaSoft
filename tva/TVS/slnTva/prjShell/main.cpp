#include <QtGui/QApplication>
#include "mainwindow.h"
#include "logger.h"
#include "shell.h"
//
//#include "newmat.h"
#include <iostream>
//#include "newmatio.h"                // need matrix output routines
//#include "newmatap.h"
#include "eigenCore.h"
#include "eigenCore.h"

using namespace std;

//
//void test1(const int& size, const double& initValue)
//{
//	SymmetricMatrix M;
//	{//make M
//		M.ReSize(3);
//		const double val(3.8e-6);
//		const double mult(2.0);
//		double B[3][3] = { { val, 0, 0 }, { 0, mult*val, 0 }, { 0, 0, val } };
//		for(int i=0;i<size;++i)
//		{
//			for (int j=0;j<size;++j)
//			{
//				M.element(i,j)=B[i][j];
//			}
//		}
//	}
//	//
//	SymmetricMatrix K;
//	{//make M
//		K.ReSize(3);
//		const double mult(2.0);
//		const double val(161);
//		double B[3][3] = { { val, -val, 0 }, { -val, val*mult, -val}, { 0, -val, val } };
//
//		for(int i=0;i<size;++i)
//		{
//			for (int j=0;j<size;++j)
//			{
//				K.element(i,j)=B[i][j];
//			}
//		}
//	}
//	const auto MK=M.i()*K;
//
//	////
//	cout<<"M"<<endl;
//	cout<<M<<endl;
//	//
//	cout<<"K"<<endl;
//	cout<<K<<endl;
//	//////
//	cout<<"inv(M)*K"<<endl;
//	cout<<MK<<endl;
//	////
//	DiagonalMatrix eigenVals;eigenVals.ReSize(3);
//	Matrix eigenVecs;
//	eigenVecs.ReSize(3,3);
//	//mat eigvec;
//	//EigenValues(MK, eigenVals, eigenVecs);
//	int codeError=tva::solveEigenProblem( M, K, eigenVals, eigenVecs );
//	//
//	//
//	cout<<"eigenValues"<<endl;
//	cout<<eigenVals<<endl;
//	cout<<"eigenVectors"<<endl;
//	cout<<eigenVecs<<endl;
//}


int main(int argc, char *argv[])
{
  QApplication::setGraphicsSystem("raster");
  QApplication a(argc, argv);
  //
  {//new test newMatLib
	  //test1(3, 0.0);
  }
  //
  tva::Shell shell;
  MainWindow w;
  tva::Logger logger(w.getWidgetLogger());
  shell.setMainWnd(&w);
  shell.setLogger(&logger);
  w.setShell(&shell);
  shell.addLineToLogger("init Shell");
  shell.show();
  //
  return a.exec();
}
