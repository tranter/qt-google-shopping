#-------------------------------------------------
#
# Project created by QtCreator 2012-01-24T07:45:24
#
#-------------------------------------------------
QT       += core gui webkit network

TARGET = qtshopping
TEMPLATE = app

linux* {
LIBS += ../qjson/build/lib/libqjson.so ../qwt-6.0/lib/libqwt.so
INCLUDEPATH += ../qjson/include ../qwt-6.0/src
}

win* {
LIBS += ../qjson/build/lib/qjson0.dll
LIBS += -L ../qwt-6.0/lib -lqwt

INCLUDEPATH += ../qjson/include
INCLUDEPATH += ../qwt-6.0/src/
}

macx* {
LIBS += -F../qjson/build/lib -framework qjson
INCLUDEPATH += ../qjson/include
}

SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    shopping_data_manager.cpp \
    productdetaildlg.cpp \
    facetplot.cpp

HEADERS  += mainwindow.h \
    form.h \
    shopping_data_manager.h \
    productdetaildlg.h \
    facetplot.h

FORMS    += mainwindow.ui \
    form.ui \
    productdetaildlg.ui \
    facetplot.ui

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    todo.txt



























































