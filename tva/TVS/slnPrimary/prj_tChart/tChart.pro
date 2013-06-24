#
include(../cmnPro.pro)
include(../cmnLibSetup.pro)
#
include(prj_tChart.pro)
#
win32:
{
 INCLUDEPATH += ../
}

#SOURCES +=  ../tvaUtil.cpp

HEADERS  += ../primary.h

SOURCES += ../../../../_addLibs/QCustomPlot/qcustomplot.cpp

HEADERS  += ../../../../_addLibs/QCustomPlot/qcustomplot.h