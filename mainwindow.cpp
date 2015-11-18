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
#include "utils.h"
#include "searchquerytestcase.h"
#include "newquestiontestcase.h"

MainWindow::MainWindow() {
    progress = 0;
    view = new QWebEngineView(this);
    testCase = NULL;

    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(tr("Run SQ"), this, SLOT(runSearchQueryTest()));
    toolBar->addAction(tr("Run NQ"), this, SLOT(runNewQuestionTest()));
    toolBar->addAction(tr("Run NQWA"), this, SLOT(runNewQuestionWithAnswerTest()));
    toolBar->addAction(tr("Run NAQ"), this, SLOT(runNewAnsweredQuestionTest()));
    toolBar->addWidget(locationEdit);
    setCentralWidget(view);
}


QWebEngineView * MainWindow::WebEngineView() {
    return view;
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
}

void MainWindow::runSearchQueryTest() {
    if (testCase)
        return;
    SearchQueryTestCase * tcase = new SearchQueryTestCase(view);
    connect(tcase, SIGNAL(TestSuiteFinished()), SLOT(onTestCompeted()));
    testCase = tcase;
    testCase->Execute();
}

void MainWindow::runNewQuestionXHelper(NewQuestionTestCase::NewQuestionType questionsType) {
    if (testCase)
        return;
    NewQuestionTestCase * tcase = new NewQuestionTestCase(view, questionsType);
    connect(tcase, SIGNAL(TestSuiteFinished()), SLOT(onTestCompeted()));
    testCase = tcase;
    testCase->Execute();
}

void MainWindow::runNewQuestionTest() {
    runNewQuestionXHelper(NewQuestionTestCase::ANY);
}

void MainWindow::runNewQuestionWithAnswerTest() {
    runNewQuestionXHelper(NewQuestionTestCase::WITH_ANSWER);
}

void MainWindow::runNewAnsweredQuestionTest() {
    runNewQuestionXHelper(NewQuestionTestCase::ANSWERED);
}

void MainWindow::onTestCompeted() {
    if (!testCase)
        return;
    QString dump = testCase->DumpResults();
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->show();
    textEdit->setPlainText(dump);
    textEdit->setBaseSize(900, 600);

    delete testCase;
    testCase = NULL;
    view->load(QUrl("about:blank"));
}
