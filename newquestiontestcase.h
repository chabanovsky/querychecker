#ifndef NEWQUESTIONTESTCASE
#define NEWQUESTIONTESTCASE

#include <QtWidgets>
#include <QList>
#include <QString>
#include <QVariant>
#include <QObject>
#include <QString>

QT_BEGIN_NAMESPACE
class QWebEngineView;
QT_END_NAMESPACE

#include "testcase.h"

class NewQuestionTestCase : public QObject, public TestCase {
    Q_OBJECT
private:
    struct Question {
        QString Text;
        QString Link;
        bool ExistOnFirstPage;
        QString Query() {
            return QUrl::toPercentEncoding(Text);
        }
    };
    enum TestState { STACKOVERFLOW, YANDEX };
public:
    enum NewQuestionType { ANY, WITH_ANSWER, ANSWERED };
    NewQuestionTestCase (QWebEngineView * initView, NewQuestionType initType);
    virtual ~NewQuestionTestCase();
    NewQuestionTestCase (NewQuestionTestCase const &);
    NewQuestionTestCase& operator=(NewQuestionTestCase const &);

public:
    virtual void Execute();
    virtual bool CanExecute();
    virtual bool Cancel();
    virtual QString DumpResults();

signals:
    void TestSuiteFinished();

protected slots:
    void finishLoading(bool);

private:
    void onTestSuiteFinished();
    void onResultFoundCallback(const QVariant& returnValue);
    QUrl onResultFoundYandex(const QVariant& returnValue);
    QUrl onResultFoundStackOverflow(const QVariant& returnValue);
    Question& question();
    QString jsCode();

private:
    QWebEngineView * view;
    QString soruQuestionUrl;
    int currentSORUQuestionPageIndex;
    QList<Question> questionsToSerch;
    TestState currentState;
    QString yandexUrl;
    int currentQuestionIndex;
    NewQuestionType questionsType;
};

#endif // NEWQUESTIONTESTCASE

