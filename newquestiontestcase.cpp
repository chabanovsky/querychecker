#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QDebug>
#include <stdexcept>

#include "newquestiontestcase.h"
#include "jquery.h"
#include "utils.h"

NewQuestionTestCase::NewQuestionTestCase (QWebEngineView * initView,NewQuestionType initType)
    : TestCase(),
    view(initView),
    soruQuestionUrl(DEFAULT_SORU_QUESTION_URL),
    currentSORUQuestionPageIndex(1),
    currentState(STACKOVERFLOW),
    yandexUrl(DEFAULT_YANDEX_SEARCH_URL),
    currentQuestionIndex (-1),
    questionsType(initType) {

    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
}

NewQuestionTestCase::~NewQuestionTestCase() {
}

NewQuestionTestCase::NewQuestionTestCase (NewQuestionTestCase const & other)
    : TestCase(other),
    view(other.view),
    soruQuestionUrl(other.soruQuestionUrl),
    currentState(other.currentState),
    yandexUrl(other.yandexUrl),
    currentQuestionIndex (other.currentQuestionIndex),
    questionsType(other.questionsType) {
}

NewQuestionTestCase& NewQuestionTestCase::operator=(NewQuestionTestCase const & other)  {
    if (&other == this) {
        return *this;
    }

    TestCase::operator =(other);
    view = other.view;
    soruQuestionUrl = other.soruQuestionUrl;
    currentState = other.currentState;
    yandexUrl = other.yandexUrl;
    currentQuestionIndex = other.currentQuestionIndex;
    questionsType = other.questionsType;
    return *this;
}

void NewQuestionTestCase::Execute() {
    questionsToSerch.clear();
    currentSORUQuestionPageIndex = 1;
    currentState = STACKOVERFLOW;
    currentQuestionIndex = -1;

    view->load(QUrl(soruQuestionUrl));
}

bool NewQuestionTestCase::CanExecute() {
    return true;
}

bool NewQuestionTestCase::Cancel() {
    return false;
}

QString NewQuestionTestCase::DumpResults() {
    QString dump;
    for (int index = 0; index < questionsToSerch.length(); ++ index) {
        Question & q = questionsToSerch[index];
        dump += QString("Exist in Yandex: ")
             + QString(q.ExistOnFirstPage ? "yes" : "no")
             + QString(", title: ")
             + q.Text
             + QString("\n");
    }
    return dump;
}

void NewQuestionTestCase::finishLoading(bool) {
    view->page()->runJavaScript(JQuery::Instance()->Code());
    switch (currentState) {
    case STACKOVERFLOW:
        view->page()->runJavaScript(jsCode(), invoke(this, &NewQuestionTestCase::onResultFoundCallback));
        break;
    case YANDEX:
        view->page()->runJavaScript(FIND_SORU_IN_YA_JS, invoke(this, &NewQuestionTestCase::onResultFoundCallback));
        break;
    default:
        throw std::invalid_argument("Invalid state was found during loading");
    }
}

void NewQuestionTestCase::onTestSuiteFinished() {
    emit TestSuiteFinished();
}

void NewQuestionTestCase::onResultFoundCallback(const QVariant& returnValue) {
    QUrl url;
    TestState oldState = currentState;
    switch (currentState) {
    case STACKOVERFLOW:
        url = onResultFoundStackOverflow(returnValue);
        break;
    case YANDEX:
        url = onResultFoundYandex(returnValue);
        break;
    default:
       throw std::invalid_argument("Invalid state was found on callback");
    }
    if (oldState != currentState && oldState == STACKOVERFLOW) {
        url = onResultFoundYandex(QVariant());
    }
    if (!url.isEmpty()) {
        view->load(url);
    }
}

QUrl NewQuestionTestCase::onResultFoundYandex(const QVariant& returnValue) {
    if (!returnValue.isNull()) {
        int result = -1;
        bool isOk = false;
        Question & q = question();
        result = returnValue.toInt(&isOk);

        q.ExistOnFirstPage = isOk && result >= 0;
    }

    ++currentQuestionIndex;

    if (currentQuestionIndex >= questionsToSerch.length()) {
        onTestSuiteFinished();
        return QUrl();
    }

    return QUrl(yandexUrl + question().Query());
}

QUrl NewQuestionTestCase::onResultFoundStackOverflow(const QVariant& returnValue) {
    QList<QVariant> first = returnValue.toList();
    for (int index = 0; index < first.length(); ++index) {
        QMap<QString, QVariant> second = first[index].toMap();
        Question question;
        question.Link = second["link"].toString();
        question.Text = second["text"].toString();
        questionsToSerch.append(question);
    }

    ++currentSORUQuestionPageIndex;
    if (questionsToSerch.length() >= QUESTION_TO_SEARCH_LIMIT) {
        currentState = YANDEX;
        return QUrl();
    }

    return QUrl(soruQuestionUrl +  "&page=" + QString::number(++currentSORUQuestionPageIndex));
}

NewQuestionTestCase::Question &NewQuestionTestCase::question() {
    if (currentQuestionIndex < 0 || currentQuestionIndex > questionsToSerch.count()) {
        throw std::invalid_argument("Invalid question index");
    }
    return questionsToSerch[currentQuestionIndex];
}
QString NewQuestionTestCase::jsCode() {
    switch(questionsType) {
    case ANY:
        return QUESTION_LIST_JS;
        break;
    case WITH_ANSWER:
        return QUESTION_WITH_ANSWER_LIST_JS;
        break;
    case ANSWERED:
        return ANSWERED_QUESTION_LIST_JS;
        break;
    default:
       throw std::invalid_argument("Invalid question type for js code");
    }
}
