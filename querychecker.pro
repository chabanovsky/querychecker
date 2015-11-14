QT      +=  webenginewidgets

macx: CONFIG -= app_bundle

HEADERS =   mainwindow.h \
    searchquery.h \
    parser.h \
    testcase.h \
    searchquerytestcase.h \
    utils.h \
    jquery.h \
    newquestiontestcase.h
SOURCES =   main.cpp \
            mainwindow.cpp \
    searchquery.cpp \
    parser.cpp \
    testcase.cpp \
    searchquerytestcase.cpp \
    jquery.cpp \
    newquestiontestcase.cpp
RESOURCES = jquery.qrc

# install
# target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/fancybrowser
INSTALLS += target

DISTFILES +=
