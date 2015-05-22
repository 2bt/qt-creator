#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T14:40:23
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = codemodelbackend
CONFIG   += console
CONFIG   -= app_bundle C++-14

TEMPLATE = app

include(ipcsource/codemodelbackendclangipc-source.pri)
include(../../../qtcreator.pri)
include(clang_installation.pri)

LIBS += -L$$OUT_PWD/../codemodelbackendipc/lib/qtcreator -lCodemodelbackendipc -lSqlite
LIBS += $$LLVM_LIBS
INCLUDEPATH += $$LLVM_INCLUDEPATH

INCLUDEPATH *= $$IDE_SOURCE_TREE/src/libs/codemodelbackendipc/source
INCLUDEPATH *= $$IDE_SOURCE_TREE/src/libs/sqlite

SOURCES += codemodelbackendmain.cpp

