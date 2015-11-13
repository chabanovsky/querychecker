/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QDebug>
#include "mainwindow.h"
#include "parser.h"

template<typename Arg, typename R, typename C>
struct InvokeWrapper {
    R *receiver;
    void (C::*memberFun)(Arg);
    void operator()(Arg result) {
        (receiver->*memberFun)(result);
    }
};

template<typename Arg, typename R, typename C>
InvokeWrapper<Arg, R, C> invoke(R *receiver, void (C::*memberFun)(Arg)) {
    InvokeWrapper<Arg, R, C> wrapper = {receiver, memberFun};
    return wrapper;
}

int MainWindow::pageLimit = 10;

MainWindow::MainWindow(const QUrl& url) {
    progress = 0;
    requestUrl = url;
    currentPage = 0;
    currentQueryIndex = -1;
    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();

    view = new QWebEngineView(this);

    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebEnginePage::Back));
    toolBar->addAction(view->pageAction(QWebEnginePage::Forward));
    toolBar->addAction(view->pageAction(QWebEnginePage::Reload));
    toolBar->addAction(view->pageAction(QWebEnginePage::Stop));
    toolBar->addWidget(locationEdit);

    setCentralWidget(view);

    Parser parser(":/stats.json");
    searchQueries = parser.Parse();

    runNextQuery();
}

void MainWindow::viewSource() {
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->adjustSize();
    textEdit->move(this->geometry().center() - textEdit->rect().center());
    textEdit->show();

    view->page()->toHtml(invoke(textEdit, &QTextEdit::setPlainText));
}

void MainWindow::adjustLocation() {
    locationEdit->setText(view->url().toString());
}

void MainWindow::runNextQuery() {
    currentPage = 0;
    ++currentQueryIndex;
    if (currentQueryIndex >= searchQueries.count()) {
        dumpResults();
        return;
    }

    SearchQuery & nextQuery = searchQueries[currentQueryIndex];
    view->load(QUrl(requestUrl.toString() +nextQuery.EncodingQuery()));
}

void MainWindow::dumpResults() {
    for (int index = 0; index < searchQueries.count(); ++index) {
        SearchQuery & query = searchQueries[index];
        qDebug() << "Query: " << query.Query() << ", in google: " << query.GoogleRank() << ", in yandex: " << query.YandexRank();
    }
    QApplication::quit();
}

void MainWindow::changeLocation() {
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    view->load(url);
    view->setFocus();
}

void MainWindow::adjustTitle() {
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p) {
    progress = p;
    adjustTitle();
}

void MainWindow::finishLoading(bool) {
    progress = 100;
    adjustTitle();
    view->page()->runJavaScript(jQuery);
    QString code = " \
function findUrl(){ \
    var search_url = 'ru.stackoverflow.com'; \
    var results = $('.serp-item'); \
    for (var index = 0; index < results.length; index++) { \
        var item = results[index]; \
        var anchor = $(item).find('.serp-url__link'); \
        var link = $(anchor).attr('href'); \
        if (link == undefined) { \
            continue; \
        } \
        if (link.indexOf(search_url) > 1){ \
            return parseInt($(item).attr('aria-posinset')); \
        } \
        return -1; \
    } \
}; findUrl();";
    view->page()->runJavaScript(code, invoke(this, &MainWindow::onResultFoundCallback));
}

void MainWindow::onResultFoundCallback(const QVariant& returnValue) {
    int result = -1;
    bool isOk = false;

    result = returnValue.toInt(&isOk);

    if (!isOk) {
        runNextQuery();
        return;
    }
    if (result > -1) {
        SearchQuery & query = searchQueries[currentQueryIndex];
        query.SetYandexRank(result);
        runNextQuery();
        return;
    }
    ++currentPage;
    if (currentPage >= MainWindow::pageLimit) {
        runNextQuery();
        return;
    }
    SearchQuery & query = searchQueries[currentQueryIndex];
    QUrl url = QUrl(requestUrl.toString() + query.EncodingQuery() + "&p=" + QString::number(currentPage));
    view->load(url);
}
