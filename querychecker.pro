QT      +=  webenginewidgets

macx: CONFIG -= app_bundle

HEADERS =   mainwindow.h \
    searchquery.h \
    parser.h
SOURCES =   main.cpp \
            mainwindow.cpp \
    searchquery.cpp \
    parser.cpp
RESOURCES = jquery.qrc

# install
# target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/fancybrowser
INSTALLS += target

DISTFILES +=
