#
include(../cmnPro.pro)
include(../cmnLibSetup.pro)
#
include(prj_tMAtr.pro)
#

win32:
{
 INCLUDEPATH += ../

}

SOURCES +=  ../tva_interval.cpp
			#../../../../_addLibs/QCustomPlot/qcustomplot.cpp \
			#../../slnProc/prjEigenCore/eigenCore.cpp\
			#../../slnPostProc/postProc.cpp
        

HEADERS  += ../tva_interval.h
#../../../../_addLibs/QCustomPlot/qcustomplot.h \
			#../../slnProc/prjEigenCore/eigenCore.h\
#			../../slnPostProc/postProc.h