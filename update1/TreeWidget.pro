#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T16:06:39
#
#-------------------------------------------------

QT       += core gui  network  sql
QT       += serialport
#QT       += xlsx
QT       += sql
#为跨平台做准备
#macx {
#  QMAKE_CXXFLAGS += -F/Library/Frameworks
#  QMAKE_LFLAGS   += -F/Library/Frameworks
#  LIBS           += -framework HALCONCpp
#}
#else {
#  #defines
#  win32:DEFINES += WIN32
#  #includes//包含目录
#  INCLUDEPATH   += "$$(HALCONROOT)/include"
#  INCLUDEPATH   += "$$(HALCONROOT)/include/halconcpp"
#  #libs//依赖库文件
#  QMAKE_LIBDIR  += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
#  unix:LIBS     += -lhalconcpp -lhalcon -lXext -lX11 -ldl -lpthread
#  win32:LIBS    += "$$(HALCONROOT)/lib/$$(HALCONARCH)/halconcpp.lib" \
#                   "$$(HALCONROOT)/lib/$$(HALCONARCH)/halcon.lib"
#}




RC_ICONS =s21.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pcbtest
TEMPLATE = app


SOURCES += main.cpp\
    myserial.cpp \
    qiplinediter.cpp \
       widget.cpp \
    serverthread.cpp \
    login.cpp \
    single.cpp \
    pgbardelegate.cpp \
    server.cpp \
    mysocket.cpp \
    mcontent.cpp \
  projectwidget.cpp




HEADERS  +=   widget.h \
    myserial.h \
     mysocket.h \
    projectwidget.h \
    qiplinediter.h \
    server.h \
    serverthread.h \
    login.h \
    single.h \
    pgbardelegate.h \
    databasesql.h \
    struct.h \
    mcontent.h \
   config.h

FORMS    +=

RESOURCES += \
    res.qrc



#if 0
##win32: LIBS += -L'C:/Program Files/MVTec/HALCON-18.11-Progress/lib/x64-win64/' -lhalcon

##INCLUDEPATH += 'C:/Program Files/MVTec/HALCON-18.11-Progress/lib/x64-win64'
##DEPENDPATH += 'C:/Program Files/MVTec/HALCON-18.11-Progress/lib/x64-win64'

##includes
#INCLUDEPATH += "$$(HALCONROOT)/include"
#INCLUDEPATH += "$$(HALCONROOT)/include/cpp"
##libs
#QMAKE_LIBDIR     += "$$(HALCONROOT)/lib/$$(HALCONARCH)"
#unix:LIBS   += -lhalconcpp -lhalcon -ldl -lpthread
#win32:LIBS  += halconcpp.lib halcon.lib

#endif
