######################################################################
# Automatically generated by qmake (2.01a) ?? 15. ??? 11:42:02 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += cmnExtInclude.h \
           tvaMatrix.h \
           ../tva_interval.h \
           ../primary.h \
           ../../../../_addLibs/Eigen/source/Eigen/Core \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/DisableStupidWarnings.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/Macros.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/MKL_support.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/Constants.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/ForwardDeclarations.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/Meta.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/XprHelper.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/StaticAssert.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/Memory.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/NumTraits.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/MathFunctions.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/GenericPacketMath.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/SSE/PacketMath.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/SSE/MathFunctions.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/SSE/Complex.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/AltiVec/PacketMath.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/AltiVec/Complex.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/NEON/PacketMath.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/NEON/Complex.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/arch/Default/Settings.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Functors.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/DenseCoeffsBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/DenseBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/BlockMethods.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/MatrixBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/CommonCwiseUnaryOps.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/CommonCwiseBinaryOps.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/MatrixCwiseUnaryOps.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/MatrixCwiseBinaryOps.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/EigenBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Assign.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/BlasUtil.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/DenseStorage.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/NestByValue.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/ForceAlignedAccess.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/ReturnByValue.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/NoAlias.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/PlainObjectBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Matrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Array.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/CwiseBinaryOp.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/CwiseUnaryOp.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/CwiseNullaryOp.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/CwiseUnaryView.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/SelfCwiseBinaryOp.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Dot.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/StableNorm.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/MapBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Stride.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Map.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Block.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/VectorBlock.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Transpose.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/DiagonalMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Diagonal.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/DiagonalProduct.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/PermutationMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Transpositions.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Redux.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Visitor.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Fuzzy.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/IO.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Swap.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/CommaInitializer.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Flagged.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/ProductBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/GeneralProduct.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/TriangularMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/SelfAdjointView.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralBlockPanelKernel.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/Parallelizer.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/CoeffBasedProduct.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralMatrixVector.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralMatrixMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/SolveTriangular.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralMatrixMatrixTriangular.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/SelfadjointMatrixVector.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/SelfadjointMatrixMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/SelfadjointProduct.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/SelfadjointRank2Update.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularMatrixVector.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularMatrixMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularSolverMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularSolverVector.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/BandMatrix.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/BooleanRedux.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Select.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/VectorwiseOp.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Random.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Replicate.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Reverse.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/ArrayBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/ArrayCwiseUnaryOps.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/plugins/ArrayCwiseBinaryOps.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/ArrayWrapper.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralMatrixMatrix_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralMatrixVector_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/GeneralMatrixMatrixTriangular_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/SelfadjointMatrixMatrix_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/SelfadjointMatrixVector_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularMatrixMatrix_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularMatrixVector_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/products/TriangularSolverMatrix_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/Assign_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/GlobalFunctions.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Core/util/ReenableStupidWarnings.h \
           ../../../../_addLibs/Eigen/source/Eigen/Eigen2Support \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Macros.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Memory.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Meta.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Lazy.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Cwise.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/CwiseOperators.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/TriangularSolver.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Block.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/VectorBlock.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Minor.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/MathFunctions.h \
           ../../../../_addLibs/Eigen/source/Eigen/LU \
           ../../../../_addLibs/Eigen/source/Eigen/src/misc/Solve.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/misc/Kernel.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/misc/Image.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/LU/FullPivLU.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/LU/PartialPivLU.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/LU/PartialPivLU_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/LU/Determinant.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/LU/Inverse.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/LU/arch/Inverse_SSE.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/LU.h \
           ../../../../_addLibs/Eigen/source/Eigen/Eigenvalues \
           ../../../../_addLibs/Eigen/source/Eigen/Cholesky \
           ../../../../_addLibs/Eigen/source/Eigen/src/Cholesky/LLT.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Cholesky/LDLT.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Cholesky/LLT_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/Jacobi \
           ../../../../_addLibs/Eigen/source/Eigen/src/Jacobi/Jacobi.h \
           ../../../../_addLibs/Eigen/source/Eigen/Householder \
           ../../../../_addLibs/Eigen/source/Eigen/src/Householder/Householder.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Householder/HouseholderSequence.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Householder/BlockHouseholder.h \
           ../../../../_addLibs/Eigen/source/Eigen/Geometry \
           ../../../../_addLibs/Eigen/source/Eigen/SVD \
           ../../../../_addLibs/Eigen/source/Eigen/QR \
           ../../../../_addLibs/Eigen/source/Eigen/src/QR/HouseholderQR.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/QR/FullPivHouseholderQR.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/QR/ColPivHouseholderQR.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/QR/HouseholderQR_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/QR/ColPivHouseholderQR_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/QR.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/SVD/JacobiSVD.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/SVD/JacobiSVD_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/SVD/UpperBidiagonalization.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/SVD.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/OrthoMethods.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/EulerAngles.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Homogeneous.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/RotationBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Rotation2D.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Quaternion.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/AngleAxis.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Transform.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Translation.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Scaling.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Hyperplane.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/ParametrizedLine.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/AlignedBox.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/Umeyama.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Geometry/arch/Geometry_SSE.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/All.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/RotationBase.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/Rotation2D.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/Quaternion.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/AngleAxis.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/Transform.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/Translation.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/Scaling.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/AlignedBox.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/Hyperplane.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigen2Support/Geometry/ParametrizedLine.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/Tridiagonalization.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/RealSchur.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/HessenbergDecomposition.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/EigenSolver.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/SelfAdjointEigenSolver.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/GeneralizedSelfAdjointEigenSolver.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/ComplexSchur.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/ComplexEigenSolver.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/MatrixBaseEigenvalues.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/RealSchur_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/ComplexSchur_MKL.h \
           ../../../../_addLibs/Eigen/source/Eigen/src/Eigenvalues/SelfAdjointEigenSolver_MKL.h
SOURCES += tvaMatrix.cpp ../tva_interval.cpp
