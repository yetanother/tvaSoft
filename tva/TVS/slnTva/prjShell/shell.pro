include(prjShell.pro)
#matrix method for torsion vibration calculation, modal analisys
QT       += core gui
#TARGET = plot-examples
TEMPLATE = app
#CONFIG += console

DEPENDPATH += . UI

CONFIG += debug_and_release

win32:
{
 QMAKE_LIBDIR_QT = ../../../../_addLibs/qt/lib
 QMAKE_INCDIR_QT = ../../../../_addLibs/qt/include	
 INCLUDEPATH += ../prjTva \
 ../../../../_addLibs/QCustomPlot\
  				../../slnPrimary \
 				../../slnProc/prjEigenCore\
 				../../slnPreProc
 INCLUDEPATH += ../../../../_addLibs/newMat/source \
 				../../slnPrimary/prj_tMatr


 CONFIG(debug, debug|release) {
 	QMAKE_LIBDIR_QT += ../../slnPrimary/prj_tMatr/debug
 	QMAKE_LIBDIR_QT += ../../slnPrimary/prj_tChart/debug
 	LIBS += tMatr.lib tChart.lib
    # TARGET = debug_binary
 } else {
 QMAKE_LIBDIR_QT += ../../slnPrimary/prj_tMatr/release
 QMAKE_LIBDIR_QT += ../../slnPrimary/prj_tChart/release
 LIBS += tMatr.lib tChart.lib
    # TARGET = release_binary
 }
}

#unix:LIBS += -L/usr/local/lib -lmath

SOURCES += ../../slnPostProc/postProc.cpp

HEADERS  += ../../slnPostProc/postProc.h

SOURCES -= OLD_tvaUtil.cpp	
HEADERS -= OLD_tvaUtil.h OLD_tvaMatrix.h
